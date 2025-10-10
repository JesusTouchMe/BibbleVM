// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_STRTAB_SECTION_H
#define BIBBLEVM_CORE_STRTAB_SECTION_H 1

#include "BibbleVM/core/bytecode/section.h"

#include <string_view>

namespace bibble {
    class StrtabSection {
    public:
        explicit StrtabSection(Section section);

        std::optional<std::string_view> get(u32 offset) const;

    private:
        Section mSection;
    };
}

#endif // BIBBLEVM_CORE_STRTAB_SECTION_H
