// Copyright 2025 JesusTouchMe

#ifndef BIBBLEVM_CORE_DISPATCH_H
#define BIBBLEVM_CORE_DISPATCH_H 1

#include "BibbleVM/core/bytecode/bytecode_stream.h"

#include "BibbleVM/config.h"

#include <array>

namespace bibble {
    class VM;
    using DispatchFn = bool(*)(VM&, BytecodeStream&);
    using DispatchTable = std::array<DispatchFn, 256>;
    using DispatchTableExt = std::array<DispatchFn, 65536>;

    // a world shattering 514 kilobytes of memory per active full dispatcher table
    // TODO: optimize the above

    void InitDispatchers(const VMConfig& config, DispatchTable& dispatchTable, DispatchTableExt& dispatchTableExt);
}

#endif // BIBBLEVM_CORE_DISPATCH_H
