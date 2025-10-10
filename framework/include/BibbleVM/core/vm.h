// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_VM_H
#define BIBBLEVM_CORE_VM_H 1

#include "BibbleVM/core/call/function.h"

#include "BibbleVM/core/exec/interpreter.h"

#include "BibbleVM/core/module/module.h"

#include "BibbleVM/core/stack/stack.h"

#include "BibbleVM/util/string.h"

#include "BibbleVM/config.h"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace bibble {
    static_assert(std::numeric_limits<float>::is_iec559, "float is not IEEE-754");
    static_assert(std::numeric_limits<double>::is_iec559, "double is not IEEE-754");

    class VM {
    friend std::unique_ptr<VM> CreateVM(VMConfig config);
    public:
        Module* getModule(u32 index) const;
        Function* getFunction(std::string_view name) const;

        u32 addModule(std::unique_ptr<Module> module);
        void addFunction(std::unique_ptr<Function> function);

        Module* currentModule();
        u32 currentModuleH();

        Value& acc();
        Value& sp();
        Stack& stack();

        // true on success
        bool push(Value value);
        std::optional<Value> pop();

        bool trap(u8 code);

        void exit(int code);
        int getExitCode() const;
        bool hasExited() const;

        void call(CallableTarget* target);

    private:
        VMConfig mConfig;

        std::vector<std::unique_ptr<Module>> mModules;
        std::unordered_map<std::string, std::unique_ptr<Function>, util::StringHash, util::StringEq> mFunctions;

        Value mAccumulator;
        Stack mStack;
        Interpreter mInterpreter;

        int mExitCode = 0;
        bool mExited = false;

        explicit VM(VMConfig config);
    };

    std::unique_ptr<VM> CreateVM(VMConfig config = {});
}

#endif // BIBBLEVM_CORE_VM_H
