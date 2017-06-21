Project CMSC 411
Daniel Franchy
Dylan Chu
Tristan Adams

Project Language: C++

Description:
This collection of programs contains a compiler and a cpu simulator.
The compiler compiles an expanded subset of MIPS assembly into machine code.
The CPU simulator is able to run the compiled code as a simulated machine
enviroment.

Usage instructions:

Running "make" will build the code
It generates executable files as "Compiler" and "Simulator"
The compiler can be run as "Compiler assembly code.h" which generates a code.h
file full of compiled machine code.
The simulator will then link the code.h automatically when you run "Simulator"
it will read the machine code line by line, expecting an "enter" between each
cycle.

Design choices:
Using a struct to contain system enviroment variables and all function maps 
allowed us to easily pass them in a mutable state.

Our simulator runs by having maps between opCodes and functions to handle
the opcodes implementation.  When an instruction is read, it isolates the
opcode and runs the matching function.

Caveats:  
Can only run machine code contained in a file named "code.h"
Only runs a subset of MIPS instructions(The ones required by the project spec)
as such it would not be able to run assembly programs using instructions not 
in that subset.  Not an issue within our purpose, but a caveat none-the-less.


Extra Credit:
N/A
