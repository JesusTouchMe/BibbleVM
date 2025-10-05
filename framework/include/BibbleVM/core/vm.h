// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_VM_H
#define BIBBLEVM_CORE_VM_H 1

#include "BibbleVM/core/exec/interpreter.h"

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
        Stack& stack();

        // true on success
        bool push(Value value);
        std::optional<Value> pop();

        bool trap(u8 code);

        void exit(int code);
        int getExitCode();
        bool hasExited() const;

        void execute(const BytecodeStream& stream);

    private:
        VMConfig mConfig;

        Stack mStack;
        Interpreter mInterpreter;

        int mExitCode = 0;
        bool mExited = false;

        explicit VM(VMConfig config);
    };

    std::unique_ptr<VM> CreateVM(VMConfig config = {});
}

#endif // BIBBLEVM_CORE_VM_H
