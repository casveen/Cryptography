CC        := g++
#MPI       := mpicc
FLAGS      = -Wall
TESTDEP    = enigma.cpp
DEP        = enigma.cpp bombe.cpp
HEADER     = enigma.h   bombe.h
PROGRAMS = $(patsubst %.cpp, %.exe, $(wildcard *.cpp))

.PHONY : all clean test valgrind

all:
	$(MAKE) $(PROGRAMS)

enigma.exe: enigma.cpp
	$(CC) $< -o $@ $(FLAGS)

test.exe : test.cpp
	$(CC) $< -o $@ $(FLAGS) $(TESTDEP)

bombe.exe : bombe.cpp
	$(CC) $< -o $@ $(FLAGS) enigma.cpp


clean :
	rm -f $(PROGRAMS)

valgrind :
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test.exe

print:
	echo $(PROGRAMS)
