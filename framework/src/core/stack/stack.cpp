// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/stack/stack.h"

namespace bibble {
    Stack::Stack(u64 size)
        : mMemory(std::make_unique<Value[]>(size))
        , mCapacity(static_cast<i64>(size)) {} // realistically this wouldn't integer overflow

    i64 Stack::sb() const {
        return mStackBase;
    }

    Value& Stack::sp() {
        return mStackPointer;
    }

    bool Stack::pushFrame(i64 minSize) {
        if (mStackPointer.integer() + minSize > mCapacity) {
            return false; // underflow
        }

        mMemory[mStackPointer.integer()].integer() = mStackBase;
        mStackBase = mStackPointer.integer();
        mStackPointer.integer() += 1;

        return true;
    }

    bool Stack::popFrame() {
        if (mStackBase < 0 ) {
            return false; // underflow
        }

        mStackPointer = mStackBase;
        mStackBase = mMemory[mStackPointer.integer()].integer();

        return true;
    }

    bool Stack::isWithinBounds(i64 index) const {
        return index > mStackBase && index < mCapacity;
    }

    Value& Stack::operator[](size_t index) {
        return mMemory[index];
    }

    const Value& Stack::operator[](size_t index) const {
        return mMemory[index];
    }
}
