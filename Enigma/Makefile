CC        := g++
#MPI       := mpicc
FLAGS      = -Wall
TESTDEP    = enigma.cpp
DEP        = enigma.cpp bombe.cpp
HEADER     = enigma.h   bombe.h
PROGRAMS = $(patsubst %.c, %.exe, $(wildcard *.c))

.PHONY : all clean test

all:
	$(MAKE) $(PROGRAMS)

enigma.exe: enigma.cpp $(DEP)
	$(CC) $< -o $@ $(FLAGS)

test.exe : test.cpp
	$(CC) $< -o $@ $(FLAGS) $(TESTDEP)

clean :
	rm -f *.o $(PROGRAMS)

serial:
	./serial_code.exe              $(KAPPA) $(ITERATIONS) $(IMAGE) $(patsubst %.jpg, %_smoothed.jpg, $(IMAGE))          0

parallel:
	mpirun -np 4 parallel_code.exe $(KAPPA) $(ITERATIONS) $(IMAGE) $(patsubst %.jpg, %_parallel_smoothed.jpg, $(IMAGE)) 0

test:
	printf "\rMaking serial denoising"; \
	./serial_code.exe $(KAPPA) $(ITERATIONS) $(IMAGE) $(patsubst %.jpg,%_smoothed.jpg,$(IMAGE)) 0;
	for number in `seq 1 37`; do \
		p=`expr $$number \* 100 / 37`; \
		printf "\rTesting... %3.0f%%" $$p; \
		mpirun -np $$number parallel_code.exe $(KAPPA) $(ITERATIONS) $(IMAGE) $(patsubst %.jpg, %_parallel_smoothed.jpg, $(IMAGE)) 0; \
		./test.exe $(patsubst %.jpg, %_parallel_smoothed.jpg, $(IMAGE)) $(patsubst %.jpg, %_smoothed.jpg, $(IMAGE)); \
	done
	printf "\n"

print:
	echo $(PROGRAMS)
