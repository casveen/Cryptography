#define CATCH_CONFIG_MAIN

#include "enigma.h"
#include "include/catch.hpp"

const int MAX_TESTS = 100;
const int MESSAGE_SIZE = 26 * 10;
const int WIRES = 26;
const int WHEELS = 4;

const Rotor IC = Rotor("DMTWSILRUYQNKFEJCAZBPGXOHV");
const Rotor IIC = Rotor("HQZGPJTMOBLNCIFDYAWVEUSRKX");
const Rotor IIIC = Rotor("UQNTLSZFMREHDPXKIBVYGJCWOA");
const Rotor IR = Rotor("JGDQOXUSCAMIFRVTPNEWKBLZYH");
const Rotor IIR = Rotor("NTZPSFBOKMWRCJDIVLAEYUXHGQ");
const Rotor IIIR = Rotor("JVIUBHTCDYAKEQZPOSGXNRMWFL");
const Reflector UKWR = Reflector("QYHOGNECVPUZTFDJAXWMKISRBL"); // ref
const Rotor ETWR = Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
const Rotor IK = Rotor("PEZUOHXSCVFMTBGLRINQJWAYDK");
const Rotor IIK = Rotor("ZOUESYDKFWPCIQXHMVBLGNJRAT");
const Rotor IIIK = Rotor("EHRVXGAOBQUSIMZFLYNWKTPDJC");
const Reflector UKWK = Reflector("IMETCGFRAYSQBZXWLHKDVUPOJN"); // ref
const Rotor ETWK = Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
const Rotor I = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
const Rotor II = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
const Rotor III = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
const Rotor IV = Rotor("ESOVPZJAYQUIRHXLNFTGKDCMWB", "J");
const Rotor V = Rotor("VZBRGITYUPSDNHLXAWMJQOFECK", "Z");
const Rotor VI = Rotor("JPGVOUMFYQBENHZRDKASXLICTW", "ZM");
const Rotor VII = Rotor("NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM");
const Rotor VIII = Rotor("FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM");
const Rotor Beta = Rotor("LEYJVCNIXWPBQMDRTAKZGFUHOS");
const Rotor Gamma = Rotor("FSOKANUERHMBTIYCWLQPZXVGJD");
const Rotor ReflectorA = Rotor("EJMZALYXVBWFCRQUONTSPIKHGD");
const Rotor ReflectorB = Rotor("YRUHQSLDPXNGOKMIEBFZCWVJAT");
const Rotor ReflectorC = Rotor("FVPJIAOYEDRZXWGCTKUQSBNMHL");
const Rotor ThinReflectorB = Rotor("ENKQAUYWJICOPBLMDXZVFTHRGS");
const Rotor ThinReflectorC = Rotor("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
const Rotor ETW = Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const vector<Rotor> allRotors = {IC,
                                 IIC,
                                 IIIC,
                                 IR,
                                 IIR,
                                 IIIR,
                                 UKWR,
                                 ETWR,
                                 IK,
                                 IIK,
                                 IIIK,
                                 UKWK,
                                 ETWK,
                                 I,
                                 II,
                                 III,
                                 IV,
                                 V,
                                 VI,
                                 VII,
                                 VIII,
                                 Beta,
                                 Gamma,
                                 ReflectorA,
                                 ReflectorB,
                                 ReflectorC,
                                 ThinReflectorB,
                                 ThinReflectorC,
                                 ETW};

TEST_CASE("Testing wiring of all known commonly used rotors") {
  for (Rotor rotor : allRotors) {
    REQUIRE(rotor.is_valid());
  }
}

TEST_CASE("Testing wiring of single random rotors") {
  // test some randomly made rotors
  for (int t = 0; t < MAX_TESTS; ++t) {
    Rotor rotor = Rotor(WIRES);
    rotor.randomize();
    REQUIRE(rotor.is_valid());
  }
}


TEST_CASE("Testing wiring of an ensemble of rotors together with a reflector") {
  // an ensemble of rotors and a reflector should give a self-reciprocal,
  // surjective ecryption with no letter encrypting to itself
  for (int t = 0; t < MAX_TESTS; ++t) {
    Enigma enigma = Enigma(WHEELS, WIRES);
    enigma.randomize();
    int c, r;
    for (int w = 0; w < WIRES; w++) {
      c = enigma.encrypt(w);
      enigma.reset();
      r = enigma.encrypt(c);
      enigma.reset();
      REQUIRE(r == w);
      REQUIRE(c != w);
    }
  }
}

TEST_CASE("Testing encryption and encryption of text") {
  // twise encrypting a plaintext should return the plaintext
  int *m = new int[MESSAGE_SIZE];
  int *c, *r; // ciphertext
  for (int t = 0; t < MAX_TESTS; ++t) {
    //allocate c, r
    c= new int[MESSAGE_SIZE];
    r= new int[MESSAGE_SIZE];
    //make a random enigma
    Enigma enigma = Enigma(WHEELS, WIRES);
    enigma.randomize();
    // make random message
    for (int i = 0; i < MESSAGE_SIZE; i++) { m[i] = rand() % WIRES; }
    // encrypt twice
    c=enigma.encrypt(m, MESSAGE_SIZE); //allocates new
    enigma.reset();
    r=enigma.encrypt(c, MESSAGE_SIZE); //allocates new
    // compare messages
    for (int i = 0; i < MESSAGE_SIZE; i++) { REQUIRE(m[i] == r[i]); }
    delete[] c;
    delete[] r;
  }
  delete[] m;
}

TEST_CASE("Testing a concrete example; encryption of a sample of the Donitz "
          "message") {
  // test if the enigma can encrypt/decrypt a portion of the Karl donitz
  // message
  Reflector ThinReflectorC = Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
  Enigma enigma({VIII, VI, V, Beta}, ThinReflectorC);
  enigma.set_plugboard("AE.BF.CM.DQ.HU.JN.LX.PR.SZ.VW");
  enigma.set_rotor_position("ZPOY");
  enigma.set_ring_setting("KTDC");
  string encryption = enigma.encrypt("RBBFPMHPHGCZXTDYGAHGUFXGEWKBLKGJ");
  REQUIRE(encryption == "FOLGENDESISTSOFORTBEKANNTZUGEBEN");
}

/*
int main() {
    static Rotor IC=                Rotor("DMTWSILRUYQNKFEJCAZBPGXOHV");
    static Rotor IIC=               Rotor("HQZGPJTMOBLNCIFDYAWVEUSRKX");
    static Rotor IIIC=              Rotor("UQNTLSZFMREHDPXKIBVYGJCWOA");
    static Rotor IR=                Rotor("JGDQOXUSCAMIFRVTPNEWKBLZYH");
    static Rotor IIR=               Rotor("NTZPSFBOKMWRCJDIVLAEYUXHGQ");
    static Rotor IIIR=              Rotor("JVIUBHTCDYAKEQZPOSGXNRMWFL");
    static Reflector UKWR=      Reflector("QYHOGNECVPUZTFDJAXWMKISRBL"); //ref
    static Rotor ETWR=              Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
    static Rotor IK=                Rotor("PEZUOHXSCVFMTBGLRINQJWAYDK");
    static Rotor IIK=               Rotor("ZOUESYDKFWPCIQXHMVBLGNJRAT");
    static Rotor IIIK=              Rotor("EHRVXGAOBQUSIMZFLYNWKTPDJC");
    static Reflector UKWK=      Reflector("IMETCGFRAYSQBZXWLHKDVUPOJN"); //ref
    static Rotor ETWK=              Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
    static Rotor I=                 Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
    static Rotor II=                Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
    static Rotor III=               Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
    static Rotor IV=                Rotor("ESOVPZJAYQUIRHXLNFTGKDCMWB", "J");
    static Rotor V=                 Rotor("VZBRGITYUPSDNHLXAWMJQOFECK", "Z");
    static Rotor VI=                Rotor("JPGVOUMFYQBENHZRDKASXLICTW", "ZM");
    static Rotor VII=               Rotor("NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM");
    static Rotor VIII=              Rotor("FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM");
    static Rotor Beta=              Rotor("LEYJVCNIXWPBQMDRTAKZGFUHOS");
}*/
