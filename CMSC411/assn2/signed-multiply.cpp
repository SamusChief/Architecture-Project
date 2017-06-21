// use Booth's algorithm to multiply two signed 32-bit numbers

#include "bittypes.h"
#include <stdio.h>

// you write this function to multiply two 32-bit integers to a 64-bit result
// using Booth's multiply algorithm 
// use only uint32_t, uint64_t, addition, shifts, and one loop over the bits
int64_t multiply(int32_t a, int32_t b)
{
        // A should be a
        int32_t A = a;
	// S should be -a
	int32_t S = -a;
	// use this for an extra bit on the end (65 bits total)
	int32_t extra = 0;
	// initialize P as an intComponents to access each half
	intComponents P;
	// b starts in the right half
	P.parts[0] = b;
	P.parts[1] = 0;
	int i;
	for(i = 0; i < 32; i++) {
		// 2: 10
		if(P.parts[0] & 1 && !extra) {
		        P.parts[1] += S;
		}
		// 1: 01
		if(!(P.parts[0] & 1) && extra) {
		        P.parts[1] += A;
		}
		// for 3 and 0, do nothing, use P as is
     		extra = P.parts[0] & 1;
		P.num >>= 1;
	}
	return P.num;
}

int main(int argc, char *argv[])
{
	// get numbers to multiply
	if (argc != 3) {
		printf("must provide two numbers as command line arguments\n");
		return 1;
	}

	int32_t a, b;
	sscanf(argv[1], "%i", &a);
	sscanf(argv[2], "%i", &b);

	int64_t myResult = multiply(a, b);
	int64_t stdResult = static_cast<int64_t>(a) * static_cast<int64_t>(b);

	printf("multiplying %d * %d (0x%08x * 0x%08x)\n", a, b, a, b);
	printf("built-in  says (0x%016llx) %lld\n", stdResult, stdResult);
	printf("hand-made says (0x%016llx) %lld\n", myResult, myResult);

	return 0;
}
