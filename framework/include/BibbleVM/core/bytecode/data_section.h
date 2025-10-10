// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_DATA_SECTION_H
#define BIBBLEVM_CORE_DATA_SECTION_H 1

#include "BibbleVM/core/bytecode/strtab_section.h"

#include "BibbleVM/core/call/callable_target.h"

#include <unordered_map>
#include <vector>

namespace bibble {
    class VM;

    struct CallEntry {
        u32 module;
        u32 address;
        std::optional<std::array<u8, 8>> name;

        static std::optional<CallEntry> ReadFromSection(const Section& section, size_t offset);
        bool writeToSection(Section& section, size_t offset) const;
    };

    class DataSection {
    public:
        explicit DataSection(Section section);

        std::optional<i8> getByte(u32 offset);
        std::optional<i16> getShort(u32 offset);
        std::optional<i32> getInt(u32 offset);
        std::optional<i64> getLong(u32 offset);
        std::optional<float> getFloat(u32 offset);
        std::optional<double> getDouble(u32 offset);
        std::optional<std::string_view> getString(u32 offset, const StrtabSection& strtab);

        const CallableTarget* getCallable(u32 offset, VM& vm);

    private:
        Section mSection;

        std::vector<std::unique_ptr<CallableTarget>> mCallableTargets; // Stores and owns callable targets that aren't resolved from functions
        std::unordered_map<u32, CallableTarget*> mCallableCache; // cache

        std::optional<std::string_view> resolveString(const u8 bytes[8], const StrtabSection&);
    };
}

#endif // BIBBLEVM_CORE_DATA_SECTION_H
