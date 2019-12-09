CC        := g++
#MPI       := mpicc
FLAGS      = -Wall -O3
TESTDEP    = enigma.cpp rotors.cpp test_enigma.cpp test_bombe.cpp bombe.cpp
DEP        = enigma.cpp bombe.cpp
#HEADER     = enigma.h   bombe.h
PROGRAMS = $(patsubst %.cpp, %.exe, $(wildcard *.cpp))
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
GCH = $(patsubst %.h, %.h.gch, $(wildcard *.h))

.PHONY : all clean test valgrind

all:
	$(MAKE) $(PROGRAMS)

#enigma.o :
#	$(CC) -c enigma.cpp $(FLAGS)
#	echo "enigma.o made"

#bombe.o :
#	$(CC) -c bombe.cpp $(FLAGS)
#	echo "bombe.o made"

#benchmarker.o : bombe.o enigma.o rotors.cpp
#	$(CC) -c benchmarker.cpp $(FLAGS)
#	echo "benchmarker.o made"

benchmarker.exe : benchmarker.o bombe.o enigma.o rotors.cpp
	$(CC) -o benchmarker.exe $(FLAGS) benchmarker.o bombe.o enigma.o rotors.cpp



	#benchmark : benchmarker.o
	#	$(CC) -o benchmarker.exe benchmarker.o

#object files
%.o : %.cpp
	g++ -o $@ -c $< $(FLAGS)

#dependency rules, for headers
enigma.o : enigma.h
bombe.o : enigma.h bombe.h
benchmarker.o : enigma.h bombe.h

#enigma.exe: enigma.cpp
#	$(CC) $< -o $@ $(FLAGS)

#enigma.o: enigma.cpp
#	$(CC) -c enigma.cpp $(FLAGS)

#test.exe : test.cpp enigma.o rotors.o test_enigma.o test_bombe.o bombe.o
#	$(CC) $< -o $@ $(FLAGS) enigma.o rotors.o test_enigma.o test_bombe.o bombe.o

#bombe.o : bombe.cpp enigma.o
#	$(CC) -c bombe.cpp enigma.o $(FLAGS)

#bombe.exe : bombe.cpp enigma.o bombe.o
#	$(CC) $< -o $@ $(FLAGS) enigma.o

#performance.exe : performance.cpp enigma.o bombe.o rotors.cpp
#	$(CC) $< -o $@ $(FLAGS) enigma.o bombe.o

#performance : performance.exe
#	valgrind --tool=callgrind ./performance.exe
#	kcachegrind

#benchmarker.o : benchmarker.cpp bombe.o enigma.o rotors.cpp
#	$(CC) -c benchmarker.cpp $(FLAGS) bombe.o enigma.o rotors.cpp

#benchmarker.exe : benchmarker.cpp bombe.o enigma.o rotors.cpp
#	$(CC) $< -o $@ $(FLAGS) bombe.o enigma.o rotors.cpp

#benchmark : benchmarker.o
#	$(CC) -o benchmarker.exe benchmarker.o



#test:
#	./test.exe

clean :
	rm -f $(PROGRAMS) $(OBJECTS) $(GCH)



valgrind :
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test.exe

print:
	echo $(PROGRAMS)
#only cpp files on compile line
