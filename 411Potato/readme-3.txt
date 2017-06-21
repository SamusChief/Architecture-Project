Names:
	Tristan Adams (tristana@umbc.edu)
	Daniel Franchy (franchy1@umbc.edu)
	Dylan Chu (vh30178@umbc.edu)

Project Phase 3 readme

Multi-Cycle-Simulator.c
	Functions as a loop which simulates hardware, and goes through the
	  cycle states
	Treats each command as a case, based on the RTL.csv, and moves through
	  iterating "state" as it goes
        Registers are preserved as they would be in the datapath, although
          not explicitely wiped constantly, they are written to by a state
          prior to being read by a state in the correct sequence as would
          match the datapath
        Cycles end/begin with reading an integer from stdin, the end/begin
          is semantics depending on how you look at it.
        Register 0 is explicitely set to 0 every cycle as a software hack in
          order to properly emulate what real hardware would do
        ASCN and DSCN are in 3 cycles each, the first cycle communicating
          the intent to scan, and the second cycle prepping the
          scanned value for storage, and the third cycle writing it

RTL.csv
	Updated to be more correct in line with phase 2 feedback
	Added functionality with the new Register

Datamap.png
	Updated to be more in line with phase 2 feedback
	Cycle Time changes from 520 to 522 (added one register as per rubric)
