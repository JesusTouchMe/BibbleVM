// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_VM_H
#define BIBBLEVM_CORE_VM_H 1

#include "BibbleVM/core/value/value.h"

#include "BibbleVM/config.h"

#include <memory>

namespace bibble {
    class VM {
    friend std::unique_ptr<VM> CreateVM(VMConfig config);
    public:
        Value& accumulator();

    private:
        VMConfig mConfig;

        Value mAccumulator;

        explicit VM(VMConfig config);
    };

    std::unique_ptr<VM> CreateVM(VMConfig config);
}

#endif // BIBBLEVM_CORE_VM_H
