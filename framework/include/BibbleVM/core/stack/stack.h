// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_STACK_H
#define BIBBLEVM_CORE_STACK_H 1

#include "BibbleVM/core/stack/frame.h"

namespace bibble {
    class Stack {
    public:
        Frame* getTopFrame() const;

        Frame* pushFrame(size_t size);
        bool popFrame(); // true on success

    private:
        std::unique_ptr<Frame> mTopFrame = nullptr;
    };
}

#endif //BIBBLEVM_CORE_STACK_H
