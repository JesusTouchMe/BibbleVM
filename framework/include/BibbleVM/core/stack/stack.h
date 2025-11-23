// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_STACK_H
#define BIBBLEVM_CORE_STACK_H 1

#include "BibbleVM/core/value/value.h"

#include <memory>
#include <optional>

namespace bibble {
    // non-resizable lifo stack
    class Stack {
    public:
        Stack(u64 size);

        i64 sb() const;
        Value& sp();

        bool pushFrame(i64 minSize); // true on success
        bool popFrame(); // true on success

        bool isWithinBounds(i64 index) const; // index > sb && index < capacity

        Value& operator[](size_t index);
        const Value& operator[](size_t index) const;

    private:
        std::unique_ptr<Value[]> mMemory;
        i64 mCapacity;

        i64 mStackBase = -1; // index of the previous frames stack base value saved. used for stack underflow checks
        Value mStackPointer = 0; // SP register following the BibbleVM specification. stored as Value for simplicity
    };
}

#endif //BIBBLEVM_CORE_STACK_H
