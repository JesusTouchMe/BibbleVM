// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_INTERPRETER_H
#define BIBBLEVM_CORE_INTERPRETER_H 1

#include "BibbleVM/core/exec/dispatch.h"

namespace bibble {
    class VM;
    class Interpreter {
    public:
        explicit Interpreter(const VMConfig& config);

        void execute(VM& vm, BytecodeStream bytecode);

    private:
        DispatchTable mDispatchTable{};
        DispatchTableExt mDispatchTableExt{};
    };
}

#endif // BIBBLEVM_CORE_INTERPRETER_H
