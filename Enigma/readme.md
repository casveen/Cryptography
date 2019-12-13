# Enigma and the Bombe
This repository contains code for encrypting/decrypting with an enigma, and breaking enigma encryptions with the algorithm used for the bombe.

Currently the enigma is very general, and there is no practical procedure for using standard wheels(only random but legally wired ones). There are some mechanical flaws, such as double-stepping not being implemented due to uncertainty as to how it works for arbitrary many wheels, and there is no automation of indicator procedure.

The Bombe is in progress and non-functional. Currently wiring is being modeled for, after which the basic framework is ready.

Information on the algorithm for the Bombe can be found [here](http://www.ellsbury.com/bombe1.htm) and [wikipedia](https://en.wikipedia.org/wiki/Bombe)

Information on the Enigma can be found [here](http://users.telenet.be/d.rijmenants/en/enigmatech.htm), [here](https://plus.maths.org/content/exploring-enigma) and for a mediocre article, on [wikipedia](https://en.wikipedia.org/wiki/Enigma_machine)([details on the rotors](https://en.wikipedia.org/wiki/Enigma_rotor_details)). There is also [this article](http://www.intelligenia.org/downloads/enigvar2.pdf) documenting variations on the enigma.

## Elements of the Enigma
### Rotors
### Plugboard
### Ring setting
### Initial position
### Indicator Procedure


## Usage

### Enigma
To create a randomly wired enigma machine with a specified number of rotors(pluss a reflector), run
```c++
Enigma enigma(number_of_rotors, number_of_letters);
```
*WIP* to create a specific enigma, run
```c++
Enigma enigma(Rotor_1, Rotor_2, Rotor_3, ... , Reflector); // f.ex enigma(IV, VI, I, R);
```
*WIP* where the rotors are provided as static variables in [...]

The enigma then has to be properly configured, setting the ring setting(ringscthellung), plugboard(steckering), initial rotor positions and proper indicator procedure.
```c++
enigma.set_ring_setting("AEG");
enigma.set_plugboard("AB CD EF GH IJ KL");
enigma.set_position("AAA");
enigma.indicator_early("KGR");
```
We are now ready to encrypt. You can encrypt a string, a file or an integer array
```c++
enigma.encrypt("ATTACKXATXDAWN");
enigma.encrypt_file("plaintext.txt", "ciphertext.txt");
enigma.encrypt({0,,,0,,,0,,,0,,}) //whatever attackatdawn represents
```
which returns a corresponding object.
### Bombe
First we make a bombe
```c++
Bombe bombe(number_of_letters); //number_of_letters is ususally 26
```
Then we have to provide the bombe with the rotors we suspect are used
```c++
bombe.set_rotors(rotor_1, rotor_2, rotor_3, ... , reflector);
```
Given that we suspect a given word(called a crib) is in the plaintext of a ciphertext, we provide the ciphertext together with the crib to the analyze function
```c++
bombe.analyze(ciphertext, crib);
```
From here the bombe does a exhaustive search by ruling out most possible rotor- and plugboard settings.

[XXX] more research needs to be done...

## To be done:
* Enigma
  - [x] Plugborad/Steckerbrettt
  - [/] Find appropriate method to make static Rotor objects representing the usual rotors.
     Candidates:
    * in the enigma header (probably really bad practice)
    * in the enigma.cpp, then enigma.cpp has to be included for the rotors to be used
    * as a separate file that can be read and made into wheels, makes naming unpractical
    * In an entirely separate file, say, rotors.cpp
  - [ ] Implement the various indicator methods
  - [ ] Make subclasses of the very general class Enigma, representing the more specific types of Enigma
  - [ ] Double stepping. Sources are very vague on how to implement this in general cases.
  - [ ] Encrypt from file
  - [/] Test on a known cipher- plaintext pair. (requires doublestepping, and correct indicator procedure)
  - [x] Make efficient version of Enigma::get_encryption(),  which gets the whole transformation of the enigma at a given rotation. Used in the Bombe, and needs to be quick
  - [ ] Parallelize
      * mpi?
      * openmp? wiring arrays might slosh if shared between threads
* Bombe
  - [x] Make it work
  - [/] Make efficient wiring of diagonal board to the enigma
  - [ ] Parallelize
      * mpi? probably most suitable, one thread on each rotor composition
      * openmp? again, sloshing might be a problem
* Coding practice
  - [x] Const all possible variables
  - [ ] Use consistent names
  - [ ] delete unused functions
* Other
  - [ ] make a vocabulary/code class, so that we are not restricted to just the standard 26 capital english letters
## Bombe

### How I want it to work:
Enigma is implemented.

To implement the wiring of the enigma, we make the diagonal board, with
a class Wire representing each wire.
(the language is a little unspecific here, so bear(?) with me)
Each letter (A-Z) has a bundle of wires representing the value it is steckered to.
f. ex, there are 26 wires representing the value A is steckered to.
So, there are 26 bundles (A-Z) with 26 wires each (steckered to A-Z).
This is represented by an array of wires, Wire tripplepointer wires, where the first index is the bundle
and the second index is the wire in the bundle. We use a tripple pointer since we
want the array elements to be pointers to a specific wire.

The b-wire in the A bundle is connected to the a-wire in the B-bundle, the c-wire
in the A-bundle connected to the a-wire in the C-bundle ad so on. We achieve the
same result by letting wires[i][j]=wires[j][i] (note, pointing to the same object)

connecting wire w1 and w2 uses the syntax w1.connect(w2)
(or equivalently w2.connect(w1))

Using the map, we have to do some wiring. if A is connected to B at position
0, we have to connect all wires in the A bundle to the B bundle through a
cartridge(battery?) offset by 0. this is done through the function
connect_cartridge(Wire dp bundle1, Wire  dp bundle2, Cartridge cart, int offset)
and has to be done for each step(XXX not good)

Ideally, we only need to do a single turn of one cartridge, and not turn all
cartridges for each step.

##What I learned
This is my first large C++ project, knowing only the basics and some related languages like C. The biggest source of bugs seems to be my insistence to program as is this was a java project, meaning that I really lost control of ownership of pointers and allocating at proper times. Here I catalogue some things that I had to learn the hard way in this process.
-classes that have ownership of some object O, and which does not allocate this object at standard instantiation should be pointers as the eventual cleanup will deallocate O which is then not allocated.(ex. Reflector r, and then trying to assign to it, which calls the first r-s destructor, deallocating "nothing").
-when working with streams. If trying to make the program output to either cout or some ofstream, it is easier to make your own outstream object and then give it the appropriate streambuf object(either from cout or the ofstream through .rdbuf()) with initialising it as myostream(appropriate_streambuf).
