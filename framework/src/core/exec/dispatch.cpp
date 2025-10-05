// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/exec/dispatch.h"

#include "BibbleVM/core/vm.h"

#define DEFINE_DISPATCH(opcode) bool Dispatch_##opcode(VM& vm, BytecodeStream& code)
#define REGISTER_DISPATCH(table, opcode) table[static_cast<size_t>(ByteOpcode::opcode)] = Dispatch_##opcode
#define REGISTER_DISPATCH_EXT(table, opcode) table[static_cast<size_t>(ExtendedOpcode::opcode)] = Dispatch_##opcode

#define DISPATCH_SUCCEED() return true
#define DISPATCH_FAIL() do { vm.exit(-2); return false; } while(0)

namespace bibble {
    DEFINE_DISPATCH(NOP) {
        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(HLT) {
        std::optional<i8> exitCode = code.fetchI8();
        if (!exitCode.has_value()) DISPATCH_FAIL();

        vm.exit(exitCode.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(TRAP) {
        std::optional<u8> trapCode = code.fetchU8();
        if (!trapCode.has_value()) DISPATCH_FAIL();

        if (!vm.trap(trapCode.value())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(TRAP_IF_ZERO) {
        std::optional<u8> trapCode = code.fetchU8();
        if (!trapCode.has_value()) DISPATCH_FAIL();

        if (vm.acc().integer() == 0) {
            if (!vm.trap(trapCode.value())) DISPATCH_FAIL();
        }

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(TRAP_IF_NOT_ZERO) {
        std::optional<u8> trapCode = code.fetchU8();
        if (!trapCode.has_value()) DISPATCH_FAIL();

        if (vm.acc().integer() != 0) {
            if (!vm.trap(trapCode.value())) DISPATCH_FAIL();
        }

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(BRK) {
        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(ADD) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() += s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SUB) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() -= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MUL) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() *= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(DIV) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() /= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MOD) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() %= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(AND) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() &= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(OR) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() |= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(XOR) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() ^= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHL) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() <<= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHR) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().integer() >>= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(NEG) {
        vm.acc().integer() = -vm.acc().integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(NOT) {
        vm.acc().integer() = ~vm.acc().integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(ADD2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() + s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SUB2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() - s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MUL2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() * s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(DIV2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() / s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MOD2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() % s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(AND2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() & s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(OR2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() | s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(XOR2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() ^ s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHL2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() << s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHR2) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = s1->integer() >> s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(ADD_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() + s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SUB_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() - s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MUL_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() * s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(DIV_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() / s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MOD_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() % s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(AND_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() & s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(OR_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() | s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(XOR_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() ^ s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHL_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() << s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHR_ST) {
        std::optional<Value> s0 = vm.pop();
        std::optional<Value> s1 = vm.pop();
        if (!s0.has_value() || !s1.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1->integer() >> s0->integer())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(NEG_ST) {
        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() = -((*frame)[index].integer());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(NOT_ST) {
        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() = ~((*frame)[index].integer());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(ADD_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() += value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SUB_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() -= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MUL_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() *= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(DIV_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() /= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MOD_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() %= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(AND_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() &= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(OR_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() |= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(XOR_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() ^= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHL_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() <<= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHR_IMM) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() >>= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(ADD_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() += value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SUB_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() -= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MUL_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() *= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(DIV_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() /= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(MOD_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() %= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(AND_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() &= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(OR_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() |= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(XOR_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() ^= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHL_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() <<= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(SHR_IMM_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].integer() >>= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FADD) {
        std::optional<Value> s0f = vm.pop();
        if (!s0f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() += s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FSUB) {
        std::optional<Value> s0f = vm.pop();
        if (!s0f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() -= s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FMUL) {
        std::optional<Value> s0f = vm.pop();
        if (!s0f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() *= s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FDIV) {
        std::optional<Value> s0f = vm.pop();
        if (!s0f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() /= s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FADD2) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() = s1f->floating() + s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FSUB2) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() = s1f->floating() - s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FMUL2) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() = s1f->floating() * s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FDIV2) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        vm.acc().floating() = s1f->floating() / s0f->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FADD_ST) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1f->floating() + s0f->floating())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FSUB_ST) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1f->floating() - s0f->floating())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FMUL_ST) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1f->floating() * s0f->floating())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FDIV_ST) {
        std::optional<Value> s0f = vm.pop();
        std::optional<Value> s1f = vm.pop();
        if (!s0f.has_value() || !s1f.has_value()) DISPATCH_FAIL();

        if (!vm.push(s1f->floating() / s0f->floating())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FNEG) {
        vm.acc().floating() = -vm.acc().floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FADD_IMM) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().floating() += value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FSUB_IMM) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().floating() -= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FMUL_IMM) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().floating() *= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FDIV_IMM) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().floating() /= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FADD_IMM_ST) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].floating() += value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FSUB_IMM_ST) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].floating() -= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FMUL_IMM_ST) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].floating() *= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FDIV_IMM_ST) {
        std::optional<float> value = code.fetchFloat();
        if (!value.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        i64 index = vm.sp().integer() - 1;
        if (frame == nullptr || !frame->isWithinBounds(index)) DISPATCH_FAIL();

        (*frame)[index].floating() /= value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_EQ) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().integer() == s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_NE) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().integer() != s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_LT) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().integer() < s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_GT) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().integer() > s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_LTE) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().integer() <= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_GTE) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().integer() >= s0->integer();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_EQ) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().floating() == s0->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_NE) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().floating() != s0->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_LT) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().floating() < s0->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_GT) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().floating() > s0->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_LTE) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().floating() <= s0->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_GTE) {
        std::optional<Value> s0 = vm.pop();
        if (!s0.has_value()) DISPATCH_FAIL();

        vm.acc().boolean() = vm.acc().floating() >= s0->floating();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_EQ0) {
        vm.acc().boolean() = vm.acc().integer() == 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_NE0) {
        vm.acc().boolean() = vm.acc().integer() != 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_LT0) {
        vm.acc().boolean() = vm.acc().integer() < 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_GT0) {
        vm.acc().boolean() = vm.acc().integer() > 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_LTE0) {
        vm.acc().boolean() = vm.acc().integer() <= 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CMP_GTE0) {
        vm.acc().boolean() = vm.acc().integer() >= 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_EQ0) {
        vm.acc().boolean() = vm.acc().floating() == 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_NE0) {
        vm.acc().boolean() = vm.acc().floating() != 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_LT0) {
        vm.acc().boolean() = vm.acc().floating() < 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_GT0) {
        vm.acc().boolean() = vm.acc().floating() > 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_LTE0) {
        vm.acc().boolean() = vm.acc().floating() <= 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(FCMP_GTE0) {
        vm.acc().boolean() = vm.acc().floating() >= 0;

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(PUSH_ACC) {
        if (!vm.push(vm.acc())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(PUSH_SP) {
        if (!vm.push(vm.sp())) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(POP_ACC) {
        std::optional<Value> value = vm.pop();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc() = value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(POP_SP) {
        std::optional<Value> value = vm.pop();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.sp() = value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(POP_DISCARD) {
        std::optional<u8> count = code.fetchU8();
        if (!count.has_value()) DISPATCH_FAIL();

        if (count.value() > vm.sp().integer()) DISPATCH_FAIL();

        vm.sp().integer() -= count.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CONST) {
        std::optional<i8> value = code.fetchI8();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = static_cast<i64>(value.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CONST32) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = static_cast<i64>(value.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CONST64) {
        std::optional<i64> value = code.fetchI64();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.acc().integer() = value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CONST_ST) {
        std::optional<i8> value = code.fetchI8();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.push(value.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CONST32_ST) {
        std::optional<i32> value = code.fetchI32();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.push(value.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(CONST64_ST) {
        std::optional<i64> value = code.fetchI64();
        if (!value.has_value()) DISPATCH_FAIL();

        vm.push(value.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(LOAD) {
        std::optional<i16> index = code.fetchI16();
        if (!index.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(index.value())) DISPATCH_FAIL();

        vm.acc() = (*frame)[index.value()];

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(LOAD_ST) {
        std::optional<i16> index = code.fetchI16();
        if (!index.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(index.value())) DISPATCH_FAIL();

        if (!vm.push((*frame)[index.value()])) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(STORE) {
        std::optional<i16> index = code.fetchI16();
        if (!index.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(index.value())) DISPATCH_FAIL();

       (*frame)[index.value()] = vm.acc();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(STORE_ST) {
        std::optional<i16> index = code.fetchI16();
        if (!index.has_value()) DISPATCH_FAIL();

        Frame* frame = vm.stack().getTopFrame();
        if (frame == nullptr || !frame->isWithinBounds(index.value())) DISPATCH_FAIL();

        std::optional<Value> value = vm.pop();
        if (!value.has_value()) DISPATCH_FAIL();

       (*frame)[index.value()] = value.value();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(JMP) {
        std::optional<i16> branch = code.fetchI16();
        if (!branch.has_value()) DISPATCH_FAIL();

        code.skip(branch.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(JZ) {
        std::optional<i16> branch = code.fetchI16();
        if (!branch.has_value()) DISPATCH_FAIL();

        if (!vm.acc().boolean()) code.skip(branch.value());

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(JNZ) {
        std::optional<i16> branch = code.fetchI16();
        if (!branch.has_value()) DISPATCH_FAIL();

        if (vm.acc().boolean()) code.skip(branch.value());

        DISPATCH_SUCCEED();
    }

    void InitDispatchers(const VMConfig& config, DispatchTable& dispatchTable, DispatchTableExt& dispatchTableExt) {
        REGISTER_DISPATCH(dispatchTable, NOP);
        REGISTER_DISPATCH(dispatchTable, HLT);
        REGISTER_DISPATCH(dispatchTable, TRAP);
        REGISTER_DISPATCH(dispatchTable, TRAP_IF_ZERO);
        REGISTER_DISPATCH(dispatchTable, TRAP_IF_NOT_ZERO);
        REGISTER_DISPATCH(dispatchTable, BRK);
        REGISTER_DISPATCH(dispatchTable, ADD);
        REGISTER_DISPATCH(dispatchTable, SUB);
        REGISTER_DISPATCH(dispatchTable, MUL);
        REGISTER_DISPATCH(dispatchTable, DIV);
        REGISTER_DISPATCH(dispatchTable, MOD);
        REGISTER_DISPATCH(dispatchTable, AND);
        REGISTER_DISPATCH(dispatchTable, OR);
        REGISTER_DISPATCH(dispatchTable, XOR);
        REGISTER_DISPATCH(dispatchTable, SHL);
        REGISTER_DISPATCH(dispatchTable, SHR);
        REGISTER_DISPATCH(dispatchTable, NEG);
        REGISTER_DISPATCH(dispatchTable, NOT);
        REGISTER_DISPATCH(dispatchTable, ADD2);
        REGISTER_DISPATCH(dispatchTable, SUB2);
        REGISTER_DISPATCH(dispatchTable, MUL2);
        REGISTER_DISPATCH(dispatchTable, DIV2);
        REGISTER_DISPATCH(dispatchTable, MOD2);
        REGISTER_DISPATCH(dispatchTable, AND2);
        REGISTER_DISPATCH(dispatchTable, OR2);
        REGISTER_DISPATCH(dispatchTable, XOR2);
        REGISTER_DISPATCH(dispatchTable, SHL2);
        REGISTER_DISPATCH(dispatchTable, SHR2);
        REGISTER_DISPATCH(dispatchTable, ADD_ST);
        REGISTER_DISPATCH(dispatchTable, SUB_ST);
        REGISTER_DISPATCH(dispatchTable, MUL_ST);
        REGISTER_DISPATCH(dispatchTable, DIV_ST);
        REGISTER_DISPATCH(dispatchTable, MOD_ST);
        REGISTER_DISPATCH(dispatchTable, AND_ST);
        REGISTER_DISPATCH(dispatchTable, OR_ST);
        REGISTER_DISPATCH(dispatchTable, XOR_ST);
        REGISTER_DISPATCH(dispatchTable, SHL_ST);
        REGISTER_DISPATCH(dispatchTable, SHR_ST);
        REGISTER_DISPATCH(dispatchTable, NEG_ST);
        REGISTER_DISPATCH(dispatchTable, NOT_ST);
        REGISTER_DISPATCH(dispatchTable, ADD_IMM);
        REGISTER_DISPATCH(dispatchTable, SUB_IMM);
        REGISTER_DISPATCH(dispatchTable, MUL_IMM);
        REGISTER_DISPATCH(dispatchTable, DIV_IMM);
        REGISTER_DISPATCH(dispatchTable, MOD_IMM);
        REGISTER_DISPATCH(dispatchTable, AND_IMM);
        REGISTER_DISPATCH(dispatchTable, OR_IMM);
        REGISTER_DISPATCH(dispatchTable, XOR_IMM);
        REGISTER_DISPATCH(dispatchTable, SHL_IMM);
        REGISTER_DISPATCH(dispatchTable, SHR_IMM);
        REGISTER_DISPATCH(dispatchTable, ADD_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, SUB_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, MUL_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, DIV_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, MOD_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, AND_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, OR_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, XOR_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, SHL_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, SHR_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, FADD);
        REGISTER_DISPATCH(dispatchTable, FSUB);
        REGISTER_DISPATCH(dispatchTable, FMUL);
        REGISTER_DISPATCH(dispatchTable, FDIV);
        REGISTER_DISPATCH(dispatchTable, FADD2);
        REGISTER_DISPATCH(dispatchTable, FSUB2);
        REGISTER_DISPATCH(dispatchTable, FMUL2);
        REGISTER_DISPATCH(dispatchTable, FDIV2);
        REGISTER_DISPATCH(dispatchTable, FADD_ST);
        REGISTER_DISPATCH(dispatchTable, FSUB_ST);
        REGISTER_DISPATCH(dispatchTable, FMUL_ST);
        REGISTER_DISPATCH(dispatchTable, FDIV_ST);
        REGISTER_DISPATCH(dispatchTable, FNEG);
        REGISTER_DISPATCH(dispatchTable, FADD_IMM);
        REGISTER_DISPATCH(dispatchTable, FSUB_IMM);
        REGISTER_DISPATCH(dispatchTable, FMUL_IMM);
        REGISTER_DISPATCH(dispatchTable, FDIV_IMM);
        REGISTER_DISPATCH(dispatchTable, FADD_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, FSUB_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, FMUL_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, FDIV_IMM_ST);
        REGISTER_DISPATCH(dispatchTable, CMP_EQ);
        REGISTER_DISPATCH(dispatchTable, CMP_NE);
        REGISTER_DISPATCH(dispatchTable, CMP_LT);
        REGISTER_DISPATCH(dispatchTable, CMP_GT);
        REGISTER_DISPATCH(dispatchTable, CMP_LTE);
        REGISTER_DISPATCH(dispatchTable, CMP_GTE);
        REGISTER_DISPATCH(dispatchTable, FCMP_EQ);
        REGISTER_DISPATCH(dispatchTable, FCMP_NE);
        REGISTER_DISPATCH(dispatchTable, FCMP_LT);
        REGISTER_DISPATCH(dispatchTable, FCMP_GT);
        REGISTER_DISPATCH(dispatchTable, FCMP_LTE);
        REGISTER_DISPATCH(dispatchTable, FCMP_GTE);
        REGISTER_DISPATCH(dispatchTable, CMP_EQ0);
        REGISTER_DISPATCH(dispatchTable, CMP_NE0);
        REGISTER_DISPATCH(dispatchTable, CMP_LT0);
        REGISTER_DISPATCH(dispatchTable, CMP_GT0);
        REGISTER_DISPATCH(dispatchTable, CMP_LTE0);
        REGISTER_DISPATCH(dispatchTable, CMP_GTE0);
        REGISTER_DISPATCH(dispatchTable, FCMP_EQ0);
        REGISTER_DISPATCH(dispatchTable, FCMP_NE0);
        REGISTER_DISPATCH(dispatchTable, FCMP_LT0);
        REGISTER_DISPATCH(dispatchTable, FCMP_GT0);
        REGISTER_DISPATCH(dispatchTable, FCMP_LTE0);
        REGISTER_DISPATCH(dispatchTable, FCMP_GTE0);
        REGISTER_DISPATCH(dispatchTable, PUSH_ACC);
        REGISTER_DISPATCH(dispatchTable, PUSH_SP);
        REGISTER_DISPATCH(dispatchTable, POP_ACC);
        REGISTER_DISPATCH(dispatchTable, POP_SP);
        REGISTER_DISPATCH(dispatchTable, POP_DISCARD);
        REGISTER_DISPATCH(dispatchTable, CONST);
        REGISTER_DISPATCH(dispatchTable, CONST32);
        REGISTER_DISPATCH(dispatchTable, CONST64);
        REGISTER_DISPATCH(dispatchTable, CONST_ST);
        REGISTER_DISPATCH(dispatchTable, CONST32_ST);
        REGISTER_DISPATCH(dispatchTable, CONST64_ST);
        REGISTER_DISPATCH(dispatchTable, LOAD);
        REGISTER_DISPATCH(dispatchTable, LOAD_ST);
        REGISTER_DISPATCH(dispatchTable, STORE);
        REGISTER_DISPATCH(dispatchTable, STORE_ST);
        REGISTER_DISPATCH(dispatchTable, JMP);
        REGISTER_DISPATCH(dispatchTable, JZ);
        REGISTER_DISPATCH(dispatchTable, JNZ);
    }
}
