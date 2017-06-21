// use the standard hardware algorithm to multiply two unsigned 32-bit numbers

#include "bittypes.h"
#include <stdio.h>

// you write this function to multiply two 32-bit integers to a 64-bit result
// using the standard hardware multiply algorithm
// use only uint32_t, uint64_t, addition, shifts, and one loop over the bits
uint64_t multiply(uint32_t a, uint32_t b)
{
        uint64_t product = 0;
	uint64_t multiplier = a; // cast the a and b values to 64 bit
	uint64_t multiplicand = b;

	// move through, shifting a to right, b to left, and adding
	while (multiplier != 0) {
	        if(multiplier & 1) {
		  product = product + multiplicand;
	        }
		multiplier = multiplier >> 1;
		multiplicand = multiplicand << 1;
	}
	return product;
}

int main(int argc, char *argv[])
{
	// get numbers to multiply
	if (argc != 3) {
		printf("must provide two numbers as command line arguments\n");
		return 1;
	}

	uint32_t a, b;
	sscanf(argv[1], "%i", &a);
	sscanf(argv[2], "%i", &b);

	uint64_t myResult = multiply(a, b);
	uint64_t stdResult = static_cast<uint64_t>(a) * static_cast<uint64_t>(b);

	printf("multiplying %d * %d (0x%08x * 0x%08x)\n", a, b, a, b);
	printf("built-in  says (0x%016llx) %llu\n", stdResult, stdResult);
	printf("hand-made says (0x%016llx) %llu\n", myResult, myResult);

	return 0;
}
