Name: Tristan Adams (tristana@umbc.edu)
Due Date: 9/23/15
Project: Homework 2: Unsigned/signed multiply, floating point multiply/add
Description:
	- These files will execute via "make" then "make program-name parameter1 parameter2"
	- They all do arithmetic using bit shifting and the like.
	- Part 1 does unsigned multiplication
	- Part 2 does signed multiplication using Booth's algorithm
	- Part 3 does floating-point multiplication with only intger operations
	- Part 4 does floating point addition with only integer operations

Known Bugs:
	- Part 4, Floating Point Addition, strange output for inputs of the
	  format: x.y -x or -x x.y (where the x's are identical whole integers, 
	  and y is any number following the decimal)
	     - For example, 1.1 and -1 results in strange output, as would
	       3.456 and -3
	     - -1.1 and 1 or -3.456 and 3 work fine, however
	     - This also seems to occur for x -y, where x - abs(y) = 1 
