// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_CODE_SECTION_H
#define BIBBLEVM_CORE_CODE_SECTION_H 1

#include "BibbleVM/core/bytecode/bytecode_reader.h"
#include "BibbleVM/core/bytecode/section.h"

namespace bibble {
    class CodeSection {
    public:
        explicit CodeSection(Section section);

        std::optional<BytecodeReader> getBytecodeReader(size_t offset) const;

    private:
        Section mSection;
    };
}

#endif // BIBBLEVM_CORE_CODE_SECTION_H
