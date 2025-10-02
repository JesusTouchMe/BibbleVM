// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/vm.h"

namespace bibble {
    Value& VM::accumulator() {
        return mAccumulator;
    }

    VM::VM(VMConfig config)
        : mConfig(config) {}

    std::unique_ptr<VM> CreateVM(VMConfig config) {
        return std::make_unique<VM>(config);
    }
}
