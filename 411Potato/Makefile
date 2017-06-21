#!smake
#  @@ Makefile for CMSC 411 - Project
#  @@ Daniel Franchy (franchy1@umbc.edu)
#

all: clean build

build: Compiler Simulator Multi-Cycle-Simulator

Compiler: Compiler.c
	g++ Compiler.c -o Compiler

Simulator: Simulator.c code.h
	g++ Simulator.c -o Simulator

Multi-Cycle-Simulator: Multi-Cycle-Simulator.c code.h
	g++ Multi-Cycle-Simulator.c -o Multi-Cycle-Simulator

clean:
	rm -f Compiler Simulator Multi-Cycle-Simulator

