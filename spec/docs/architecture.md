# **The BibbleVM Architecture**
This page describes an abstract machine, not any particular implementation of BibbleVM.

## **The Bibble Bytecode Executable Format**
Also known as the `bbx` format. Compiled code for the BibbleVM is represented using a hardware- and OS-independent binary format, typically stored in a file. 
The `bbx` file format defines the data, instructions and exported symbols within a BibbleVM executable or library. 
This format also includes details such as byte ordering that might not exist in a platform-dependent object file format.
<br><br>
The `bbx` format is covered in more detail in ["The bbx File Format"](bbx-format.md).

## **Data Types**
BibbleVM operates on 4 kinds of types: _primitive types_, the _handle_ type, the _pointer_ type and _reference types_. However, these are all stored in the same 64-bit value slots.<br>
The primitive types are: _integral types_ and _floating-point types_ (see below).
<br><br>
BibbleVM expects that all type checking is done at compile time, and does not have to be done by the VM. 
Values do not need to be inspectable to have their types determined at runtime. 
Instead, the instruction set distinguishes its operand types using instructions intended to operate on specific types.
For example, `add` and `fadd` are both instructions that add two numeric values and produce numeric results, but they're specialized for different operand types: _integer_ and _float_ respectively.
<br><br>
BibbleVM has explicit support for objects. An object is a dynamically allocated class whose memory is handled by the automatic storage manager and its VM type is `reference`.<br>
BibbleVM also has explicit support for manual memory allocation. Manual memory is mainly meant for array allocation, but can be used for small structures to save the automatic storage manager time. 
Its VM type is `pointer`, but is not required to be represented as a real platform-dependent memory pointer.

### **Integral Types**
Integrals in BibbleVM are 64-bit signed two's-complement integers with instructions to shrink their values down to 8-, 16- or 32-bit.

### **Floating-Point Types**
Floating-points (or "floats") in BibbleVM are 64-bit double-precision format IEEE 754 values as specified in the IEEE standard.

### **The Handle Type**
Unfortunately, BibbleVM can't be fully platform-independent, and at a certain point, it must interact with its host platform.<br>
The _handle_ type provides a way to pass around opaque platform-dependent values, such as Windows `HANDLE`s or Unix file descriptors, without exposing their internals.
<br><br>
The _handle_ type is stored as a 64-bit value like other VM values.
<br><br>
In combination with implementation-defined trap routines, handles act as the bridge between low-level libraries and the underlying systems, usually without requiring platform-dependent code to be loaded.

### **The Pointer Type**
The _pointer_ type represents manually managed memory in BibbleVM.<br>
A _pointer_ value refers to a region of memory allocated manually through the use of `alloc` instructions, and its lifetime is controlled entirely by the program.<br>
BibbleVM never automatically frees manual memory and never performs any pointer-based garbage collection.
<br><br>
The _pointer_ type is stored as a 64-bit standard value, but its internal representation is implementation-defined.<br>
They are **not** required to correspond to raw machine addresses and may instead be offsets, handles into an internal heap or other abstract identifiers.
<br><br>
Unlike typical pointers, performing pointer arithmetic is not explicitly supported and will lead to undefined behavior.
<br><br>
Because pointer-based memory is untyped, BibbleVM performs no automatic type checking, but might perform automatic bounds checks or pointer validation depending on safety settings.
All safety and memory discipline is the responsibility of the compiler and user code.<br>
Typical uses include array storage, temporary buffers, and low-level data manipulation that benefits from avoiding garbage collection overhead.
<br><br>
BibbleVM instructions that interact with pointer-based memory operate on _raw bytes_ and an offset from the base pointer.

### **The Reference Type**
The _reference_ type represents objects allocated by the automatic storage manager.<br>
A _reference_ value is essentially a pointer to a heap-allocated instance of a class, and its lifetime is tracked by BibbleVM's automatic storage manager.<br>
A _reference_ always identifies a fully typed object with a known class layout including fields, methods and an optional destructor.
<br><br>
The _reference_ type is stored as a 64-bit standard value, but unlike pointers, the VM is responsible for their reachability analysis and destruction using conservative garbage collection.
<br><br>
Because _references_ carry type information, instructions that operate on objects might perform automatic type checking depending on safety settings.
BibbleVM still offers instructions to let the program perform manual type checking.<br>
BibbleVM ensures memory safety for automatically managed objects.

## **Runtime Data Areas**
BibbleVM defines several runtime data areas that are used during the execution of a program. Some of these are global while others are per thread.
Global data areas are created when the VM starts and destroyed when the VM exits and per thread data areas are created when a thread is created and destroyed when the thread exits.

### **The `acc` Register**
`acc` is short for `accumulator`.
Each BibbleVM thread has its own `acc` register which holds exactly one 64-bit value.<br>
The `acc` register is **volatile**: its contents are not preserved across calls. During a function call, the caller must assume the contents of the `acc` register is overridden by the callee.

### **The `sp` Register**
