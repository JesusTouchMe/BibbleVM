// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/vm.h"

#include <algorithm>
#include <iostream>

namespace bibble {
    Module* VM::getModule(u32 index) const {
        if (mExited) return nullptr;
        if (index >= mModules.size()) return nullptr;

        return mModules[index].get();
    }

    Function* VM::getFunction(std::string_view name) const {
        if (mExited) return nullptr;
        auto it = mFunctions.find(name);
        if (it == mFunctions.end()) return nullptr;
        return it->second.get();
    }

    u32 VM::addModule(std::unique_ptr<Module> module) {
        if (mExited) return 0xFFFFFFFF;

        try {
            mModules.push_back(std::move(module));
            return mModules.size() - 1;
        } catch (...) {
            exit(1);
            return 0xFFFFFFFF;
        }
    }

    void VM::addFunction(std::unique_ptr<Function> function) {
        if (mExited) return;

        if (mFunctions.contains(function->getName())) {
            exit(1);
            return;
        }

        try {
            mFunctions.emplace(function->getName(), std::move(function));
        } catch (...) {
            exit(1);
        }
    }

    Module* VM::currentModule() {
        if (mExited) return nullptr;
        return getModule(currentModuleH());
    }

    u32 VM::currentModuleH() {
        if (mExited) return 0xFFFFFFFF;
        return mInterpreter.getActiveModule();
    }

    Value& VM::acc() {
        return mAccumulator;
    }

    Value& VM::sp() {
        return mStack.getTopFrame()->sp();
    }

    Stack& VM::stack() {
        return mStack;
    }

    bool VM::push(Value value) {
        if (mExited) return false;

        Frame* frame = mStack.getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(sp().integer())) return false;

        (*frame)[sp().integer()++] = value;

        return true;
    }

    std::optional<Value> VM::pop() {
        if (mExited) return std::nullopt;

        Frame* frame = mStack.getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(sp().integer() - 1)) return std::nullopt;

        return (*frame)[--sp().integer()];
    }

    bool VM::trap(u8 code) {
        if (mExited) return false;

        if (code == 0) {
            std::cout << acc().integer() << std::endl;
        }

        return true;
    }

    void VM::exit(int code) {
        if (mExited) return;
        mExitCode = code;
        mExited = true;
    }

    int VM::getExitCode() const {
        return mExitCode;
    }

    bool VM::hasExited() const {
        return mExited;
    }

    void VM::call(CallableTarget* target) {
        if (mExited) return;

        mInterpreter.execute(*this, target->module, target->entry);
    }

    VM::VM(VMConfig config)
        : mConfig(config)
        , mInterpreter(config) {}

    std::unique_ptr<VM> CreateVM(VMConfig config) {
        return std::unique_ptr<VM>(new VM(config));
    }
}
