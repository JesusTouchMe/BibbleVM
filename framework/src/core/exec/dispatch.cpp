// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/exec/dispatch.h"

#include "BibbleVM/core/vm.h"

#define DEFINE_DISPATCH(opcode) bool Dispatch_##opcode(VM& vm, BytecodeStream& code)
#define REGISTER_DISPATCH(table, opcode) table[static_cast<size_t>(ByteOpcode::opcode)] = Dispatch_##opcode
#define REGISTER_DISPATCH_EXT(table, opcode) table[static_cast<size_t>(ExtendedOpcode::opcode)] = Dispatch_##opcode

#define DISPATCH_SUCCESS true
#define DISPATCH_FAILURE false
#define DISPATCH_SUCCEED() return DISPATCH_SUCCESS
#define DISPATCH_FAIL() return DISPATCH_FAILURE

namespace bibble {
    DEFINE_DISPATCH(NOP) {
        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(HLT) { // TODO: if the vm is embedded and shouldn't randomly exit the host, exit only the VM in a special way
        std::optional<i8> exitCode = code.fetchI8();
        if (!exitCode.has_value()) DISPATCH_FAIL();

        std::exit(exitCode.value());
    }

    DEFINE_DISPATCH(TRAP) {
        std::optional<u8> trapCode = code.fetchU8();
        if (!trapCode.has_value()) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(TRAP_IF_ZERO) {
        std::optional<u8> trapCode = code.fetchU8();
        if (!trapCode.has_value()) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(TRAP_IF_NOT_ZERO) {
        std::optional<u8> trapCode = code.fetchU8();
        if (!trapCode.has_value()) DISPATCH_FAIL();

        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(BRK) {
        DISPATCH_SUCCEED();
    }

    DEFINE_DISPATCH(ADD) {
    }

    DEFINE_DISPATCH(SUB) {
    }

    DEFINE_DISPATCH(MUL) {
    }

    DEFINE_DISPATCH(DIV) {
    }

    DEFINE_DISPATCH(MOD) {
    }

    DEFINE_DISPATCH(AND) {
    }

    DEFINE_DISPATCH(OR) {
    }

    DEFINE_DISPATCH(XOR) {
    }

    DEFINE_DISPATCH(SHL) {
    }

    DEFINE_DISPATCH(SHR) {
    }

    DEFINE_DISPATCH(NEG) {
    }

    DEFINE_DISPATCH(NOT) {
    }

    DEFINE_DISPATCH(ADD2) {
    }

    DEFINE_DISPATCH(SUB2) {
    }

    DEFINE_DISPATCH(MUL2) {
    }

    DEFINE_DISPATCH(DIV2) {
    }

    DEFINE_DISPATCH(MOD2) {
    }

    DEFINE_DISPATCH(AND2) {
    }

    DEFINE_DISPATCH(OR2) {
    }

    DEFINE_DISPATCH(XOR2) {
    }

    DEFINE_DISPATCH(SHL2) {
    }

    DEFINE_DISPATCH(SHR2) {
    }

    DEFINE_DISPATCH(ADD_ST) {
    }

    DEFINE_DISPATCH(SUB_ST) {
    }

    DEFINE_DISPATCH(MUL_ST) {
    }

    DEFINE_DISPATCH(DIV_ST) {
    }

    DEFINE_DISPATCH(MOD_ST) {
    }

    DEFINE_DISPATCH(AND_ST) {
    }

    DEFINE_DISPATCH(OR_ST) {
    }

    DEFINE_DISPATCH(XOR_ST) {
    }

    DEFINE_DISPATCH(SHL_ST) {
    }

    DEFINE_DISPATCH(SHR_ST) {
    }

    DEFINE_DISPATCH(NEG_ST) {
    }

    DEFINE_DISPATCH(NOT_ST) {
    }

    DEFINE_DISPATCH(ADD_IMM) {
    }

    DEFINE_DISPATCH(SUB_IMM) {
    }

    DEFINE_DISPATCH(MUL_IMM) {
    }

    DEFINE_DISPATCH(DIV_IMM) {
    }

    DEFINE_DISPATCH(MOD_IMM) {
    }

    DEFINE_DISPATCH(AND_IMM) {
    }

    DEFINE_DISPATCH(OR_IMM) {
    }

    DEFINE_DISPATCH(XOR_IMM) {
    }

    DEFINE_DISPATCH(SHL_IMM) {
    }

    DEFINE_DISPATCH(SHR_IMM) {
    }

    DEFINE_DISPATCH(ADD_IMM_ST) {
    }

    DEFINE_DISPATCH(SUB_IMM_ST) {
    }

    DEFINE_DISPATCH(MUL_IMM_ST) {
    }

    DEFINE_DISPATCH(DIV_IMM_ST) {
    }

    DEFINE_DISPATCH(MOD_IMM_ST) {
    }

    DEFINE_DISPATCH(AND_IMM_ST) {
    }

    DEFINE_DISPATCH(OR_IMM_ST) {
    }

    DEFINE_DISPATCH(XOR_IMM_ST) {
    }

    DEFINE_DISPATCH(SHL_IMM_ST) {
    }

    DEFINE_DISPATCH(SHR_IMM_ST) {
    }

    DEFINE_DISPATCH(FADD) {
    }

    DEFINE_DISPATCH(FSUB) {
    }

    DEFINE_DISPATCH(FMUL) {
    }

    DEFINE_DISPATCH(FDIV) {
    }

    DEFINE_DISPATCH(FADD2) {
    }

    DEFINE_DISPATCH(FSUB2) {
    }

    DEFINE_DISPATCH(FMUL2) {
    }

    DEFINE_DISPATCH(FDIV2) {
    }

    DEFINE_DISPATCH(FADD_ST) {
    }

    DEFINE_DISPATCH(FSUB_ST) {
    }

    DEFINE_DISPATCH(FMUL_ST) {
    }

    DEFINE_DISPATCH(FDIV_ST) {
    }

    DEFINE_DISPATCH(FNEG) {
    }

    DEFINE_DISPATCH(FADD_IMM) {
    }

    DEFINE_DISPATCH(FSUB_IMM) {
    }

    DEFINE_DISPATCH(FMUL_IMM) {
    }

    DEFINE_DISPATCH(FDIV_IMM) {
    }

    DEFINE_DISPATCH(FADD_IMM_ST) {
    }

    DEFINE_DISPATCH(FSUB_IMM_ST) {
    }

    DEFINE_DISPATCH(FMUL_IMM_ST) {
    }

    DEFINE_DISPATCH(FDIV_IMM_ST) {
    }

    DEFINE_DISPATCH(PUSH_ACC) {
    }

    DEFINE_DISPATCH(PUSH_SP) {
    }

    DEFINE_DISPATCH(POP_ACC) {
    }

    DEFINE_DISPATCH(POP_SP) {
    }

    DEFINE_DISPATCH(POP_DISCARD) {
    }

    DEFINE_DISPATCH(CONST) {
    }

    DEFINE_DISPATCH(CONST32) {
    }

    DEFINE_DISPATCH(CONST64) {
    }

    DEFINE_DISPATCH(CONST_ST) {
    }

    DEFINE_DISPATCH(CONST32_ST) {
    }

    DEFINE_DISPATCH(CONST64_ST) {
    }

    DEFINE_DISPATCH(LOAD) {
    }

    DEFINE_DISPATCH(LOAD_ST) {
    }

    DEFINE_DISPATCH(STORE) {
    }

    DEFINE_DISPATCH(STORE_ST) {
    }

    void InitDispatchers(VMConfig& config, DispatchTable& dispatchTable, DispatchTableExt& dispatchTableExt) {
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
    }
}
