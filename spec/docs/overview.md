# **Overview**
_This document is still work in progress and some sections or entire chapters have not yet been written. If you have any questions, wait until the full Bibble Virtual Machine technical specification has been released._

## **The Purpose of BibbleVM**
BibbleVM is engineered with performance as the top priority.
To reach that goal, it omits many runtime safety mechanisms found in modern virtual machines. 
There is no dynamic type checking, no runtime-enforced function constraints, and no implicit validation. 
The VM assumes that compilers and assemblers emit correct code, following the same trust model as native machine code.
<br><br>
However, this does not eliminate the ability to run untrusted code. BibbleVM provides a clearly defined **sandboxing mode** that restricts all interaction with the host environment.
The sandbox is enforced inside the VM and cannot be bypassed without explicitly loading unsafe, user-approved libraries.
This mode is controlled entirely by the embedding host: the standalone BibbleVM distribution always runs in trusted mode.
<br><br>
BibbleVM supports both **automatic storage management** via classes, objects and a conservative garbage collection algorithm and **manual storage management**
via explicit allocation and raw memory operations. Both approaches integrate cleanly with the VM's object model, and manual memory operations remain functional in sandboxed mode,
where they operate under tightened safety rules.
<br><br>
The execution model is built around deterministic 8-byte slots and a broad, specialized instruction set designed for performance rather than compactness. 
Each operation has a clearly defined behavior to keep compilation straightforward and interpreter overhead low.
<br><br>
Finally, BibbleVM is designed for seamless integration into larger systems. 
Together with its compiler, the VM forms a fast, statically typed, compiled environment that can replace the typical embedded scripting engines used today. 
The execution model, binary format, and host interface are all structured to make embedding straightforward and predictable.