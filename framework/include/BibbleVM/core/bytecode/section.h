// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_DATA_SECTION_H
#define BIBBLEVM_CORE_DATA_SECTION_H 1

#include "BibbleVM/core/value/value.h"

#include <optional>
#include <span>

namespace bibble {
    class Section {
    public:
        explicit Section(std::span<u8> bytes);

        std::span<u8>::iterator begin();
        std::span<u8>::iterator end();

        std::optional<u8*> get(size_t offset);
        std::optional<const u8*> get(size_t offset) const;

        std::optional<u8> getU8(size_t offset) const;
        std::optional<u16> getU16(size_t offset) const;
        std::optional<u32> getU32(size_t offset) const;
        std::optional<u64> getU64(size_t offset) const;

        std::optional<i8> getI8(size_t offset) const;
        std::optional<i16> getI16(size_t offset) const;
        std::optional<i32> getI32(size_t offset) const;
        std::optional<i64> getI64(size_t offset) const;

        std::optional<float> getFloat(size_t offset) const;
        std::optional<double> getDouble(size_t offset) const;

        // all return true on success

        bool setU8(size_t offset, u8 value);
        bool setU16(size_t offset, u16 value);
        bool setU32(size_t offset, u32 value);
        bool setU64(size_t offset, u64 value);

        bool setI8(size_t offset, i8 value);
        bool setI16(size_t offset, i16 value);
        bool setI32(size_t offset, i32 value);
        bool setI64(size_t offset, i64 value);

        bool setFloat(size_t offset, float value);
        bool setDouble(size_t offset, double value);

    private:
        std::span<u8> mBytes;
    };
}

#endif //BIBBLEVM_CORE_DATA_SECTION_H