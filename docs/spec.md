BibbleVM Instruction Set Architecture v0.1-predev
=

# 1. Overview

## 1.1 Execution Model
BibbleVM is a stack-based machine with three dedicated registers:
- **Accumulator (ACC)**: Holds the result of most instructions. Subsequent instructions can consume ACC directly or move it to the stack.
- **Stack Pointer (SP)**: Holds the top of the current stack frame as an index from its base. SP is implicitly managed by push/pop instructions, but may be manually modified if required.
- **Program Counter (PC)**: Holds the address of the next instruction to execute. PC is internal-only and cannot be accessed directly by bytecode. Control flow instructions modify PC implicitly.

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
- **Control Flow**: E.g JMP, JEQ, JLT, CALL, RET.
- **Memory / Object Management**: e.g ALLOCATE, FREE, READ, WRITE, NEWUNIQUE, NEWSHARED.
- **Miscellaneous**: E.g NOP. <!-- TODO: add more here -->
- **Debug**: E.g BRK.

---

# 3. Instruction Set

## 3.1 Instruction Notation
The following conventions are used throughout the next section to describe **stack effect**, **ACC effect** and operand usage.

### 3.1.1 Terminology
This section defines some common terms seen in the instruction set:
- **integer**: A numeric value represented as a signed integer. Its size is either explicitly stated where it's used or should be assumed to be the same size as a stack slot (8 bytes).
- **float**: A numeric value represented in IEEE 754 single- or double-precision format. The size of either explicitly stated where it's used or should be assumed to be the same size as a stack slot (8 bytes or double-precision).

### 3.1.2 Register Operands
See [Section 1.1](#11-execution-model).
- **ACC**: The accumulator register.
- **SP**: The stack pointer register.
- **PC**: The program counter register.

### 3.1.3 Stack Operands
- **S0**: Top of the stack as indicated by **SP**.
- **S1**: Next value down from the top (second from top).
- **S2, S3, Sn...**: Subsequent values down the stack.
- **S[n]**: Stack slot at `n` starting at index 0 for the bottom.
- `f` suffix means the value addresses is treated as a floating-point value.

These values represent stack slots BEFORE stack effect is applied.

### 3.1.4 Stack Effects
- **Pop n**: Removes the top `n` values from the stack (`S0..S(n-1)`) and consumes them accordingly.
- **Push n**: Pushes `n` values onto the stack.
- **-**: Instruction does not modify the stack.

### 3.1.5 ACC Effects
- **ACC ← expr**: The accumulator is updated with the result of `expr`.
- Use stack operands in `expr` to indicate used stack values.
- **-**: Instruction does not modify ACC.

### 3.1.6 Operand Column
- Lists instruction-specific operands encoded directly into the bytecode, such as stack indices, immediate values, or offsets.
- Each operand is written in the form `(i|f)n operand-name`, where:
  - `i` indicates an **integer** value.
  - `f` indicates a **floating-point** value.
  - `n` specifies the **size of the operand in bits**.
  - `operand-name` is the **name used in the instruction’s effect column**.

- Operands appear directly after the opcode byte(s) in bytecode in the same order as they were written.
- Use `-` if there are no operands.

### 3.1.7 Notes Column
- Additional remarks or implementation details that don't fit in description.
- Use `-` if there are no notes.

### 3.1.8 Encoding Column
- Shows how instructions should be encoded in bytecode.
- `X` means any value, usually used for operands.

## 3.2 Instructions

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
|     PUSH_ACC     |     0x70     |      -       |    Push 1     |          -           | Pushes the value of ACC onto the stack.                                                                                                                                               | -                         | 70                         |
|     PUSH_SP      |     0x71     |      -       |    Push 1     |          -           | Pushes the value of SP onto the stack.                                                                                                                                                | -                         | 71                         |
|     POP_ACC      |     0x72     |      -       |     Pop 1     |       ACC ← S0       | Pops a value from the stack and inserts it into ACC.                                                                                                                                  | -                         | 72                         |
|      POP_SP      |     0x73     |      -       |     Pop 1     |          -           | Pops a value from the stack and inserts it into SP.                                                                                                                                   | -                         | 73                         |
|   POP_DISCARD    |     0x74     |   i8 count   |  Pop `count`  |          -           | Pops `count` values from the stack and discards them.                                                                                                                                 | -                         | 74 XX                      |
|      CONST       |     0x75     |   i8 value   |       -       |    ACC ← `value`     | Moves an immediate `value` into ACC.                                                                                                                                                  | -                         | 75 XX                      |
|     CONST32      |     0x76     |  i32 value   |       -       |    ACC ← `value`     | Moves an immediate `value` into ACC.                                                                                                                                                  | -                         | 76 XX XX XX XX             |
|     CONST64      |     0x77     |  i64 value   |       -       |    ACC ← `value`     | Moves an immediate `value` into ACC.                                                                                                                                                  | -                         | 77 XX XX XX XX XX XX XX XX |
|     CONST_ST     |     0x78     |   i8 value   |    Push 1     |          -           | Pushes an immediate `value` to the stack.                                                                                                                                             | -                         | 78 XX                      |
|    CONST32_ST    |     0x79     |  i32 value   |    Push 1     |          -           | Pushes an immediate `value` to the stack.                                                                                                                                             | -                         | 79 XX XX XX XX             |
|    CONST64_ST    |     0x7A     |  i64 value   |    Push 1     |          -           | Pushes an immediate `value` to the stack.                                                                                                                                             | -                         | 7A XX XX XX XX XX XX XX XX |
|       LOAD       |     0x7B     |  i16 index   |       -       |  ACC ← S\[`index`\]  | Load a stack value located at the unsigned immediate `index` into ACC.                                                                                                                | -                         | 7B XX XX                   |
|     LOAD_ST      |     0x7C     |  i16 index   |    Push 1     |          -           | Load a stack value located at the unsigned immediate `index` and pushes it to the stack.                                                                                              | -                         | 7C XX XX                   |
|      STORE       |     0x7D     |  i16 index   |       -       |          -           | Store the value in ACC into the stack slot located at the unsigned immediate `index`.                                                                                                 | -                         | 7D XX XX                   |
|     STORE_ST     |     0x7E     |  i16 index   |     Pop 1     |          -           | Pops a value from the stack and moves it into the stack slot located at the unsigned immediate `index`.                                                                               | -                         | 7E XX XX                   |