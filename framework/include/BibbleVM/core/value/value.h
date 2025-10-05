// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_VALUE_H
#define BIBBLEVM_CORE_VALUE_H 1

#include <cstdint>

namespace bibble {
    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    class Value {
    public:
        template<typename T>
        Value(T value)
            : mValue(value) {}

        Value() : mValue(0) {}

        i64& integer() { return mValue.integer; }
        i64  integer() const { return mValue.integer; }

        u64& uinteger() { return mValue.uinteger; }
        u64 uinteger() const { return mValue.uinteger; }

        double& floating() { return mValue.floating; }
        double  floating() const { return mValue.floating; }

        i64& boolean() { return mValue.integer; }
        bool boolean() const { return mValue.integer != 0; }

    private:
        union Generic {
            i64 integer;
            u64 uinteger;
            double floating;
        } mValue;
    };
}

#endif // BIBBLEVM_CORE_VALUE_H
