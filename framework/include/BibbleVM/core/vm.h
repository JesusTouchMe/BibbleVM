// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_VM_H
#define BIBBLEVM_CORE_VM_H 1

#include "BibbleVM/core/stack/stack.h"

#include "BibbleVM/config.h"

#include <memory>
#include <optional>

namespace bibble {
    class VM {
    friend std::unique_ptr<VM> CreateVM(VMConfig config);
    public:
        Value& acc();
        Value& sp();
        Value& pc();
        Stack& stack();

        // true on success
        bool push(Value value);
        std::optional<Value> pop();

    private:
        VMConfig mConfig;

        Value mAccumulator;
        Value mStackPointer;
        Value mProgramCounter;
        Stack mStack;

        explicit VM(VMConfig config);
    };

    std::unique_ptr<VM> CreateVM(VMConfig config);
}

#endif // BIBBLEVM_CORE_VM_H
