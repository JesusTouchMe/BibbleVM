// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CONFIG_H
#define BIBBLEVM_CONFIG_H 1

namespace bibble {
    struct VMConfig {
        i64 stackSize = 0x100000; // this is the value of 8MB divided by 8 which is the size of a stack slot. in total, gives us 8mb big stack
        bool sandbox = false;
    };
}

#endif // BIBBLEVM_CONFIG_H
