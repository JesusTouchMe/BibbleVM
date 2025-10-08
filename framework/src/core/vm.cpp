// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/vm.h"

#include <algorithm>
#include <iostream>

namespace bibble {
    Value& VM::acc() {
        return mStack.getTopFrame()->acc();
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

    void VM::execute(const BytecodeReader& stream) {
        if (mExited) return;

        mInterpreter.execute(*this, stream);
    }

    VM::VM(VMConfig config)
        : mConfig(config)
        , mInterpreter(config) {}

    std::unique_ptr<VM> CreateVM(VMConfig config) {
        return std::unique_ptr<VM>(new VM(config));
    }
}
