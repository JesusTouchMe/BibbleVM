cmake_minimum_required(VERSION 3.26)

set(SOURCES
        src/core/bytecode/bytecode_reader.cpp
    src/core/exec/dispatch.cpp
    src/core/vm.cpp
    src/core/stack/stack.cpp
    src/core/exec/interpreter.cpp
        src/core/bytecode/section.cpp
    src/core/bytecode/data_section.cpp
    src/core/module/module.cpp
    src/core/call/function.cpp
    src/core/bytecode/code_section.cpp
    src/core/bytecode/strtab_section.cpp
    src/core/call/callable_target.cpp
)

set(HEADERS
    include/BibbleVM/core/bytecode/opcodes.h
    include/BibbleVM/core/vm.h
        include/BibbleVM/core/bytecode/bytecode_reader.h
    include/BibbleVM/core/exec/dispatch.h
    include/BibbleVM/config.h
    include/BibbleVM/core/value/value.h
    include/BibbleVM/core/stack/stack.h
    include/BibbleVM/core/exec/interpreter.h
        include/BibbleVM/core/bytecode/section.h
    include/BibbleVM/core/bytecode/data_section.h
    include/BibbleVM/core/bytecode/strtab_section.h
    include/BibbleVM/core/call/callable_target.h
    include/BibbleVM/core/bytecode/code_section.h
    include/BibbleVM/core/module/module.h
    include/BibbleVM/core/call/function.h
    include/BibbleVM/util/string.h
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(BibbleVM-framework ${SOURCES} ${HEADERS})
add_library(Bibble.VM ALIAS BibbleVM-framework)

target_include_directories(BibbleVM-framework
    PUBLIC
        include
)

if(WIN32)
    target_compile_definitions(BibbleVM-framework PUBLIC PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(BibbleVM-framework PUBLIC PLATFORM_MACOS)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(BibbleVM-framework PUBLIC PLATFORM_LINUX)
endif()

target_compile_features(BibbleVM-framework PUBLIC cxx_std_20)