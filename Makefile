# Trivial makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C compilation.

# Note that rule for goal (parse) must be the first one in this file.

parse: parse.o scan.o
	g++ -o parse parse.o scan.o

clean:
	rm *.o parse

test: parse
	./tests/input.txt
test1: parse
	./tests/input1.txt
test2: parse
	./tests/input2.txt
test3: parse
	./tests/input3.txt
test4: parse
	./tests/input4.txt
test5: parse
	./tests/input5.txt
test6: parse
	./tests/input6.txt
test7: parse
	./tests/input7.txt
test8: parse
	./tests/input8.txt
test9: parse
	./tests/input9.txt

parse.o: scan.h
scan.o: scan.h
