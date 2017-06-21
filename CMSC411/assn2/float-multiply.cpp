// build a floating point multiply from integer operations

#include "bittypes.h"
#include <stdio.h>

// you write this function
// use only integer types and operations (multiplies, adds, shifts, etc.)
// be sure to handle infinity, NaN and denormalized!
uint32_t multiply(uint32_t a, uint32_t b)
{
        // 1. Extract the relevant parts
        int32_t aSign = a & 0x80000000;
	int32_t aExpo = a & 0x7F800000;
	int64_t aMant = a & 0x7FFFFF;
	int32_t bSign = b & 0x80000000;
	int32_t bExpo = b & 0x7F800000;
	int64_t bMant = b & 0x7FFFFF;

	// infinity handling and nan handling
        int negInf = 0xFF800000;
        int posInf = 0x7F800000;
	int nan = 0x7FC00000;
	if (b == nan || a == nan)
	  return nan;
        if (b == posInf) {
          if(aSign != 0)
            return negInf;
          else
            return posInf;
        }
        if (b == negInf) {
          if(aSign != 0)
            return posInf;
          else
            return negInf;
        }
        if (a == posInf) {
          if(bSign != 0)
            return negInf;
          else
            return posInf;
        }
        if (a == negInf) {
          if(bSign != 0)
            return posInf;
          else
            return negInf;
        }


	// 2. shift the exponents right and unbias them
	aExpo >>= 23;
	aExpo -= 127;
	bExpo >>= 23;
	bExpo -= 127;

	// 3. add in the hidden 24th bit
	aMant |= 0x800000;
	bMant |= 0x800000;

        // 4. add the exponents
	int32_t rExpo = aExpo + bExpo;
        // 5. multiply the mantissas, and shift them back right
	int64_t rMant = aMant * bMant;
	rMant >>= 24;

	// 6. adjust the exponent if the Mantissa allows it, otherwise shift left
	if(rMant & 0x800000)
		rExpo ++;
	else
	        rMant <<= 1;

	// 7. get the sign of the result
	int32_t rSign = aSign ^ bSign;

	// 8. handle overflow based on the sign
	if (rExpo < -126 || rExpo > 127) {
	  if (rSign != 0)
	    return negInf;
	  else
	    return posInf;
	}

	// 9. rebias and shift the exponent
	rExpo += 127;
	rExpo <<= 23;
	
	// 10. re mask values and repackage them
	rSign = rSign & 0x80000000;
	rExpo = rExpo & 0x7F800000;
	rMant = rMant & 0x7FFFFF;
	int32_t result = rMant | rExpo;
	result = result | rSign;

	// 11. return the result
	return result;
}

int main(int argc, char *argv[])
{
	FloatBits a, b, stdResult, myResult;

	// get numbers to multiply
	if (argc != 3) {
		printf("must provide two numbers as command line arguments\n");
		return 1;
	}
	sscanf(argv[1], "%f", &a.f);
	sscanf(argv[2], "%f", &b.f);

	// compute result
	stdResult.f = a.f * b.f;
	myResult.bits = multiply(a.bits, b.bits);

	printf("multiplying %g * %g (0x%08x * 0x%08x)\n", a.f, b.f, a.bits, b.bits);
	printf("built-in  says (0x%08x) %g\n", stdResult.bits, stdResult.f);
	printf("hand-made says (0x%08x) %g\n", myResult.bits, myResult.f);

	return 0;
}
