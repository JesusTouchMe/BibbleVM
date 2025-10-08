// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_BYTECODE_STREAM_H
#define BIBBLEVM_CORE_BYTECODE_STREAM_H 1

#include "BibbleVM/core/bytecode/opcodes.h"

#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <variant>

namespace bibble {
    class BytecodeReader {
    public:
        BytecodeReader(std::span<const u8> bytes, size_t startPosition);

        size_t getSize() const;
        size_t getPosition() const;
        size_t getRemaining() const;

        // Returns true on success
        bool skip(i64 count);

        // All these return nullopt if out of bounds and never for any other reason

        std::optional<u8> fetchU8();
        std::optional<u16> fetchU16();
        std::optional<u32> fetchU32();
        std::optional<u64> fetchU64();

        std::optional<i8> fetchI8();
        std::optional<i16> fetchI16();
        std::optional<i32> fetchI32();
        std::optional<i64> fetchI64();

        std::optional<float> fetchFloat();
        std::optional<double> fetchDouble();

        // helper. will automatically fetch 1-3 bytes depending on encoding
        std::optional<std::variant<ByteOpcode, ExtendedOpcode>> fetchOpcode();

    private:
        std::span<const u8> mBytes;
        size_t mPosition;
    };
}

#endif // BIBBLEVM_CORE_BYTECODE_STREAM_H
