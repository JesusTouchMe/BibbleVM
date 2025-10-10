// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/bytecode/strtab_section.h"

namespace bibble {
    StrtabSection::StrtabSection(Section section)
        : mSection(section) {}

    std::optional<std::string_view> StrtabSection::get(u32 offset) const {
        std::optional<u16> length = mSection.getU16(offset);
        if (!length.has_value()) return std::nullopt;

        return mSection.getString(offset, length.value());
    }
}
