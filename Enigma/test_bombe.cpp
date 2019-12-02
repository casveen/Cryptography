// main() provided by Catch in file 020-TestCase-1.cpp.
#include "bombe.h"
#include "enigma.h"
#include "include/catch.hpp"
#include "rotors.cpp"   //all rotors,

SCENARIO("bombe finds the configuration of an enimga", "[bombe]") {
    GIVEN("A bombe using wheels I, II, III and reflector UKW") {
        Bombe bombe({I, II, III}, UKWR);
        bombe.get_setting().stop_on_first_valid= true;
        Enigma enigma({I, II, III}, UKWR);
        // enigma.set_verbose(true);
        string plaintext=
            "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
        WHEN("Given a long ciphertext encrypted with ring-setting AAA, "
             "ring-position AAA and no steckering") {
            enigma.set_rotor_position("AAA");
            cout << enigma.get_rotor_position_as_string() << "\n";
            string ciphertext= enigma.encrypt(plaintext);
            enigma.reset();
            enigma.set_rotor_position("AAA");
            // enigma.reset();
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                REQUIRE(enigma.encrypt(ciphertext) == plaintext);
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                REQUIRE(solutions[0].rotor_position == "AAA");
            }
        }

        WHEN("Given a long ciphertext encrypted with ring-setting CAA, "
             "ring-position CDE and no steckering") {
            // there was a bug with the enigma doing two turns for the first
            // check this test would not pass with this bug
            enigma.set_rotor_position("CAA");
            string ciphertext= enigma.encrypt(plaintext);
            enigma.reset();
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                REQUIRE(solutions[0].rotor_position == "CAA");
            }
        }

        WHEN("Given a long ciphertext encrypted with ring-setting FGH, "
             "ring-position CDE and no steckering") {
            // there was a bug with the enigma doing two turns for the first
            // check this test would not pass with this bug
            enigma.set_rotor_position("FGH");
            string ciphertext= enigma.encrypt(plaintext);
            enigma.reset();
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                REQUIRE(solutions[0].rotor_position == "FGH");
            }
        }

        WHEN(
            "Given a long ciphertext encrypted with ring-setting RFW, "
            "ring-position BCD, bombe starting in same config and steckering") {
            // there was a bug with the enigma doing two turns for the first
            // check this test would not pass with this bug
            enigma.set_rotor_position("RFW");
            enigma.set_ring_setting("BCD");
            enigma.set_plugboard("AC. BD. EG. IR. JT. QZ");
            string ciphertext= enigma.encrypt(plaintext);
            enigma.reset();
            bombe.set_rotor_position("RFW");
            bombe.set_ring_setting("BCD");
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                REQUIRE(solutions[0].rotor_position == "RFW");
                REQUIRE(solutions[0].ring_setting == "BCD");
                // compare plugboards
                Plugboard correct_plugboard("AC. BD. EG. IR. JT. QZ", 26);
                for (int i= 0; i < 26; ++i) {
                    REQUIRE(solutions[0].plugboard->get_wiring(i) ==
                            correct_plugboard.get_wiring(i));
                }
            }
        }
        /*
      WHEN("Given a long ciphertext encrypted with ring-setting RFW, "
           "ring-position BCD and steckering") {
        // there was a bug with the enigma doing two turns for the first check
        // this test would not pass with this bug
        enigma.set_rotor_position("QFV");
        enigma.set_ring_setting("BCD");
        string ciphertext = enigma.encrypt(plaintext);
        enigma.reset();
        bombe.set_rotor_position("AAA");
        bombe.set_ring_setting("AAA");
        THEN("Running bombe with a complete crib should return the above "
             "setting") {
          vector<struct EnigmaSetting> solutions =
              bombe.analyze(ciphertext, plaintext);
          REQUIRE(solutions[0].rotor_position == "QFW");
        }
    }*/
    }
}

// TEST_CASE("Testing if bombe can find setting when")

/*
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

TEST_CASE("Testing wiring of an ensemble of rotors together with a reflector")
{
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
    // allocate c, r
    c = new int[MESSAGE_SIZE];
    r = new int[MESSAGE_SIZE];
    // make a random enigma
    Enigma enigma = Enigma(WHEELS, WIRES);
    enigma.randomize();
    // make random message
    for (int i = 0; i < MESSAGE_SIZE; i++) {
      m[i] = rand() % WIRES;
    }
    // encrypt twice
    c = enigma.encrypt(m, MESSAGE_SIZE); // allocates new
    enigma.reset();
    r = enigma.encrypt(c, MESSAGE_SIZE); // allocates new
    // compare messages
    for (int i = 0; i < MESSAGE_SIZE; i++) {
      REQUIRE(m[i] == r[i]);
    }
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
}*/
