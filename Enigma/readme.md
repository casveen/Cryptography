# Enigma and the Bombe
This repository contains code for encrypting/decrypting with an enigma, and breaking enigma encryptions with the algorithm used for the bombe.

Information on the algorithm for the Bombe can be found [here](http://www.ellsbury.com/bombe1.htm) and [wikipedia](https://en.wikipedia.org/wiki/Bombe)

Information on the Enigma can be found [here](http://users.telenet.be/d.rijmenants/en/enigmatech.htm), [here](https://plus.maths.org/content/exploring-enigma) and for a mediocre article, on [wikipedia](https://en.wikipedia.org/wiki/Enigma_machine)([details on the rotors](https://en.wikipedia.org/wiki/Enigma_rotor_details)). There is also [this article](http://www.intelligenia.org/downloads/enigvar2.pdf) documenting variations on the enigma.

## Elements of the Enigma
The enigma consists of several mechanical components. Here is a quick explanation, but it does not come close to looking at a good diagram of these structures. For those you should check out the links above.
#### Rotors

#### Reflector

#### Plugboard

#### Ring setting
Each rotor has a movable ring with notches.
Setting the ring setting of the medium rotor to C, means that each wire is offset by 2. If current flows from the fast rotor at A(note, note wire A, but position A. This is a little confusing, but I would suggest looking at the sources for some good diagrams), it enters at a position Y (since "Y"=24="A"-2).
All in all, the ring setting gives a offset in where the current flows(just like the rotor position, kind of) but also determines the position of the notch(es).

#### Rotor position/Initial position
On a physical enigma, the rotor position are the letters shown in the windows to the rotor, read left to right. The rotor position determines the position of each wheel. The initial rotor position is the rotor position before the plaintext is encrypted.

#### Indicator Procedure

## Required libraries
Boost, specifically program options

## Compilation
There are 5 executables
* enigma.exe - encode in the terminal
* bombe.exe - crack code in terminal
* test.exe - run suite of tests
* benchmarker.exe - make benchmark results
* performance.exe     - should be deleted, not used anymore

All are compiled through g++ and make, so running
```bash
Enigma>> make all
```
compiles all executables.
If you dont use make, this is equivalent to
```bash
g++ -Wall -pedantic -Ofast -fopenmp  -c src/enigma_main.cpp -o build/enigma_main.o -I include -I src
g++ -Wall -pedantic -Ofast -fopenmp  -c src/enigma.cpp -o build/enigma.o -I include -I src
g++ -Wall -pedantic -Ofast -fopenmp  -c src/rotors.cpp -o build/rotors.o -I include -I src
g++ -o bin/enigma.exe -Wall -pedantic -Ofast -fopenmp  build/enigma_main.o build/enigma.o build/rotors.o -L lib -lboost_program_options
g++ -Wall -pedantic -Ofast -fopenmp  -c test/test.cpp -o build/test.o -I include -I src
g++ -Wall -pedantic -Ofast -fopenmp  -c test/test_enigma.cpp -o build/test_enigma.o -I include -I src
g++ -Wall -pedantic -Ofast -fopenmp  -c test/test_bombe.cpp -o build/test_bombe.o -I include -I src
g++ -Wall -pedantic -Ofast -fopenmp  -c src/bombe.cpp -o build/bombe.o -I include -I src
g++ -o bin/test.exe -Wall -pedantic -Ofast -fopenmp  build/test.o build/test_enigma.o build/test_bombe.o build/enigma.o build/bombe.o -I include -I src
g++ -Wall -pedantic -Ofast -fopenmp  -c src/performance.cpp -o build/performance.o -I include -I src
g++ -o bin/performance.exe -Wall -pedantic -Ofast -fopenmp  build/performance.o build/bombe.o build/enigma.o -I include -I src
g++ -Wall -pedantic -Ofast -fopenmp  -c test/benchmarker.cpp -o build/benchmarker.o -I include -I src
g++ -o bin/benchmarker.exe -Wall -pedantic -Ofast -fopenmp  build/benchmarker.o build/enigma.o build/bombe.o -I include -I src
```



## Usage

### Enigma in code
To use an enigma in your C++ code, import enigma.h (and remember to include the include directory of this project when linking), and create an enigma with the syntax
#### From initializer list and Reflector
To create a enigma machine with a specified number of rotors(pluss a reflector), run
```c++
Enigma enigma({rotor_1, rotor_2, ...}, reflector);
//Enigma enigma(stator. {rotor_1, rotor_2, ...}, reflector); if you want a non-standard stator
```
the rotors used above have to be rotor objects. Commonly used rotors are defined in "rotors.cpp", open the file to get the name of the rotors.
Otherwise you can make custom rotors by calling
```c++
Rotor rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", letters)
```
Where letters are the amount of letters, that is, the amount of wires in the rotor. You can use any positive integer amount of letters, but you will only get meaningful verbose output for <=26 letters. When using the rotor in an enigma, the amount of letters have to be an even number due to the mechanics of the reflector.

The first string determines what the letters map on to. I.e. if the 3-rd letter is K, the letter D(3rd letter when counting from 0) maps to K. Note that this does not mean that K maps to D, this however has to be the case for all letters when making a reflector which is made by the same procedure. For even more examples, see rotors.cpp.

When the enigma is initialized, you can set a ring setting, rotor position and steckering/plugboard setting.
It is very important that ring setting is set before rotor position.
```c++
enigma.set_ring_setting("EPEL");
enigma.set_rotor_position("CDTJ");
enigma.set_plugboard("AK. PQ. DF") //steckers A to K, P to Q and D to F
```

Usually the setting to use before encrypting the plaintext was determined by an indicator procedure, but these are not implemented and not needed for proper encryption.

We are now ready to encrypt. You can encrypt a string, an istream onto an ostream(for example file to cout, or file to another file) or an integer array
```c++
//string
enigma.encrypt("ATTACKXATXDAWN");
//integer(representing B)
enigma.encrypt(1)
//file to cout
//----make ostream, sharing buffer with cout
ofstream   outfilestream;
streambuf *streambuffer= cout.rdbuf();
ostream outstream(streambuffer);
//----open  file to read from
ifstream instream(<NAME OF INPUTFILE>);
//----and encrypt
enigma.encrypt_file(instream, outstream);
```

###From EnigmaSetting structure
the EnigmaSetting structure contains fields that define an enigma
```c++
struct EnigmaSetting {
    vector<Rotor> rotors;
    Reflector     reflector;
    Rotor         stator; //only used if trivial_stator=false
    bool          trivial_stator= true; //the trivial stator maps A to A, B to B, etc.
    Plugboard     plugboard;
    string        ring_setting;
    string        rotor_position;
};
```
Filling in the fields properly as above and calling the constructor of Enigma with this structure gives the corresponding enigma. From here you can encrypt as above.
##Enigma as executable
To use an enigma from the command line, use the program enigma.exe.
You will have to set the configuration here as well, use the --help flag to show documentation.

example
```bash
Enigma>> ./bin/enigma.exe --rotors VIII,VI,V,BETA --reflector THINREFLECTORC --plugboard AE.BF.CM.DQ.HU.JN.LX.
PR.SZ.VW --rotorposition CDSZ --ringsetting EPEL --plaintext LANOTCTOUARBBFPMHPHGCZXTDYGAHGUFXGEWKBLKGJWLQXXTGPJJAVTOCKZ
FSLPPQIHZFX


KRKRALLEXXFOLGENDESISTSOFORTBEKANNTZUGEBENXXICHHABEFOLGELNBEBEFEHLERHA
```



### Bombe
First we make a bombe
```c++
Bombe bombe({suspected_rotor_1, suspected_rotor_2, ...}, {suspected_reflector_1, suspected_reflector_1, ...});
```
You usually dont know the rotor configuration or even which rotors are used when trying to break a cipher, so in a worst case scenario the lists of suspected rotors would be all rotors, and list of reflectors all reflectors.

From here you only have to supply a crib to try and break the cipher
```c++
bombe.analyze(crib); //if you have one suspected crib
bombe.analyze({crib_1, crib_2, ...); //if you have several cribs
```
From here the bombe does a exhaustive search by ruling out all possible rotor configurations on all cribs provided.

#### Using the bombe with the configuration grid
During a run through all possible configurations, the bombe will configurations that are equivalnet several times. To avoid the unneccesary comparisons the bombe can keep track of which configurations it has checked with a configuration grid, this is enabled by passing true to the last argument in the initialization of the bombe.

Initially the checking will seem slower, but will gradually speed up and finish about 10 times faster than without using the configuration grid(depending on the amount of notches!). The configuration grid does however use A LOT of space by string a boolean for each possible configuration, easily using up towards 2 GB with a 3 rotor enigma.

Currently using the configuration grid on a >4 rotor enigma will lead to not being able to allocate the required memory.

## To be done:
* Enigma
  - [x] Plugborad/Steckerbrettt
  - [x] Find appropriate method to make static Rotor objects representing the usual rotors.
     Candidates:
    * in the enigma header (probably really bad practice)
    * in the enigma.cpp, then enigma.cpp has to be included for the rotors to be used
    * as a separate file that can be read and made into wheels, makes naming unpractical
    * In an entirely separate file, say, rotors.cpp
  - [ ] Implement the various indicator methods
  - [ ] Make subclasses of the very general class Enigma, representing the more specific types of Enigma
  - [x] Double stepping. Sources are very vague on how to implement this in general cases.
  - [x] Encrypt from file
  - [x] Test on a known cipher- plaintext pair. (requires doublestepping, and correct indicator procedure)
  - [x] Make efficient version of Enigma::get_encryption(),  which gets the whole transformation of the enigma at a given rotation. Used in the Bombe, and needs to be quick
  - [ ] Parallelize - not really needed
      * mpi?
      * openmp? wiring arrays might slosh if shared between threads
* Bombe
  - [x] Make it work
  - [x] Make efficient wiring of diagonal board to the enigma
  - [ ] Parallelize
      * mpi? probably most suitable, one thread on each rotor composition
      * openmp? again, sloshing might be a problem
* Coding practice
  - [x] Const all possible variables
  - [x] Use consistent names
  - [ ] delete unused functions
* Other
  - [ ] make a vocabulary/code class, so that we are not restricted to just the standard 26 capital english letters
  - [ ] make a tighter configuration grid
##What I learned
This is my first large C++ project, knowing only the basics and some related languages like C. The biggest source of bugs seems to be my insistence to program as is this was a java project, meaning that I really lost control of ownership of pointers and allocating at proper times. Here I catalogue some things that I had to learn the hard way in this process.
* classes that have ownership of some object O, and which does not allocate this object at standard instantiation should be pointers as the eventual cleanup will deallocate O which is then not allocated.(ex. Reflector r, and then trying to assign to it, which calls the first r-s destructor, deallocating "nothing").
* when working with streams. If trying to make the program output to either cout or some ofstream, it is easier to make your own outstream object and then give it the appropriate streambuf object(either from cout or the ofstream through .rdbuf()) with initialising it as myostream(appropriate_streambuf).
* initialize in the same order that variables are declared in class
* If possible const all variables and functions
* An object should not hold a pointer to another unless it exerts some unique kind of ownership over that object.
For example it would be more appropriate to use references rather than pointers to rotors in the enigma.
