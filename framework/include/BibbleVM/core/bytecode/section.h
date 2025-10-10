// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_SECTION_H
#define BIBBLEVM_CORE_SECTION_H 1

#include "BibbleVM/core/value/value.h"

#include <cstring>
#include <optional>
#include <span>
#include <string_view>

namespace bibble {
    class Section;

    template<class T>
    concept SectionReadable = requires(const Section& section, size_t offset) {
        { T::ReadFromSection(section, offset) } -> std::same_as<std::optional<T>>;
    };

    template<class T>
    concept SectionWritable = requires(const T t, Section& section, size_t offset) {
        { t.writeToSection(section, offset) } -> std::same_as<bool>;
    };

    class Section {
    public:
        explicit Section(std::span<u8> bytes);

        std::span<u8> getUnderlyingSpan() const;
        size_t getSize() const;

        std::span<u8>::iterator begin();
        std::span<u8>::iterator end();

        // Unsafe because they don't check how many bytes user intends to use from this. Check getBytes(size_t) for a safe version
        std::optional<u8*> getUnsafe(size_t offset);
        std::optional<const u8*> getUnsafe(size_t offset) const;

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

        std::optional<std::string_view> getString(size_t offset, size_t length) const;

        template<size_t Count>
        std::optional<const u8*> getBytes(size_t offset) const {
            if (offset + Count > mBytes.size()) return std::nullopt;
            return getUnsafe(offset);
        }

        template<size_t Count>
        bool getBytes(size_t offset, u8* out) const {
            if (offset + Count > mBytes.size()) return false;
            if constexpr (Count == 0) return true;

            std::memcpy(out, mBytes.data() + offset, Count);
            return true;
        }

        template<SectionReadable T>
        std::optional<T> getCustom(size_t offset) const {
            return T::ReadFromSection(*this, offset);
        }

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

        template<size_t Count>
        bool setBytes(size_t offset, const u8* bytes) {
            if (offset + Count > mBytes.size()) return false;
            if constexpr (Count == 0) return true;

            std::memcpy(mBytes.data() + offset, bytes, Count);
            return true;
        }

        template<SectionWritable T>
        bool setCustom(size_t offset, const T& value) {
            return value.writeToSection(*this, offset);
        }

    private:
        std::span<u8> mBytes;
    };
}

#endif //BIBBLEVM_CORE_SECTION_H