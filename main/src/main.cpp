// Copyright 2025 JesusTouchMe

#include <BibbleVM/core/vm.h>

#include <iostream>

int main() {
    auto vm = bibble::CreateVM();

    vm->stack().pushFrame(1);

    bibble::u8 bytecodeArr[] = {
        // data section

        // main
        0xFF, 0xFF, 0xFF, 0xFF, // module
        0x00, 0x00, 0x00, 0x00, // address (known)

        // test
        0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x10,



        // code section

        // main
        0x85, 0x22, // CONST 34
        0x32, 0x00, 0x00, 0x00, 0x23, // ADD_IMM 35
        0x80, // PUSH_ACC
        0x9A, 0x00, 0x00, 0x00, 0x08, 0x01, // CALL 8, 1
        0x01, 0x00, // HLT 0

        // test
        0x8B, 0x00, 0x00, // LOAD 0
        0x02, 0x00, // TRAP 0
        0x9F, // RET
    };

    std::unique_ptr<bibble::u8[]> bytecode = std::make_unique<bibble::u8[]>(sizeof(bytecodeArr));
    std::memcpy(bytecode.get(), bytecodeArr, sizeof(bytecodeArr));

    bibble::Section data({ &bytecode[0], 16 });
    bibble::Section strtab({ &bytecode[16], 0 });
    bibble::Section code({ &bytecode[16], 22 });

    bibble::u32 moduleH = vm->addModule(std::make_unique<bibble::Module>(std::move(bytecode), bibble::DataSection(data), bibble::StrtabSection(strtab), bibble::CodeSection(code)));
    if (vm->hasExited()) {
        std::cerr << "Failed to add module to VM\n";
        return vm->getExitCode();
    }

    bibble::Module* module = vm->getModule(moduleH);
    if (module == nullptr) {
        std::cerr << "VM internal state broken\n";
        return 1;
    }

    std::optional<bibble::BytecodeReader> mainEntry = module->code().getBytecodeReader(0);
    if (!mainEntry.has_value()) {
        std::cerr << "Main entry out of bounds\n";
        return 1;
    }

    bibble::CallableTarget mainTarget = {moduleH, mainEntry.value()};

    vm->addFunction(std::make_unique<bibble::Function>(moduleH, "main", mainTarget));
    if (vm->hasExited()) {
        std::cerr << "Failed to add main() to VM\n";
        return vm->getExitCode();
    }

    bibble::Function* mainFunction = vm->getFunction("main");
    if (mainFunction == nullptr) {
        std::cerr << "VM internal state broken\n";
        return 1;
    }

    CallableTrampoline(mainFunction->target(), *vm);

    if (!vm->hasExited()) return 100;

    return vm->getExitCode();
}