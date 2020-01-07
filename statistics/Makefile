CC:= g++
FLAGS= -Wall -pedantic -pg -g# -Ofast
SRC=src
BIN=bin
BUILD=build
INCLUDE=include
##NAMES
SOURCE_NAME    =statistics.cpp
OBJECT_NAME    = $(SOURCE_NAME:%.cpp=%.o)
EXECUTABLE_NAME=test.exe
#TEST_NAME      =test.cpp test_bombe.cpp test_enigma.cpp
##FILES
SOURCE_FILES     = $(SOURCE_NAME:%=$(SRC)/%)
EXECUTABLE_FILES = $(EXECUTABLE_NAME:%=$(BIN)/%)
OBJECT_FILES     = $(SOURCE_NAME:%.cpp=$(BUILD)/%.o)
##DEPENDENCIES
TEST_DEP_NAMES   = statistics.o test.o 
TEST_DEP         = $(TEST_DEP_NAMES:%=$(BUILD)/%)

LIB := -L lib
INC := -I $(INCLUDE) -I $(SRC)

.PHONY : all clean test remake

#VPATH, so that make looks in correct directories
vpath %.cpp src test
#vpath %.hpp include
#vpath %.o   build
#vpath %.exe bin



#check:
#	@echo $(SOURCE_NAME:%=$(BUILD)/%)

##EXECUTABLES
all : $(EXECUTABLE_NAME)

test.exe : $(TEST_DEP)
	$(CC) -o bin/test.exe $(FLAGS) $^ $(INC)

test : test.exe
	./bin/test.exe

clean:
	@echo " Cleaning...";
	@echo " rm -r $(BUILDDIR) $(EXECUTABLE_FILES)"; rm -r -f $(BUILD) $(EXECUTABLE_FILES)

remake:
	make clean
	make all

#object files
$(BUILD)/%.o: %.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) -c $< -o $@ $(INC)
