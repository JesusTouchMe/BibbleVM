// Copyright 2025 JesusTouchMe

#include "BibbleVM/core/exec/interpreter.h"

#include "BibbleVM/core/vm.h"

namespace bibble {
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    Interpreter::Interpreter(const VMConfig& config) {
        InitDispatchers(config, mDispatchTable, mDispatchTableExt);
    }

    void Interpreter::execute(VM& vm, BytecodeStream bytecode) {
        while (true) {
            std::optional<std::variant<ByteOpcode, ExtendedOpcode>> opcode = bytecode.fetchOpcode();
            if (!opcode.has_value()) {
                vm.exit(-1);
                return;
            }

            DispatchFn dispatch = nullptr;

            std::visit(overloaded {
                        [this, &dispatch](ByteOpcode arg) { dispatch = mDispatchTable[static_cast<size_t>(arg)]; },
                        [this, &dispatch](ExtendedOpcode arg) { dispatch = mDispatchTableExt[static_cast<size_t>(arg)]; }
                    }, opcode.value());

            if (dispatch == nullptr) {
                vm.exit(-1);
                return;
            }

            if (!dispatch(vm, bytecode)) {
                vm.exit(-1);
                return;
            }

            if (vm.hasExited()) {
                return;
            }
        }
    }
}
