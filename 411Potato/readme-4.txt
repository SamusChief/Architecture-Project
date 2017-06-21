Names:
        Tristan Adams (tristana@umbc.edu)
        Daniel Franchy (franchy1@umbc.edu)
        Dylan Chu (vh30178@umbc.edu)

Project Phase 4 readme

Multi-Cycle-Simulator.c
	Our simulator was working perfectly in the last
	phase and we opted to not make any enhancements, so it remains the
	same.  Note that although it uses the .c extension it is actually
	written with c++.

optimus-prime.asm
	Optimus prime is our final robot design.  In order to test out all our
	ideas in a proper simulation, we had each member of our group create 
	multiple robots, since there will be many robots in the final 
	simulation.  In the end we combined our successful ideas into one 
	robot.  We found out that an erratic, start-and-stop movement pattern
	was the best for evasion, and then wrote some simple aiming code so 
	our robot could aim and shoot.  Additonally, our robot still fires 
	short range shots occasionally, so we do not have to waste time 
	aiming.

Compile instructions:
	Our Multi-Cycle-Simulator expects the asm code to be compiled and in a 
	file named "code.h" at compile time.  The simulator itself can be
	compiled with the current version of g++ on the GL servers, no
	extraneous flags are needed.  If an asm compiler is needed, our
	Compiler.c is able to compile asm with the command line args of the
	input and output file, it is also a c++ file.
