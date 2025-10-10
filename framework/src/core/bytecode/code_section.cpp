// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/bytecode/code_section.h"

namespace bibble {
    CodeSection::CodeSection(Section section)
        : mSection(section) {}

    std::optional<BytecodeReader> CodeSection::getBytecodeReader(size_t offset) const {
        if (offset >= mSection.getSize()) return std::nullopt;

        return BytecodeReader(mSection.getUnderlyingSpan(), offset);
    }
}
