# what to build if you just say "make"
TARGETS = unsigned-multiply signed-multiply float-multiply float-add
all: $(TARGETS)

# tell make about dependencies between files
unsigned_multiply: unsigned_multiply.cpp bittypes.h
signed_multiply: signed_multiply.cpp bittypes.h
float_multiply: float_multiply.cpp bittypes.h
float_add: float_add.cpp bittypes.h

# "make clean" removes build files
clean:
	rm -f $(TARGETS)
	rm -f *.o

