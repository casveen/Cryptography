CC:= g++
FLAGS= -Wall -pedantic -O3
SRC=src
BIN=bin
BUILD=build
INCLUDE=include
##NAMES
SOURCE_NAME    =enigma.cpp bombe.cpp enigma_main.cpp rotors.cpp
OBJECT_NAME    = $(SOURCE_NAME:%.cpp=%.o)
EXECUTABLE_NAME=enigma.exe test.exe performance.exe benchmark.exe
#TEST_NAME      =test.cpp test_bombe.cpp test_enigma.cpp
##FILES
SOURCE_FILES     = $(SOURCE_NAME:%=$(SRC)/%)
EXECUTABLE_FILES = $(EXECUTABLE_NAME:%=$(BIN)/%)
OBJECT_FILES     = $(SOURCE_NAME:%.cpp=$(BUILD)/%.o)
#TEST_FILES       = $(TEST_NAME:%.cpp=$(BUILD)/%.o)

LIB := -L lib -lboost_program_options
INC := -I include

#VPATH, so that make looks in correct directories
vpath %.cpp src test
vpath %.h   include
vpath %.o   build
vpath %.exe bin
#.PHONY : all clean test valgrind

check:
	@echo $(SOURCE_NAME:%=$(BUILD)/%)

##EXECUTABLES
enigma.exe : enigma_main.o enigma.o rotors.o
	$(CC) -o bin/enigma.exe $(FLAGS) $^ $(LIB)

benchmarker.exe : benchmarker.o bombe.o enigma.o rotors.cpp
	$(CC) -o bin/benchmarker.exe $(FLAGS) $^

performance.exe : performance.o bombe.o enigma.o rotors.cpp
	$(CC) -o bin/performance.exe $(FLAGS) $^

test.exe : test.o bombe.o enigma.o test_enigma.o test_bombe.o rotors.o
	$(CC) -o bin/test.exe $(FLAGS) $^

profile : performance.exe
	valgrind --tool=callgrind ./performance.exe
	kcachegrind

valgrind : performance.exe enigma.exe
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./performance.exe
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./enigma.exe --rotors I,II,III --reflector UKWK --plaintext ARTADOZSDUXDHCAMMRTCBVBLUYTOKGGEWZFYUICNNVPBRNYBRSCTSNUMLAYVAW



test : test.exe
	./test.exe

benchmark : benchmarker.exe
	./benchmarker.exe


clean:
	@echo " Cleaning...";
	@echo " rm -r $(BUILDDIR) $(EXECUTABLE_FILES)"; rm -r $(BUILD) $(EXECUTABLE_FILES)

#object files, static pattern
$(OBJECT_NAME): %.o: %.cpp
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@ -I $(INCLUDE)
#%.o : %.cpp
#	g++ -o $@ -c $< $(FLAGS)
