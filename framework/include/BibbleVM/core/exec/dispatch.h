// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_DISPATCH_H
#define BIBBLEVM_CORE_DISPATCH_H 1

#include "BibbleVM/core/bytecode/bytecode_reader.h"

#include "BibbleVM/config.h"

#include <array>

#define DISPATCH_RETURN (-1)
#define DISPATCH_SUCCESS 0
#define DISPATCH_ERROR 1

namespace bibble {
    class VM;
    using DispatchErr = int; // enum?
    using DispatchFn = DispatchErr(*)(VM&, BytecodeReader&);
    using DispatchTable = std::array<DispatchFn, 256>;
    using DispatchTableExt = std::array<DispatchFn, 65536>;

    // a world shattering 514 kilobytes of memory per active full dispatcher table
    // TODO: optimize the above

    void InitDispatchers(const VMConfig& config, DispatchTable& dispatchTable, DispatchTableExt& dispatchTableExt);
}

#endif // BIBBLEVM_CORE_DISPATCH_H
