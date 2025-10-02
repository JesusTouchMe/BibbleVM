// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_INTERPRETER_H
#define BIBBLEVM_CORE_INTERPRETER_H 1

#include "BibbleVM/core/bytecode/bytecode_stream.h"

#include "BibbleVM/config.h"

#include <array>

namespace bibble {
    class VM;
    using DispatchFn = bool(*)(VM&, BytecodeStream&);
    using DispatchTable = std::array<DispatchFn, 256>;
    using DispatchTableExt = std::array<DispatchTable, 65536>;

    // a world shattering 514 kilobytes of memory per active full dispatcher

    void InitDispatchers(VMConfig& config, DispatchTable& dispatchTable, DispatchTableExt& dispatchTableExt);
}

#endif // BIBBLEVM_CORE_INTERPRETER_H
