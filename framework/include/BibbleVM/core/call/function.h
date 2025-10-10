// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_FUNCTION_H
#define BIBBLEVM_CORE_FUNCTION_H 1

#include "BibbleVM/core/call/callable_target.h"

namespace bibble {
    class Function {
    public:
        Function(u32 module, std::string_view name, CallableTarget target);

        u32 getModule() const;
        std::string_view getName() const;
        CallableTarget& target();

    private:
        u32 mModule; // the module that defined this function symbol. as a handle

        std::string_view mName;
        CallableTarget mTarget;
    };
}

#endif // BIBBLEVM_CORE_FUNCTION_H
