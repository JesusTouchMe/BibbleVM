// Copyright 2025 JesusTouchMe

#include <BibbleVM/core/vm.h>

int main() {
    auto vm = bibble::CreateVM();

    vm->stack().pushFrame(0);

    bibble::u8 bytecode[] = {
        0x85, 0x22, // CONST 34
        0x32, 0x00, 0x00, 0x00, 0x23, // ADD_IMM 35
        0x02, 0x00, // TRAP 0
        0x97, 0x00, 0x02, // JMP 2
        0x01, 0x45, // HLT 69
        0x01, 0x00, // HLT 0
    };

    bibble::BytecodeReader stream(bytecode);

    vm->execute(stream);

    if (!vm->hasExited()) return 100;

    return vm->getExitCode();
}