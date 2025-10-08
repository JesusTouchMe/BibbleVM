// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_BYTECODE_STRUCT_DATA_SECTION_H
#define BIBBLEVM_CORE_BYTECODE_STRUCT_DATA_SECTION_H 1

#include "BibbleVM/core/bytecode/section.h"

namespace bibble {
    struct NameWrapper {
        u8 name[8];

        NameWrapper(const u8* p) {
            name[0] = p[0];
            name[1] = p[1];
            name[2] = p[2];
            name[3] = p[3];
            name[4] = p[4];
            name[5] = p[5];
            name[6] = p[6];
            name[7] = p[7];
        }
    };

    struct CallEntry {
        u32 module;
        u32 address;
        std::optional<NameWrapper> name;

        static std::optional<CallEntry> ReadFromSection(const Section& section, size_t offset);
        bool writeToSection(Section& section, size_t offset);
    };
}

#endif //BIBBLEVM_CORE_BYTECODE_STRUCT_DATA_SECTION_H