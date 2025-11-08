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

    u32 Interpreter::getActiveModule() const {
        return mActiveModule;
    }

    void Interpreter::execute(VM& vm, u32 module, BytecodeReader bytecode) {
        mActiveModule = module;
        while (true) {
            if (vm.hasExited()) {
                return;
            }

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

            DispatchErr err = dispatch(vm, bytecode);
            if (err == DISPATCH_SUCCESS) {
                continue;
            } else if (err == DISPATCH_RETURN) {
                return;
            } else {
                vm.exit(-1);
                return;
            }
        }
    }
}
