// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_OPCODES_H
#define BIBBLEVM_CORE_OPCODES_H 1

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


    enum class ByteOpcode : u8 {
        NOP = 0x00,
        HLT = 0x01,
        TRAP = 0x02,
        TRAP_IF_ZERO = 0x03,
        TRAP_IF_NOT_ZERO = 0x04,
        BRK = 0x05,

        ADD = 0x10,
        SUB = 0x11,
        MUL = 0x12,
        DIV = 0x13,
        MOD = 0x14,
        AND = 0x15,
        OR = 0x16,
        XOR = 0x17,
        SHL = 0x18,
        SHR = 0x19,
        NEG = 0x1A,
        NOT = 0x1B,
        ADD2 = 0x1C,
        SUB2 = 0x1D,
        MUL2 = 0x1E,
        DIV2 = 0x1F,
        MOD2 = 0x20,
        AND2 = 0x21,
        OR2 = 0x22,
        XOR2 = 0x23,
        SHL2 = 0x24,
        SHR2 = 0x25,
        ADD_ST = 0x26,
        SUB_ST = 0x27,
        MUL_ST = 0x28,
        DIV_ST = 0x29,
        MOD_ST = 0x2A,
        AND_ST = 0x2B,
        OR_ST = 0x2C,
        XOR_ST = 0x2D,
        SHL_ST = 0x2E,
        SHR_ST = 0x2F,
        NEG_ST = 0x30,
        NOT_ST = 0x31,
        ADD_IMM = 0x32,
        SUB_IMM = 0x33,
        MUL_IMM = 0x34,
        DIV_IMM = 0x35,
        MOD_IMM = 0x36,
        AND_IMM = 0x37,
        OR_IMM = 0x38,
        XOR_IMM = 0x39,
        SHL_IMM = 0x3A,
        SHR_IMM = 0x3B,
        ADD_IMM_ST = 0x3C,
        SUB_IMM_ST = 0x3D,
        MUL_IMM_ST = 0x3E,
        DIV_IMM_ST = 0x3F,
        MOD_IMM_ST = 0x40,
        AND_IMM_ST = 0x41,
        OR_IMM_ST = 0x42,
        XOR_IMM_ST = 0x43,
        SHL_IMM_ST = 0x44,
        SHR_IMM_ST = 0x45,

        FADD = 0x50,
        FSUB = 0x51,
        FMUL = 0x52,
        FDIV = 0x53,
        FADD2 = 0x54,
        FSUB2 = 0x55,
        FMUL2 = 0x56,
        FDIV2 = 0x57,
        FADD_ST = 0x58,
        FSUB_ST = 0x59,
        FMUL_ST = 0x5A,
        FDIV_ST = 0x5B,
        FNEG = 0x5C,
        FADD_IMM = 0x5D,
        FSUB_IMM = 0x5E,
        FMUL_IMM = 0x5F,
        FDIV_IMM = 0x60,
        FADD_IMM_ST = 0x61,
        FSUB_IMM_ST = 0x62,
        FMUL_IMM_ST = 0x63,
        FDIV_IMM_ST = 0x64,

        PUSH_ACC = 0x70,
        PUSH_SP = 0x71,
        POP_ACC = 0x72,
        POP_SP = 0x73,
        POP_DISCARD = 0x74,
        CONST = 0x75,
        CONST32 = 0x76,
        CONST64 = 0x77,
        CONST_ST = 0x78,
        CONST32_ST = 0x79,
        CONST64_ST = 0x7A,
        LOAD = 0x7B,
        LOAD_ST = 0x7C,
        STORE = 0x7D,
        STORE_ST = 0x7E,
    };

    enum class ExtendedOpcode : u16 {

    };
}

#endif // BIBBLEVM_CORE_OPCODES_H
