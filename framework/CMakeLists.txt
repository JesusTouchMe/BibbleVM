cmake_minimum_required(VERSION 3.26)

set(SOURCES
    src/core/bytecode/bytecode_stream.cpp
    src/core/exec/dispatch.cpp
    src/core/vm.cpp
)

set(HEADERS
    include/BibbleVM/core/bytecode/opcodes.h
    include/BibbleVM/core/vm.h
    include/BibbleVM/core/bytecode/bytecode_stream.h
    include/BibbleVM/core/exec/dispatch.h
    include/BibbleVM/config.h
    include/BibbleVM/core/value/value.h
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