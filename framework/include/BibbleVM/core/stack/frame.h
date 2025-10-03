// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_FRAME_H
#define BIBBLEVM_CORE_FRAME_H 1

#include "BibbleVM/core/value/value.h"

#include <memory>

namespace bibble {
    class Frame {
    friend class Stack;
    public:
        Frame(size_t size, std::unique_ptr<Frame> previous);

        bool isWithinBounds(i64 index) const;

        Value& operator[](size_t index);
        const Value& operator[](size_t index) const;

    private:
        std::unique_ptr<Frame> mPrevious;
        std::unique_ptr<Value[]> mData;
        size_t mSize;
    };
}

#endif //BIBBLEVM_CORE_FRAME_H
