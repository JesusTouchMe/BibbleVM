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
        explicit Value(T value)
            : mValue(value) {}

        Value() : mValue(0) {}

        u64& integer() { return mValue.integer; }
        u64  integer() const { return mValue.integer; }

        double& floating() { return mValue.floating; }
        double  floating() const { return mValue.floating; }

    private:
        union Generic {
            u64 integer;
            double floating;
        } mValue;
    };
}

#endif // BIBBLEVM_CORE_VALUE_H
