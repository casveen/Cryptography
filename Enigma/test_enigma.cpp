// main() provided by Catch in file test.cpp.
#include "enigma.h"
#include "include/catch.hpp"
#include "rotors.cpp"   //all rotors,

const int MAX_TESTS   = 1;
const int MESSAGE_SIZE= 26 * 10;
const int WIRES       = 26;
const int WHEELS      = 4;

TEST_CASE("Testing wiring of all known commonly used rotors") {
    // cout << "HHHHHHHHHH\n";
    for (Rotor rotor : allRotors) { REQUIRE(rotor.is_valid()); }
}

TEST_CASE("Testing wiring of single random rotors") {
    // test some randomly made rotors
    for (int t= 0; t < MAX_TESTS; ++t) {
        Rotor rotor= Rotor(WIRES);
        rotor.randomize();
        REQUIRE(rotor.is_valid());
    }
}

TEST_CASE("Testing wiring of an ensemble of rotors together with a reflector") {
    // an ensemble of rotors and a reflector should give a self-reciprocal,
    // surjective ecryption with no letter encrypting to itself
    for (int t= 0; t < MAX_TESTS; ++t) {
        Enigma enigma= Enigma(WHEELS, WIRES);
        enigma.randomize();
        int c, r;
        for (int w= 0; w < WIRES; w++) {
            c= enigma.encrypt(w);
            enigma.reset();
            r= enigma.encrypt(c);
            enigma.reset();
            REQUIRE(r == w);
            REQUIRE(c != w);
        }
    }
}

TEST_CASE("Testing encryption and encryption of text") {
    // twise encrypting a plaintext should return the plaintext
    int *m= new int[MESSAGE_SIZE];
    int *c, *r;   // ciphertext
    for (int t= 0; t < MAX_TESTS; ++t) {
        // allocate c, r
        c= new int[MESSAGE_SIZE];
        r= new int[MESSAGE_SIZE];
        // make a random enigma
        Enigma enigma= Enigma(WHEELS, WIRES);
        enigma.randomize();
        // make random message
        for (int i= 0; i < MESSAGE_SIZE; i++) { m[i]= rand() % WIRES; }
        // encrypt twice
        c= enigma.encrypt(m, MESSAGE_SIZE);   // allocates new
        enigma.reset();
        r= enigma.encrypt(c, MESSAGE_SIZE);   // allocates new
        // compare messages
        for (int i= 0; i < MESSAGE_SIZE; i++) { REQUIRE(m[i] == r[i]); }
        delete[] c;
        delete[] r;
    }
    delete[] m;
}

TEST_CASE("Testing a concrete example; encryption of a sample of the Donitz "
          "message") {
    // test if the enigma can encrypt/decrypt a portion of the Karl donitz
    // message
    Reflector ThinReflectorC= Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
    Enigma    enigma({VIII, VI, V, Beta}, ThinReflectorC);
    enigma.set_plugboard("AE.BF.CM.DQ.HU.JN.LX.PR.SZ.VW");
    enigma.set_rotor_position("ZPOY");
    enigma.set_ring_setting("KTDC");
    string encryption= enigma.encrypt("RBBFPMHPHGCZXTDYGAHGUFXGEWKBLKGJ");
    REQUIRE(encryption == "FOLGENDESISTSOFORTBEKANNTZUGEBEN");
}
