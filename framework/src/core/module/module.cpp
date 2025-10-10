// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/module/module.h"

namespace bibble {
    Module::Module(std::unique_ptr<u8[]> bytecode, DataSection dataSection, StrtabSection strtabSection, CodeSection codeSection)
        : mBytecode(std::move(bytecode))
        , mDataSection(std::move(dataSection))
        , mStrtabSection(std::move(strtabSection))
        , mCodeSection(std::move(codeSection)) {}


    DataSection& Module::data() {
        return mDataSection;
    }

    const StrtabSection& Module::strtab() const {
        return mStrtabSection;
    }

    const CodeSection& Module::code() const {
        return mCodeSection;
    }
}
