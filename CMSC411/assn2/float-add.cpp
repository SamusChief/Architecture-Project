// build a floating point addition from integer operations

#include "bittypes.h"
#include <stdio.h>

// you write this function
// use only integer types and operations (multiplies, adds, shifts, etc.)
// be sure to handle infinity, NaN and denormalized!
uint32_t add(uint32_t a, uint32_t b)
{
        // Extract the relevant parts
        uint32_t aSign = a & 0x80000000;
	uint32_t bSign = b & 0x80000000;
	uint32_t aExpo = a & 0x7F800000;
	uint32_t bExpo = b & 0x7F800000;
	aExpo >>= 23;
	bExpo >>= 23;
	uint32_t rExpo = 0;
	uint32_t aMant = a & 0x7FFFFF;
	uint32_t bMant = b & 0x7FFFFF;
	aMant |= 0x800000;
	bMant |= 0x800000;
	uint64_t rMant = 0;
	uint32_t result = 0;

	// infinity handling and nan handling; see chart in HW file for details
        int negInf = 0xFF800000;
        int posInf = 0x7F800000;
	int nan = 0x7FC00000;
        if (b == posInf) {
                if (a == negInf || a == nan)
		        return nan;
		return posInf;
        }
        if (b == negInf) {
	        if (a == posInf || a == nan)
		        return nan;
	        
		return negInf;
	}
	if (a == posInf) {
                if (b == negInf || b == nan)
		        return nan;
                
		return posInf;
	}
	if (a == negInf) {
                if (b == posInf || b == nan)
		        return nan;
                
		return negInf;
	}
	if (a == nan || b == nan)
	        return nan;

	if(aMant == bMant && aSign != bSign)
	  return 0;

	// 2. pick the bigger exponent
	if (aExpo > bExpo) {
	        // make this exponent the key one
	        rExpo = aExpo;
		// are the signs different? Subtract instead of add
		if (aSign > bSign || bSign > aSign) {
		        rMant = aMant - (bMant >> (aExpo - bExpo));
		}
		// signs are the same, use addition
		else {
		        rMant = aMant + (bMant >> (aExpo - bExpo));
		}
		// shift mantissa and exponent
		while (rMant > 0xFFFFFF) {
		  rMant >>= 1;
		  rExpo++;
		}
		// normalize mantissa
		while (rMant < 0x800000 && rMant != 0) {
		  rMant <<= 1;
		  rExpo--;
		}
		// reconstruct result
		result = result | aSign | (rExpo << 23) |
		  (rMant & 0x7FFFFF);
	}
	// use the bExpo as the primary exponent
	else if (bExpo > aExpo) {
	        rExpo = bExpo;
		// signs are different, subtract a from b
		if (aSign > bSign || bSign > aSign) {
		        rMant = bMant - (aMant >> (bExpo - aExpo));
		}
		// signs are the same, add a and b
		else {
		        rMant = bMant + (aMant >> (bExpo - aExpo));
		}
		// shift mantissa and increment exponent
		while (rMant > 0xFFFFFF) {
		  rMant >>= 1;
		  rExpo++;
		}
		// normalize mantissa
		while (rMant < 0x800000 && rMant != 0) {
		  rMant <<= 1;
		  rExpo--;
		}
		// reconstruct result
		result = result | aSign | (rExpo << 23) |
		  (rMant & 0x7FFFFF);
		
	}
	// exponents are equal
	else {
	        rExpo = aExpo;
		// signs are different, 
		if (bSign > aSign) {
		        rMant = bMant - aMant;
			// shift mantissa and increment exponent
			// then normalize
			while (rMant > 0xFFFFFF) {
			        rMant >>= 1;
				rExpo++;
                        }
                        while (rMant < 0x800000 && rMant != 0) {
			        rMant <<= 1;
				rExpo--;
                        }
                        // reconstruct result
                        result = result | bSign | (rExpo << 23) |
                          (rMant & 0x7FFFFF);

		}
		else if (aSign > bSign) {
		        rMant = aMant - bMant;
			// shifting mantissa, increment exponent
			// then normalize
			while (rMant > 0xFFFFFF) {
			        rMant >>= 1;
				rExpo++;
                        }
                        while (rMant < 0x800000 && rMant != 0) {
			        rMant <<= 1;
				rExpo--;
			}
                        // reconstruct result
                        result = result | aSign | (rExpo << 23) |
                          (rMant & 0x7FFFFF);
		}
		else {
		        rMant = aMant + bMant;
                        // shifting mantissa, increment exponent
                        // then normalize
			while (rMant > 0xFFFFFF) {
		                rMant >>= 1;
				rExpo++;
			}
			while ((rMant < 0x800000) & rMant != 0) {
		                rMant <<= 1;
				rExpo--;
			}
			// reconstruct result
			result = result | aSign | (rExpo << 23) |
			  (rMant & 0x7FFFFF);
		}
	}
	// return final result
	return result;
}

int main(int argc, char *argv[])
{
	FloatBits a, b, stdResult, myResult;

	// get numbers to add
	if (argc != 3) {
		printf("must provide two numbers as command line arguments\n");
		return 1;
	}
	sscanf(argv[1], "%f", &a.f);
	sscanf(argv[2], "%f", &b.f);

	// compute result
	stdResult.f = a.f + b.f;
	myResult.bits = add(a.bits, b.bits);

	printf("adding %g + %g (0x%08x + 0x%08x)\n", a.f, b.f, a.bits, b.bits);
	printf("built-in  says (0x%08x) %g\n", stdResult.bits, stdResult.f);
	printf("hand-made says (0x%08x) %g\n", myResult.bits, myResult.f);

	return 0;
}
