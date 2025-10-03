// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/vm.h"

namespace bibble {
    Value& VM::acc() {
        return mAccumulator;
    }

    Value& VM::sp() {
        return mStackPointer;
    }

    Value& VM::pc() {
        return mProgramCounter;
    }

    Stack& VM::stack() {
        return mStack;
    }

    bool VM::push(Value value) {
        Frame* frame = mStack.getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(sp().integer())) return false;

        (*frame)[sp().integer()++] = value;

        return true;
    }

    std::optional<Value> VM::pop() {
        Frame* frame = mStack.getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(sp().integer() - 1)) return std::nullopt;

        return (*frame)[--sp().integer()];
    }

    VM::VM(VMConfig config)
        : mConfig(config) {}

    std::unique_ptr<VM> CreateVM(VMConfig config) {
        return std::make_unique<VM>(config);
    }
}
