CC        := g++
#MPI       := mpicc
FLAGS      = -Wall -g -O3
TESTDEP    = enigma.cpp rotors.cpp test_enigma.cpp test_bombe.cpp bombe.cpp
DEP        = enigma.cpp bombe.cpp
HEADER     = enigma.h   bombe.h
PROGRAMS = $(patsubst %.cpp, %.exe, $(wildcard *.cpp))
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

.PHONY : all clean test valgrind

all:
	$(MAKE) $(PROGRAMS)

enigma.exe: enigma.cpp
	$(CC) $< -o $@ $(FLAGS)

enigma.o: enigma.h enigma.cpp
	$(CC) $< -c $(FLAGS) enigma.cpp

test.exe : test.cpp enigma.o rotors.o test_enigma.o test_bombe.o bombe.o
	$(CC) $< -o $@ $(FLAGS) enigma.o rotors.o test_enigma.o test_bombe.o bombe.o

bombe.o : bombe.h enigma.h bombe.cpp
	$(CC) $< -c $(FLAGS) bombe.cpp

bombe.exe : bombe.cpp enigma.o
	$(CC) $< -o $@ $(FLAGS) enigma.o

performance.exe : performance.cpp enigma.o bombe.o rotors.cpp
	$(CC) $< -o $@ $(FLAGS) enigma.o bombe.o


test:
	./test.exe

clean :
	rm -f $(PROGRAMS) $(OBJECTS)



valgrind :
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test.exe

print:
	echo $(PROGRAMS)
