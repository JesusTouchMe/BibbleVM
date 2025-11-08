// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/stack/frame.h"

namespace bibble {
    Frame::Frame(size_t size, std::unique_ptr<Frame> previous)
        : mPrevious(std::move(previous))
        , mData(std::make_unique<Value[]>(size))
        , mSize(size)
        , mStackPointer(0) {}

    bool Frame::isWithinBounds(i64 index) const {
        return index >= 0 && index < mSize;
    }

    Value& Frame::sp() {
        return mStackPointer;
    }

    Value& Frame::operator[](size_t index) {
        return mData[index];
    }

    const Value& Frame::operator[](size_t index) const {
        return mData[index];
    }
}
