// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/stack/stack.h"

namespace bibble {
    Frame* Stack::getTopFrame() const {
        return mTopFrame.get();
    }

    Frame* Stack::pushFrame(size_t size) {
        mTopFrame = std::make_unique<Frame>(size, std::move(mTopFrame));
        return mTopFrame.get();
    }

    bool Stack::popFrame() {
        if (!mTopFrame) return false;

        mTopFrame = std::move(mTopFrame->mPrevious);
        return true;
    }
}
