all: hw1.o
	g++ -Wall hw1.o -o hw1.out

hw1.o: hw1.c
	g++ -Wall -c hw1.c

clean:
	rm -rf *.o
	rm -f hw1.out
	rm -f *~*.h.gch
	rm -f *~
	rm -f results.txt

run:
	./hw1.out $(INFILE) $(OUTFILE)