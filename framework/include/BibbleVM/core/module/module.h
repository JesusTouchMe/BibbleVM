// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_MODULE_H
#define BIBBLEVM_CORE_MODULE_H 1

#include "BibbleVM/core/bytecode/code_section.h"
#include "BibbleVM/core/bytecode/data_section.h"
#include "BibbleVM/core/bytecode/strtab_section.h"

#include <memory>

namespace bibble {
    // This represents a loaded bbx file and holds its bytecode and parsed sections.
    class Module {
    public:
        Module(std::unique_ptr<u8[]> bytecode, DataSection dataSection, StrtabSection strtabSection, CodeSection codeSection);

        DataSection& data();
        const StrtabSection& strtab() const;
        const CodeSection& code() const;

    private:
        std::unique_ptr<u8[]> mBytecode;

        DataSection mDataSection;
        StrtabSection mStrtabSection;
        CodeSection mCodeSection;
    };
}

#endif // BIBBLEVM_CORE_MODULE_H
