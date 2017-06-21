// type definitions for manipulating raw bits of numbers

#include <stdint.h>				// definitions of int32_t and int64_t types */

// convert between float and its bit representation
union FloatBits {
	float f;
	uint32_t bits;
};

// convert between 64-bit unsigned integer and 32-bit components
union uintComponents {
	uint64_t num;
	uint32_t parts[2];
};

// convert between 64-bit signed integer and 32-bit components
union intComponents {
	int64_t num;
	int32_t parts[2];
};
