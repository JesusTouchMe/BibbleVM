// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/bytecode/structs/data_section.h"

namespace bibble {
    std::optional<CallEntry> CallEntry::ReadFromSection(const Section& section, size_t offset) {
        std::optional<u32> module = section.getU32(offset);
        std::optional<u32> address = section.getU32(offset + 4);
        std::optional<u8[8]> name = std::nullopt;

        if (!module.has_value() || !address.has_value()) return std::nullopt;

        if (address.value() == 0xFFFFFFFF) {
            u8 value[8];
            if (!section.getBytes<8>(offset + 8, value)) return std::nullopt;

            name = value;
        }
    }

    bool CallEntry::writeToSection(Section& section, size_t offset) {
    }
}