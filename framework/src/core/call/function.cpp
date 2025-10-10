// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/call/function.h"

namespace bibble {
    Function::Function(u32 module, std::string_view name, CallableTarget target)
        : mModule(module)
        , mName(name)
        , mTarget(std::move(target)) {}

    u32 Function::getModule() const {
        return mModule;
    }

    std::string_view Function::getName() const {
        return mName;
    }

    CallableTarget& Function::target() {
        return mTarget;
    }
}
