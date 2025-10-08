// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/bytecode/section.h"

namespace bibble {
    Section::Section(std::span<u8> bytes)
        : mBytes(bytes) {}

    std::span<u8>::iterator Section::begin() {
        return mBytes.begin();
    }

    std::span<u8>::iterator Section::end() {
        return mBytes.end();
    }

    std::optional<u8*> Section::get(size_t offset) {
        if (offset >= mBytes.size()) return std::nullopt;

        return mBytes.data() + offset;
    }

    std::optional<const u8*> Section::get(size_t offset) const {
        if (offset >= mBytes.size()) return std::nullopt;

        return mBytes.data() + offset;
    }

    std::optional<u8> Section::getU8(size_t offset) const {
        if (offset >= mBytes.size()) return std::nullopt;

        return mBytes[offset];
    }

    std::optional<u16> Section::getU16(size_t offset) const {
        if (offset + 1 >= mBytes.size()) return std::nullopt;

        const u8* p = mBytes.data() + offset;
        u16 value = (static_cast<u16>(p[0]) << 8) |
                    (static_cast<u16>(p[1]));
        return value;
    }

    std::optional<u32> Section::getU32(size_t offset) const {
        if (offset + 3 >= mBytes.size()) return std::nullopt;

        const u8* p = mBytes.data() + offset;
        u32 value = (static_cast<u32>(p[0]) << 24) |
                    (static_cast<u32>(p[1]) << 16) |
                    (static_cast<u32>(p[2]) << 8) |
                    (static_cast<u32>(p[3]));
        return value;
    }

    std::optional<u64> Section::getU64(size_t offset) const {
        if (offset + 7 >= mBytes.size()) return std::nullopt;

        const u8* p = mBytes.data() + offset;
        u32 value = (static_cast<u64>(p[0]) << 56) |
                    (static_cast<u64>(p[1]) << 48) |
                    (static_cast<u64>(p[2]) << 40) |
                    (static_cast<u64>(p[3]) << 32) |
                    (static_cast<u64>(p[4]) << 24) |
                    (static_cast<u64>(p[5]) << 16) |
                    (static_cast<u64>(p[6]) << 8) |
                    (static_cast<u64>(p[7]));
        return value;
    }

    std::optional<i8> Section::getI8(size_t offset) const {
        return getU8(offset);
    }

    std::optional<i16> Section::getI16(size_t offset) const {
        return getU16(offset);
    }

    std::optional<i32> Section::getI32(size_t offset) const {
        return getU32(offset);
    }

    std::optional<i64> Section::getI64(size_t offset) const {
        return getU64(offset);
    }

    std::optional<float> Section::getFloat(size_t offset) const {
        static_assert(sizeof(float) == sizeof(u32), "float and u32 sizes don't match");

        std::optional<u32> raw = getU32(offset);
        if (!raw.has_value()) return std::nullopt;

        float value;
        std::memcpy(&value, &raw, sizeof(value));

        return value;
    }

    std::optional<double> Section::getDouble(size_t offset) const {
        static_assert(sizeof(double) == sizeof(u64), "double and u64 sizes don't match");

        std::optional<u64> raw = getU64(offset);
        if (!raw.has_value()) return std::nullopt;

        double value;
        std::memcpy(&value, &raw, sizeof(value));

        return value;
    }

    bool Section::setU8(size_t offset, u8 value) {
        if (offset >= mBytes.size()) return false;

        mBytes[offset] = value;
        return true;
    }

    bool Section::setU16(size_t offset, u16 value) {
        if (offset + 1 >= mBytes.size()) return false;

        u8* p = mBytes.data() + offset;
        p[0] = static_cast<u8>(value >> 8);
        p[1] = static_cast<u8>(value);

        return true;
    }

    bool Section::setU32(size_t offset, u32 value) {
        if (offset + 3 >= mBytes.size()) return false;

        u8* p = mBytes.data() + offset;
        p[0] = static_cast<u8>(value >> 24);
        p[1] = static_cast<u8>(value >> 16);
        p[2] = static_cast<u8>(value >> 8);
        p[3] = static_cast<u8>(value);

        return true;
    }

    bool Section::setU64(size_t offset, u64 value) {
        if (offset + 7 >= mBytes.size()) return false;

        u8* p = mBytes.data() + offset;
        p[0] = static_cast<u8>(value >> 56);
        p[1] = static_cast<u8>(value >> 48);
        p[2] = static_cast<u8>(value >> 40);
        p[3] = static_cast<u8>(value >> 32);
        p[4] = static_cast<u8>(value >> 24);
        p[5] = static_cast<u8>(value >> 16);
        p[6] = static_cast<u8>(value >> 8);
        p[7] = static_cast<u8>(value);

        return true;
    }

    bool Section::setI8(size_t offset, i8 value) {
        return setU8(offset, value);
    }

    bool Section::setI16(size_t offset, i16 value) {
        return setU16(offset, value);
    }

    bool Section::setI32(size_t offset, i32 value) {
        return setU32(offset, value);
    }

    bool Section::setI64(size_t offset, i64 value) {
        return setU64(offset, value);
    }

    bool Section::setFloat(size_t offset, float value) {
        static_assert(sizeof(float) == sizeof(u32), "float and u32 sizes don't match");

        u32 raw;
        std::memcpy(&raw, &value, sizeof(value));
        return setU32(offset, raw);
    }

    bool Section::setDouble(size_t offset, double value) {
        static_assert(sizeof(double) == sizeof(u64), "double and u64 sizes don't match");

        u64 raw;
        std::memcpy(&raw, &value, sizeof(value));
        return setU64(offset, raw);
    }
}
