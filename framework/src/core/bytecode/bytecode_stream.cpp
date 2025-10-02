// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/bytecode/bytecode_stream.h"

#include <algorithm>

namespace bibble {
    BytecodeStream::BytecodeStream(std::span<const u8> bytes)
        : mBytes(bytes)
        , mPosition(0) {}

    size_t BytecodeStream::getSize() const {
        return mBytes.size();
    }

    size_t BytecodeStream::getPosition() const {
        return mPosition;
    }

    size_t BytecodeStream::getRemaining() const {
        return mBytes.size() - mPosition;
    }

    bool BytecodeStream::skip(size_t count) {
        if (count > getRemaining()) return false;
        mPosition += count;
        return true;
    }

    std::optional<u8> BytecodeStream::fetchU8() {
        if (getRemaining() < 1) return std::nullopt;

        u8 value = mBytes[mPosition++];
        return value;
    }

    std::optional<u16> BytecodeStream::fetchU16() {
        if (getRemaining() < 2) return std::nullopt;

        u16 value = (static_cast<u16>(mBytes[mPosition++]) << 8) |
                    (static_cast<u16>(mBytes[mPosition++]));

        return value;
    }

    std::optional<u32> BytecodeStream::fetchU32() {
        if (getRemaining() < 4) return std::nullopt;

        u32 value = (static_cast<u32>(mBytes[mPosition++]) << 24) |
                    (static_cast<u32>(mBytes[mPosition++]) << 16) |
                    (static_cast<u32>(mBytes[mPosition++]) << 8) |
                    (static_cast<u32>(mBytes[mPosition++]));

        return value;
    }

    std::optional<u64> BytecodeStream::fetchU64() {
        if (getRemaining() < 8) return std::nullopt;

        u32 value = (static_cast<u64>(mBytes[mPosition++]) << 56) |
                    (static_cast<u64>(mBytes[mPosition++]) << 48) |
                    (static_cast<u64>(mBytes[mPosition++]) << 40) |
                    (static_cast<u64>(mBytes[mPosition++]) << 32) |
                    (static_cast<u64>(mBytes[mPosition++]) << 24) |
                    (static_cast<u64>(mBytes[mPosition++]) << 16) |
                    (static_cast<u64>(mBytes[mPosition++]) << 8) |
                    (static_cast<u64>(mBytes[mPosition++]));

        return value;
    }

    std::optional<i8> BytecodeStream::fetchI8() {
        return fetchU8();
    }

    std::optional<i16> BytecodeStream::fetchI16() {
        return fetchU16();
    }

    std::optional<i32> BytecodeStream::fetchI32() {
        return fetchU32();
    }

    std::optional<i64> BytecodeStream::fetchI64() {
        return fetchU64();
    }

    std::optional<std::variant<ByteOpcode, ExtendedOpcode>> BytecodeStream::fetchOpcode() {
        auto opcode = fetchU8();
        if (!opcode.has_value()) return std::nullopt;

        if (opcode != 0xFF) return static_cast<ByteOpcode>(opcode.value());

        auto extendedOpcode = fetchU16();
        if (!extendedOpcode.has_value()) return std::nullopt;

        return static_cast<ExtendedOpcode>(extendedOpcode.value());
    }
}
