// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/call/callable_target.h"

#include "BibbleVM/core/vm.h"

namespace bibble {
    void CallableTrampoline(const CallableTarget& target, VM& vm) {
        vm.interpreter().execute(vm, target.module, target.entry);
    }
}
