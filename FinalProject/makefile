assembler: main.o compile.o general_functions.o list.o structures.o
	gcc *.c -Wall -ansi -pedantic -o assembler
main.o: main.c compile.h
	gcc main.c -Wall -ansi -pedantic -c
compile.o: compile.c compile.h general_functions.h list.h structures.h constants.h list.h
	gcc compile.c -Wall -ansi -pedantic -c
general_functions.o: general_functions.c general_functions.h
	gcc general_functions.c -Wall -ansi -pedantic -c
list.o: list.c list.h
	gcc list.c -Wall -ansi -pedantic -c
structures.o: structures.c structures.h constants.h list.h
	gcc structures.c -Wall -ansi -pedantic -c
