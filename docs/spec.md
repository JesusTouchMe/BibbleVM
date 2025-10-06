BibbleVM Instruction Set Architecture v0.1-predev
=

# 1. Overview

## 1.1 Execution Model
BibbleVM is a stack-based machine with three dedicated registers:
- **Accumulator (ACC)**: Holds the result of most instructions. Subsequent instructions can consume ACC directly or move it to the stack.
- **Stack Pointer (SP)**: Holds the index above the top of the current stack frame as an index from its base. SP is implicitly managed by push/pop instructions, but may be manually modified if required.
- **Program Counter (PC)**: Holds the address of the next instruction to execute. PC is internal-only and cannot be accessed directly by bytecode. Control flow instructions modify PC implicitly.
  - PC isn't required to be a register in VM implementations and is mainly just used to talk about execution.

Execution proceeds sequentially through instructions unless a **control flow instruction** modifies PC.

### 1.1.1 Stack Frames
- Each function allocates a fixed-size frame defined in its metadata.
- Frames consist of 8-byte slots.
- Slots may be accessed by absolute index or using SP via push/pop instructions

---

## 1.2 Value Representation
- **Slot size**: 8 bytes.
- **Valid contents**: Integers, floating-point values, pointers/handles or object references.
- Slots are untyped; the programmer or compiler is responsible for type correctness.

---

## 1.3 Memory Model

### 1.3.1 Object Management
- **Unique Object**: One owning reference; non-owners hold weak references.
- **Shared Object**: Atomically Reference Counted (ARC); ownership explicitly modified by instructions.
- **Weak Object**: Non-owning reference to a unique or shared object. The VM automatically validates it on use, but validity can also be checked manually and is recommended.

### 1.3.2 Manual Memory
BibbleVM exposes manual primitives for low-level memory management:
- **Memory blocks** can be allocated on the heap or stack. Stack allocations are always aligned to the nearest stack slot.
- **Type-specific operations** allow reading and writing values of 1, 2, 4, or 8 bytes.

---

## 1.4 Global State
BibbleVM maintains a single global state:
- **Functions and Classes**: Globally visible, referenced by optionally mangled names (e.g. `ns::function(II)I` or `ns::MyClass`). Name mangling is handled by the programmer or compiler.
  - Because there are no modules, it is recommended that the programmer or compiler always adds what would be the module name in a function or class name, even without full mangling.
- **Global Data Section**: Contains a contiguous region of raw data. Values are accessed by a byte offset.
  - Global data is untyped and unchecked access could result in garbage data or even crashes.

---

## 1.5 Function Model
- Each function specifies its required stack frame size.
- Calls allocate a new frame, execute, and restore the caller's frame on return.
- Return values are always written to **ACC**. 

---

# 2. Instruction Format

## 2.1 General Encoding Rules
- **Opcode size**: 1 byte normally; extended opcodes use 3 bytes.
  - **Extended opcodes**: If the first byte is `0xFF`, the next two bytes form the extended opcode.
- **Operand types**: May include:
  - Immediate types (integer, floating-point).
  - Stack slot indexes.
  - Data section indexes.
  - Offsets for jumps and memory access.
- **Endianness**: Big-endian encoding for multi-byte operands.
- **Alignment**: No.

---

## 2.2 Instruction Categories
Instructions can be grouped by purpose:
- **Arithmetic / Logical**: E.g. ADD, SUB, MUL, DIV, AND, OR, XOR.
- **Stack / Register Manipulation**: E.g PUSH, POP, LOAD, STORE, CONST.
- **Control Flow**: E.g JMP, JZ, JNZ, CALL, RET.
- **Memory / Object Management**: e.g ALLOCATE, FREE, READ, WRITE, NEWUNIQUE, NEWSHARED.
- **Miscellaneous**: E.g NOP, TRAP, HLT.
- **Debug**: E.g BRK.

---

# 3. The `bbx` File Format
Each `bbx` file contains the definitions of symbols, functions, classes and internal data used by the code in the executable or by the linker.

A `bbx` file consists of a stream of bytes. Multibyte data items are always stored in big-endian order.

This chapter presents the `bbx` file format using pseudostructures written in a C++-like struct notation. The contents of the structures describing the `bbx` file format are referred to as _items_. Successive items are stored in the `bbx` file sequentially, without padding or alignment.

This chapter defines its own set of data types representing file data: u8, u16, u32 and u64 represent an unsigned one-, two-, four- or eight-byte quantity respectively. An i in place of the u means that the type is signed.

## 3.1 The `BBXFile` Structure
A `bbx` file contains one of these:
```
BBXFile {
  u32 magic;
  u16 version;
  DataSection data;
  SymbolSection symbols;
  Section strtab;
  CodeSection code;
  u8 extra_section_count;
  Section extra_sections[extra_section_count];
}
```
The items of the `BBXFile` struct are as follows:
* **magic**: A magic number identifying the `bbx` file format. It has the value 0xB1BB1E69.
* **version**: The bytecode version for VM legacy support.
* **data**: The section storing all data ranging from strings to global variables ([3.2.2](#322-datasection-structure)).
* **symbols**: The section storing all symbols to define functions and classes ([3.2.3](#323-symbolsection-structure)).
* **strtab**: Section for holding all variable-length strings ([3.2.4](#324-strtabsection-structure)). Usually for fixed length strings of 8 bytes or more, making the firts 4 bytes `@STR` and the next 4 an index into strtab will result in a variable-length strtab string being used instead of a fixed-length.
* **code**: The section storing all bytecode instructions ([3.2.5](#325-codesection-structure)).
* **extra_section_count**: The amount of extra sections (nonstandard or optional) present in the file.
* **extra_sections[]**: A table of structures ([3.2.1](#321-section-structure), [3.2.6](#326-extra-section-structures)).

## 3.2 The `Section` Structures
This chapter will define a base `Section` struct as well as several derivatives.

A struct deriving from another struct(s) will place the items of the derived struct(s) before its own items in order of derive list.

### 3.2.1 `Section` Structure
```
Section {
  u8 name[8];
  u32 size;
}
```

### 3.2.2 `DataSection` Structure

### 3.2.3 `SymbolSection` Structure

### 3.2.4 `StrtabSection` Structure

### 3.2.5 `CodeSection` Structure

### 3.2.6 Extra Section Structures

---

# 4. Instruction Set

## 4.1 Instruction Notation
The following conventions are used throughout the next section to describe **stack effect**, **ACC effect** and operand usage.

### 4.1.1 Terminology
This section defines some common terms seen in the instruction set:
- **integer**: A numeric value represented as a signed integer. Its size is either explicitly stated where it's used or should be assumed to be the same size as a stack slot (8 bytes).
- **float**: A numeric value represented in IEEE 754 single- or double-precision format. The size of either explicitly stated where it's used or should be assumed to be the same size as a stack slot (8 bytes or double-precision).

### 4.1.2 Register Operands
See [Section 1.1](#11-execution-model).
- **ACC**: The accumulator register.
- **SP**: The stack pointer register.
- **PC**: The program counter register.

### 4.1.3 Stack Operands
- **S0**: Top of the stack as indicated by **SP**.
- **S1**: Next value down from the top (second from top).
- **S2, S3, Sn...**: Subsequent values down the stack.
- **S[n]**: Stack slot at `n` starting at index 0 for the bottom.
- `f` suffix means the value addressed is treated as a floating-point value.

These values represent stack slots BEFORE stack effect is applied.

### 4.1.4 Stack Effects
- **Pop n**: Removes the top `n` values from the stack (`S0..S(n-1)`) and consumes them accordingly.
- **Push n**: Pushes `n` values onto the stack.
- **-**: Instruction does not modify the stack.

### 4.1.5 ACC Effects
- **ACC ← expr**: The accumulator is updated with the result of `expr`.
- Use stack operands in `expr` to indicate used stack values.
- **-**: Instruction does not modify ACC.

### 4.1.6 Operand Column
- Lists instruction-specific operands encoded directly into the bytecode, such as stack indices, immediate values, or offsets.
- Each operand is written in the form `(i|f)n operand-name`, where:
  - `i` indicates an **integer** value.
  - `f` indicates a **floating-point** value.
  - `n` specifies the **size of the operand in bits**.
  - `operand-name` is the **name used in the instruction’s effect column**.

- Operands appear directly after the opcode byte(s) in bytecode in the same order as they were written.
- Use `-` if there are no operands.

### 4.1.7 Notes Column
- Additional remarks or implementation details that don't fit in description.
- Use `-` if there are no notes.

### 4.1.8 Encoding Column
- Shows how instructions should be encoded in bytecode.
- `X` means any value, usually used for operands.

## 4.2 Instructions

|     Mnemonic     | Opcode (Hex) |   Operands   | Stack Effect  |      ACC Effect      | Description                                                                                                                                                                           | Notes                     | Encoding                   |
|:----------------:|:------------:|:------------:|:-------------:|:--------------------:|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------|----------------------------|
|       NOP        |     0x00     |      -       |       -       |          -           | Performs no operation.                                                                                                                                                                | -                         | 00                         |
|       HLT        |     0x01     | i8 exit-code |       -       |          -           | Stops VM execution immediately and exits with the provided `exit-code`.                                                                                                               | -                         | 01 XX                      |
|       TRAP       |     0x02     |   i8 code    |       -       |          -           | Executes a VM-defined trap routine. The 8-bit `code` specifies the trap type. Can trigger runtime errors, host calls or debug hooks.                                                  | Code values defined below | 02 XX                      |
|   TRAP_IF_ZERO   |     0x03     |   i8 code    |       -       |          -           | Executed a VM-defined trap routine if the value in accumulator is equal to zero. The 8-bit `code` specifies the trap type. Can trigger runtime errors, host calls or debug hooks.     | Code values defined below | 03 XX                      |
| TRAP_IF_NOT_ZERO |     0x04     |   i8 code    |       -       |          -           | Executed a VM-defined trap routine if the value in accumulator is not equal to zero. The 8-bit `code` specifies the trap type. Can trigger runtime errors, host calls or debug hooks. | Code values defined below | 04 XX                      |
|       BRK        |     0x05     |      -       |       -       |          -           | Triggers a breakpoint with a debugger attached; ignored normally.                                                                                                                     | -                         | 05                         |
|       ADD        |     0x10     |      -       |     Pop 1     |    ACC ← ACC + S0    | Pops an integer and adds it to ACC.                                                                                                                                                   | -                         | 10                         |
|       SUB        |     0x11     |      -       |     Pop 1     |    ACC ← ACC - S0    | Pops an integer and subtracts it from ACC.                                                                                                                                            | -                         | 11                         |
|       MUL        |     0x12     |      -       |     Pop 1     |    ACC ← ACC * S0    | Pops an integer and multiplies ACC by it.                                                                                                                                             | -                         | 12                         |
|       DIV        |     0x13     |      -       |     Pop 1     |    ACC ← ACC / S0    | Pops an integer and divides ACC by it.                                                                                                                                                | -                         | 13                         |
|       MOD        |     0x14     |      -       |     Pop 1     |    ACC ← ACC % S0    | Pops an integer and computes ACC mod S0.                                                                                                                                              | -                         | 14                         |
|       AND        |     0x15     |      -       |     Pop 1     |    ACC ← ACC & S0    | Pops an integer and bitwise ANDs with ACC.                                                                                                                                            | -                         | 15                         |
|        OR        |     0x16     |      -       |     Pop 1     |   ACC ← ACC \| S0    | Pops an integer and bitwise OR with ACC.                                                                                                                                              | -                         | 16                         |
|       XOR        |     0x17     |      -       |     Pop 1     |    ACC ← ACC ^ S0    | Pops an integer and bitwise XOR with ACC.                                                                                                                                             | -                         | 17                         |
|       SHL        |     0x18     |      -       |     Pop 1     |   ACC ← ACC << S0    | Pops an integer and shifts ACC left.                                                                                                                                                  | -                         | 18                         |
|       SHR        |     0x19     |      -       |     Pop 1     |   ACC ← ACC >> S0    | Pops an integer and shifts ACC right.                                                                                                                                                 | -                         | 19                         |
|       NEG        |     0x1A     |      -       |       -       |      ACC ← -ACC      | Negates ACC.                                                                                                                                                                          | -                         | 1A                         |
|       NOT        |     0x1B     |      -       |       -       |      ACC ← ~ACC      | Bitwise NOT of ACC.                                                                                                                                                                   | -                         | 1B                         |
|       ADD2       |     0x1C     |      -       |     Pop 2     |    ACC ← S1 + S0     | Pops two integers, adds into ACC.                                                                                                                                                     | -                         | 1C                         |
|       SUB2       |     0x1D     |      -       |     Pop 2     |    ACC ← S1 - S0     | Pops two integers, subtracts top from second into ACC.                                                                                                                                | -                         | 1D                         |
|       MUL2       |     0x1E     |      -       |     Pop 2     |    ACC ← S1 * S0     | Pops two integers, multiplies into ACC.                                                                                                                                               | -                         | 1E                         |
|       DIV2       |     0x1F     |      -       |     Pop 2     |    ACC ← S1 / S0     | Pops two integers, divides second by top into ACC.                                                                                                                                    | -                         | 1F                         |
|       MOD2       |     0x20     |      -       |     Pop 2     |    ACC ← S1 % S0     | Pops two integers, computes modulo into ACC.                                                                                                                                          | -                         | 20                         |
|       AND2       |     0x21     |      -       |     Pop 2     |    ACC ← S1 & S0     | Pops two integers, bitwise AND into ACC.                                                                                                                                              | -                         | 21                         |
|       OR2        |     0x22     |      -       |     Pop 2     |    ACC ← S1 \| S0    | Pops two integers, bitwise OR into ACC.                                                                                                                                               | -                         | 22                         |
|       XOR2       |     0x23     |      -       |     Pop 2     |    ACC ← S1 ^ S0     | Pops two integers, bitwise XOR into ACC.                                                                                                                                              | -                         | 23                         |
|       SHL2       |     0x24     |      -       |     Pop 2     |    ACC ← S1 << S0    | Pops two integers, shift second left by top into ACC.                                                                                                                                 | -                         | 24                         |
|       SHR2       |     0x25     |      -       |     Pop 2     |    ACC ← S1 >> S0    | Pops two integers, shift second right by top into ACC.                                                                                                                                | -                         | 25                         |
|      ADD_ST      |     0x26     |      -       | Pop 2, Push 1 |          -           | Pops two integers, adds, pushes result back to stack.                                                                                                                                 | -                         | 26                         |
|      SUB_ST      |     0x27     |      -       | Pop 2, Push 1 |          -           | Pops two integers, subtracts, pushes result.                                                                                                                                          | -                         | 27                         |
|      MUL_ST      |     0x28     |      -       | Pop 2, Push 1 |          -           | Pops two integers, multiplies, pushes result.                                                                                                                                         | -                         | 28                         |
|      DIV_ST      |     0x29     |      -       | Pop 2, Push 1 |          -           | Pops two integers, divides, pushes result.                                                                                                                                            | -                         | 29                         |
|      MOD_ST      |     0x2A     |      -       | Pop 2, Push 1 |          -           | Pops two integers, modulo, pushes result.                                                                                                                                             | -                         | 2A                         |
|      AND_ST      |     0x2B     |      -       | Pop 2, Push 1 |          -           | Pops two integers, bitwise AND, pushes result.                                                                                                                                        | -                         | 2B                         |
|      OR_ST       |     0x2C     |      -       | Pop 2, Push 1 |          -           | Pops two integers, bitwise OR, pushes result.                                                                                                                                         | -                         | 2C                         |
|      XOR_ST      |     0x2D     |      -       | Pop 2, Push 1 |          -           | Pops two integers, bitwise XOR, pushes result.                                                                                                                                        | -                         | 2D                         |
|      SHL_ST      |     0x2E     |      -       | Pop 2, Push 1 |          -           | Pops two integers, shift, pushes result.                                                                                                                                              | -                         | 2E                         |
|      SHR_ST      |     0x2F     |      -       | Pop 2, Push 1 |          -           | Pops two integers, shift, pushes result.                                                                                                                                              | -                         | 2F                         |
|      NEG_ST      |     0x30     |      -       | Pop 1, Push 1 |          -           | Negates top stack slot.                                                                                                                                                               | -                         | 30                         |
|      NOT_ST      |     0x31     |      -       | Pop 1, Push 1 |          -           | Bitwise NOT of top stack slot.                                                                                                                                                        | -                         | 31                         |
|     ADD_IMM      |     0x32     |  i32 value   |       -       | ACC ← ACC + `value`  | Adds 32-bit immediate to ACC.                                                                                                                                                         | -                         | 32 XX XX XX XX             |
|     SUB_IMM      |     0x33     |  i32 value   |       -       | ACC ← ACC - `value`  | Subtracts 32-bit immediate from ACC.                                                                                                                                                  | -                         | 33 XX XX XX XX             |
|     MUL_IMM      |     0x34     |  i32 value   |       -       | ACC ← ACC * `value`  | Multiplies ACC by 32-bit immediate.                                                                                                                                                   | -                         | 34 XX XX XX XX             |
|     DIV_IMM      |     0x35     |  i32 value   |       -       | ACC ← ACC / `value`  | Divides ACC by 32-bit immediate.                                                                                                                                                      | -                         | 35 XX XX XX XX             |
|     MOD_IMM      |     0x36     |  i32 value   |       -       | ACC ← ACC % `value`  | Modulo ACC by 32-bit immediate.                                                                                                                                                       | -                         | 36 XX XX XX XX             |
|     AND_IMM      |     0x37     |  i32 value   |       -       | ACC ← ACC & `value`  | Bitwise AND ACC with 32-bit immediate.                                                                                                                                                | -                         | 37 XX XX XX XX             |
|      OR_IMM      |     0x38     |  i32 value   |       -       | ACC ← ACC \| `value` | Bitwise OR ACC with 32-bit immediate.                                                                                                                                                 | -                         | 38 XX XX XX XX             |
|     XOR_IMM      |     0x39     |  i32 value   |       -       | ACC ← ACC ^ `value`  | Bitwise XOR ACC with 32-bit immediate.                                                                                                                                                | -                         | 39 XX XX XX XX             |
|     SHL_IMM      |     0x3A     |  i32 value   |       -       | ACC ← ACC << `value` | Shift ACC left by 32-bit immediate.                                                                                                                                                   | -                         | 3A XX XX XX XX             |
|     SHR_IMM      |     0x3B     |  i32 value   |       -       | ACC ← ACC >> `value` | Shift ACC right by 32-bit immediate.                                                                                                                                                  | -                         | 3B XX XX XX XX             |
|    ADD_IMM_ST    |     0x3C     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, adds immediate, pushes result.                                                                                                                                          | -                         | 3C XX XX XX XX             |
|    SUB_IMM_ST    |     0x3D     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, subtracts immediate, pushes result.                                                                                                                                     | -                         | 3D XX XX XX XX             |
|    MUL_IMM_ST    |     0x3E     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, multiplies by immediate, pushes result.                                                                                                                                 | -                         | 3E XX XX XX XX             |
|    DIV_IMM_ST    |     0x3F     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, divides by immediate, pushes result.                                                                                                                                    | -                         | 3F XX XX XX XX             |
|    MOD_IMM_ST    |     0x40     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, modulo by immediate, pushes result.                                                                                                                                     | -                         | 40 XX XX XX XX             |
|    AND_IMM_ST    |     0x41     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, bitwise AND with immediate, pushes result.                                                                                                                              | -                         | 41 XX XX XX XX             |
|    OR_IMM_ST     |     0x42     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, bitwise OR with immediate, pushes result.                                                                                                                               | -                         | 42 XX XX XX XX             |
|    XOR_IMM_ST    |     0x43     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, bitwise XOR with immediate, pushes result.                                                                                                                              | -                         | 43 XX XX XX XX             |
|    SHL_IMM_ST    |     0x44     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, shift left by immediate, pushes result.                                                                                                                                 | -                         | 44 XX XX XX XX             |
|    SHR_IMM_ST    |     0x45     |  i32 value   | Pop 1, Push 1 |          -           | Pops integer, shift right by immediate, pushes result.                                                                                                                                | -                         | 45 XX XX XX XX             |
|       FADD       |     0x50     |      -       |     Pop 1     |   ACC ← ACC + S0f    | Pops float, adds to ACC.                                                                                                                                                              | -                         | 50                         |
|       FSUB       |     0x51     |      -       |     Pop 1     |   ACC ← ACC - S0f    | Pops float, subtracts from ACC.                                                                                                                                                       | -                         | 51                         |
|       FMUL       |     0x52     |      -       |     Pop 1     |   ACC ← ACC * S0f    | Pops float, multiplies ACC.                                                                                                                                                           | -                         | 52                         |
|       FDIV       |     0x53     |      -       |     Pop 1     |   ACC ← ACC / S0f    | Pops float, divides ACC.                                                                                                                                                              | -                         | 53                         |
|      FADD2       |     0x54     |      -       |     Pop 2     |   ACC ← S1f + S0f    | Pops two floats, adds into ACC.                                                                                                                                                       | -                         | 54                         |
|      FSUB2       |     0x55     |      -       |     Pop 2     |   ACC ← S1f - S0f    | Pops two floats, subtracts top from second into ACC.                                                                                                                                  | -                         | 55                         |
|      FMUL2       |     0x56     |      -       |     Pop 2     |   ACC ← S1f * S0f    | Pops two floats, multiplies into ACC.                                                                                                                                                 | -                         | 56                         |
|      FDIV2       |     0x57     |      -       |     Pop 2     |   ACC ← S1f / S0f    | Pops two floats, divides second by top into ACC.                                                                                                                                      | -                         | 57                         |
|     FADD_ST      |     0x58     |      -       | Pop 2, Push 1 |          -           | Pops two floats, adds, pushes result.                                                                                                                                                 | -                         | 58                         |
|     FSUB_ST      |     0x59     |      -       | Pop 2, Push 1 |          -           | Pops two floats, subtracts, pushes result.                                                                                                                                            | -                         | 59                         |
|     FMUL_ST      |     0x5A     |      -       | Pop 2, Push 1 |          -           | Pops two floats, multiplies, pushes result.                                                                                                                                           | -                         | 5A                         |
|     FDIV_ST      |     0x5B     |      -       | Pop 2, Push 1 |          -           | Pops two floats, divides, pushes result.                                                                                                                                              | -                         | 5B                         |
|       FNEG       |     0x5C     |      -       |       -       |      ACC ← -ACC      | Negates ACC as a float.                                                                                                                                                               | -                         | 5C                         |
|     FADD_IMM     |     0x5D     |  f32 value   |       -       | ACC ← ACC + `value`  | Adds 32-bit float immediate to ACC.                                                                                                                                                   | -                         | 5D XX XX XX XX             |
|     FSUB_IMM     |     0x5E     |  f32 value   |       -       | ACC ← ACC - `value`  | Subtracts 32-bit float immediate from ACC.                                                                                                                                            | -                         | 5E XX XX XX XX             |
|     FMUL_IMM     |     0x5F     |  f32 value   |       -       | ACC ← ACC * `value`  | Multiplies ACC by 32-bit float immediate.                                                                                                                                             | -                         | 5F XX XX XX XX             |
|     FDIV_IMM     |     0x60     |  f32 value   |       -       | ACC ← ACC / `value`  | Divides ACC by 32-bit float immediate.                                                                                                                                                | -                         | 60 XX XX XX XX             |
|   FADD_IMM_ST    |     0x61     |  f32 value   | Pop 1, Push 1 |          -           | Pops float, adds immediate, pushes result.                                                                                                                                            | -                         | 61 XX XX XX XX             |
|   FSUB_IMM_ST    |     0x62     |  f32 value   | Pop 1, Push 1 |          -           | Pops float, subtracts immediate, pushes result.                                                                                                                                       | -                         | 62 XX XX XX XX             |
|   FMUL_IMM_ST    |     0x63     |  f32 value   | Pop 1, Push 1 |          -           | Pops float, multiplies by immediate, pushes result.                                                                                                                                   | -                         | 63 XX XX XX XX             |
|   FDIV_IMM_ST    |     0x64     |  f32 value   | Pop 1, Push 1 |          -           | Pops float, divides by immediate, pushes result.                                                                                                                                      | -                         | 64 XX XX XX XX             |
|      CMP_EQ      |     0x65     |      -       |     Pop 1     |   ACC ← ACC == S0    | Pops integer and compares it to ACC, storing true if they are equal and false otherwise in ACC.                                                                                       | -                         | 65                         |
|      CMP_NE      |     0x66     |      -       |     Pop 1     |   ACC ← ACC != S0    | Pops integer and compares it to ACC, storing true if they are not equal and false otherwise in ACC.                                                                                   | -                         | 66                         |
|      CMP_LT      |     0x67     |      -       |     Pop 1     |    ACC ← ACC < S0    | Pops integer and compares it to ACC, storing true if ACC is less than the value and false otherwise in ACC.                                                                           | -                         | 67                         |
|      CMP_GT      |     0x68     |      -       |     Pop 1     |    ACC ← ACC > S0    | Pops integer and compares it to ACC, storing true if ACC is greater than the value and false otherwise in ACC.                                                                        | -                         | 68                         |
|     CMP_LTE      |     0x69     |      -       |     Pop 1     |   ACC ← ACC <= S0    | Pops integer and compares it to ACC, storing true if ACC is less than or equal to the value and false otherwise in ACC.                                                               | -                         | 69                         |
|     CMP_GTE      |     0x6A     |      -       |     Pop 1     |   ACC ← ACC >= S0    | Pops integer and compares it to ACC, storing true if ACC is greater than or equal to the value and false otherwise in ACC.                                                            | -                         | 6A                         |
|     FCMP_EQ      |     0x6B     |      -       |     Pop 1     |   ACC ← ACC == S0    | Pops float and compares it to ACC, storing true if they are equal and false otherwise in ACC.                                                                                         | -                         | 6B                         |
|     FCMP_NE      |     0x6C     |      -       |     Pop 1     |   ACC ← ACC != S0    | Pops float and compares it to ACC, storing true if they are not equal and false otherwise in ACC.                                                                                     | -                         | 6C                         |
|     FCMP_LT      |     0x6D     |      -       |     Pop 1     |    ACC ← ACC < S0    | Pops float and compares it to ACC, storing true if ACC is less than the value and false otherwise in ACC.                                                                             | -                         | 6D                         |
|     FCMP_GT      |     0x6E     |      -       |     Pop 1     |    ACC ← ACC > S0    | Pops float and compares it to ACC, storing true if ACC is greater than the value and false otherwise in ACC.                                                                          | -                         | 6E                         |
|     FCMP_LTE     |     0x6F     |      -       |     Pop 1     |   ACC ← ACC <= S0    | Pops float and compares it to ACC, storing true if ACC is less than or equal to the value and false otherwise in ACC.                                                                 | -                         | 6F                         |
|     FCMP_GTE     |     0x70     |      -       |     Pop 1     |   ACC ← ACC >= S0    | Pops float and compares it to ACC, storing true if ACC is greater than or equal to the value and false otherwise in ACC.                                                              | -                         | 70                         |
|     CMP_EQ0      |     0x71     |      -       |       -       |    ACC ← ACC == 0    | Sets ACC to true if it's equal to 0 and false otherwise.                                                                                                                              | -                         | 71                         |
|     CMP_NE0      |     0x72     |      -       |       -       |    ACC ← ACC == 0    | Sets ACC to true if it's not equal to 0 and false otherwise.                                                                                                                          | -                         | 72                         |
|     CMP_LT0      |     0x73     |      -       |       -       |    ACC ← ACC < 0     | Sets ACC to true if it's less than to 0 and false otherwise.                                                                                                                          | -                         | 73                         |
|     CMP_GT0      |     0x74     |      -       |       -       |    ACC ← ACC > 0     | Sets ACC to true if it's greater than to 0 and false otherwise.                                                                                                                       | -                         | 74                         |
|     CMP_LTE0     |     0x75     |      -       |       -       |    ACC ← ACC <= 0    | Sets ACC to true if it's less than or equal to 0 and false otherwise.                                                                                                                 | -                         | 75                         |
|     CMP_GTE0     |     0x76     |      -       |       -       |    ACC ← ACC >= 0    | Sets ACC to true if it's greater than or equal to 0 and false otherwise.                                                                                                              | -                         | 76                         |
|     FCMP_EQ0     |     0x77     |      -       |       -       |   ACC ← ACC == 0f    | Sets ACC to true if it's equal to 0 and false otherwise.                                                                                                                              | -                         | 71                         |
|     FCMP_NE0     |     0x78     |      -       |       -       |   ACC ← ACC == 0f    | Sets ACC to true if it's not equal to 0 and false otherwise.                                                                                                                          | -                         | 72                         |
|     FCMP_LT0     |     0x79     |      -       |       -       |    ACC ← ACC < 0f    | Sets ACC to true if it's less than to 0 and false otherwise.                                                                                                                          | -                         | 73                         |
|     FCMP_GT0     |     0x7A     |      -       |       -       |    ACC ← ACC > 0f    | Sets ACC to true if it's greater than to 0 and false otherwise.                                                                                                                       | -                         | 74                         |
|    FCMP_LTE0     |     0x7B     |      -       |       -       |   ACC ← ACC <= 0f    | Sets ACC to true if it's less than or equal to 0 and false otherwise.                                                                                                                 | -                         | 75                         |
|    FCMP_GTE0     |     0x7C     |      -       |       -       |   ACC ← ACC >= 0f    | Sets ACC to true if it's greater than or equal to 0 and false otherwise.                                                                                                              | -                         | 76                         |
|     PUSH_ACC     |     0x80     |      -       |    Push 1     |          -           | Pushes the value of ACC onto the stack.                                                                                                                                               | -                         | 80                         |
|     PUSH_SP      |     0x81     |      -       |    Push 1     |          -           | Pushes the value of SP onto the stack.                                                                                                                                                | -                         | 81                         |
|     POP_ACC      |     0x82     |      -       |     Pop 1     |       ACC ← S0       | Pops a value from the stack and inserts it into ACC.                                                                                                                                  | -                         | 82                         |
|      POP_SP      |     0x83     |      -       |     Pop 1     |          -           | Pops a value from the stack and inserts it into SP.                                                                                                                                   | -                         | 83                         |
|   POP_DISCARD    |     0x84     |   i8 count   |  Pop `count`  |          -           | Pops `count` values from the stack and discards them.                                                                                                                                 | -                         | 84 XX                      |
|      CONST       |     0x85     |   i8 value   |       -       |    ACC ← `value`     | Moves an immediate `value` into ACC.                                                                                                                                                  | -                         | 85 XX                      |
|     CONST32      |     0x86     |  i32 value   |       -       |    ACC ← `value`     | Moves an immediate `value` into ACC.                                                                                                                                                  | -                         | 86 XX XX XX XX             |
|     CONST64      |     0x87     |  i64 value   |       -       |    ACC ← `value`     | Moves an immediate `value` into ACC.                                                                                                                                                  | -                         | 87 XX XX XX XX XX XX XX XX |
|     CONST_ST     |     0x88     |   i8 value   |    Push 1     |          -           | Pushes an immediate `value` to the stack.                                                                                                                                             | -                         | 88 XX                      |
|    CONST32_ST    |     0x89     |  i32 value   |    Push 1     |          -           | Pushes an immediate `value` to the stack.                                                                                                                                             | -                         | 89 XX XX XX XX             |
|    CONST64_ST    |     0x8A     |  i64 value   |    Push 1     |          -           | Pushes an immediate `value` to the stack.                                                                                                                                             | -                         | 8A XX XX XX XX XX XX XX XX |
|       LOAD       |     0x8B     |  i16 index   |       -       |  ACC ← S\[`index`\]  | Load a stack value located at the unsigned immediate `index` into ACC.                                                                                                                | -                         | 8B XX XX                   |
|     LOAD_ST      |     0x8C     |  i16 index   |    Push 1     |          -           | Load a stack value located at the unsigned immediate `index` and pushes it to the stack.                                                                                              | -                         | 8C XX XX                   |
|      STORE       |     0x8D     |  i16 index   |       -       |          -           | Store the value in ACC into the stack slot located at the unsigned immediate `index`.                                                                                                 | -                         | 8D XX XX                   |
|     STORE_ST     |     0x8E     |  i16 index   |     Pop 1     |          -           | Pops a value from the stack and moves it into the stack slot located at the unsigned immediate `index`.                                                                               | -                         | 8E XX XX                   |
|       JMP        |     0x97     |  i16 branch  |       -       |          -           | Increments PC by `branch`.                                                                                                                                                            | -                         | 97 XX XX                   |
|        JZ        |     0x98     |  i16 branch  |       -       |          -           | Increments PC by `branch` if ACC is equal to false.                                                                                                                                   | -                         | 98 XX XX                   |
|       JNZ        |     0x99     |  i16 branch  |       -       |          -           | Increments PC by `branch` if ACC is equal to true, tested as NOT equal to false (0).                                                                                                  |                           |                            |