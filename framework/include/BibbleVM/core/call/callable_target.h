// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_CALLABLE_TARGET_H
#define BIBBLEVM_CORE_CALLABLE_TARGET_H 1

#include "BibbleVM/core/bytecode/bytecode_reader.h"

namespace bibble {
    class VM;

    struct CallableTarget {
        u32 module;
        BytecodeReader entry;

        CallableTarget(u32 module, BytecodeReader entry) : module(module), entry(std::move(entry)) {}
    };

    // This exists to avoid headaches in the future when i add jit compiler or native functions.
    // Everything that calls a function in the VM calls this internally.
    void CallableTrampoline(const CallableTarget& target, VM& vm);
}

#endif // BIBBLEVM_CORE_CALLABLE_TARGET_H
