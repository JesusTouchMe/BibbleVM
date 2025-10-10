// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_INTERPRETER_H
#define BIBBLEVM_CORE_INTERPRETER_H 1

#include "BibbleVM/core/exec/dispatch.h"

namespace bibble {
    class VM;
    class Interpreter {
    public:
        explicit Interpreter(const VMConfig& config);

        u32 getActiveModule() const;

        void execute(VM& vm, u32 module, BytecodeReader bytecode);

    private:
        DispatchTable mDispatchTable{};
        DispatchTableExt mDispatchTableExt{};

        u32 mActiveModule;
    };
}

#endif // BIBBLEVM_CORE_INTERPRETER_H
