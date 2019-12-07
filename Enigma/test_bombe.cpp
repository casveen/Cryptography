// main() provided by Catch in file 020-TestCase-1.cpp.
#include "bombe.h"
#include "enigma.h"
#include "include/catch.hpp"
#include "rotors.cpp"   //all rotors,

SCENARIO("bombe finds the configuration of an enimga", "[bombe]") {

    GIVEN("Enigma with rotors I, II, III, reflector UKW and a long plaintext") {
        Bombe bombe({I, II, III}, UKWR);
        bombe.get_setting().stop_on_first_valid= true;
        Enigma enigma({I, II, III}, UKWR);
        // enigma.set_verbose(true);
        string plaintext=
            "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";

        WHEN("Given ciphertext encrypted with ring-setting AAA, "
             "ring-position AAA and no steckering") {
            cout << "test 1\n";
            enigma.set_rotor_position("AAA");
            enigma.set_ring_setting("AAA");
            enigma.set_plugboard("");
            string ciphertext= enigma.encrypt(plaintext);
            bombe.get_setting().starting_rotor_positions= "AAA";
            bombe.get_setting().starting_ring_setting   = "AAA";
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                enigma.set_setting(solutions[0]);
                CHECK(enigma.encrypt(ciphertext) == plaintext);
                CHECK(solutions[0].rotor_position == "AAA");
            }
        }

        WHEN("Given ciphertext encrypted with ring-position CAA and no "
             "steckering") {
            cout << "test 2\n";
            // there was a bug with the enigma doing two turns for the first
            // check this test would not pass with this bug
            enigma.set_rotor_position("CAA");
            enigma.set_ring_setting("AAA");
            enigma.set_plugboard("");
            string ciphertext= enigma.encrypt(plaintext);
            bombe.get_setting().starting_rotor_positions= "AAA";
            bombe.get_setting().starting_ring_setting   = "AAA";
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                // enigma.set_setting(solutions[0]);
                // enigma.reset();
                enigma.set_setting(solutions[0]);
                CHECK(enigma.encrypt(ciphertext) == plaintext);
                CHECK(solutions[0].rotor_position == "CAA");
            }
        }

        WHEN("Given ciphertext encrypted with ring-setting FGH, ring-position "
             "CDE, no steckering and bombe at same ring set") {
            cout << "test 3\n";
            enigma.set_rotor_position("CDE");
            enigma.set_ring_setting("FGH");
            string ciphertext= enigma.encrypt(plaintext);
            bombe.get_setting().starting_rotor_positions= "AAA";
            bombe.get_setting().starting_ring_setting   = "FGH";
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                // enigma.reset();
                // enigma.set_setting(solutions[0]);
                // cout<<"\n\nTEST: "<<enigma.encrypt(ciphertext)<<"\n";
                REQUIRE(solutions[0].rotor_position == "CDE");
                REQUIRE(solutions[0].ring_setting == "FGH");
            }
        }

        WHEN("Given ciphertext encrypted with ring-setting RFW, ring-position "
             "BCD, bombe in same config and steckering") {
            cout << "test 4\n";
            // there was a bug with the enigma doing two turns for the first
            // check this test would not pass with this bug
            enigma.set_rotor_position("MCW");
            enigma.set_ring_setting("BBA");
            enigma.set_plugboard("AC. BD. EG. IR. JT. QZ");
            string ciphertext= enigma.encrypt(plaintext);
            // bombe.set_rotor_position("RFW");
            bombe.get_setting().starting_rotor_positions= "AAA";
            bombe.get_setting().starting_ring_setting   = "BBA";
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                // enigma.reset();
                // enigma.set_setting(solutions[0]);
                // cout << "\n\nTEST: " << enigma.encrypt(ciphertext) << "\n";
                enigma.set_setting(solutions[0]);
                CHECK(enigma.encrypt(ciphertext) == plaintext);
                CHECK(solutions[0].rotor_position == "MCW");
                CHECK(solutions[0].ring_setting == "BBA");
                // compare plugboards
                Plugboard correct_plugboard("AC. BD. EG. IR. JT. QZ", 26);
                for (int i= 0; i < 26; ++i) {
                    CHECK(solutions[0].plugboard->get_wiring(i) ==
                          correct_plugboard.get_wiring(i));
                }
            }
        }

        WHEN("Given ciphertext encrypted with ring-setting GDA, rotor-position "
             "PAH, steckering and bombe starting from ring-setting AAA") {
            cout << "test 5\n";
            enigma.set_rotor_position("PAH");
            enigma.set_ring_setting("GDA");
            enigma.set_plugboard("AK. IE. DV. CQ. BN, MO, PJ. WR. UX");
            string ciphertext= enigma.encrypt(plaintext);
            enigma.reset();
            // bombe.set_rotor_position("RFW");
            bombe.set_ring_setting("AAA");
            THEN("Running bombe with a complete crib should return the above "
                 "setting") {
                cout << "ANALYZING in test 5\n";
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, plaintext);
                enigma.set_setting(solutions[0]);
                CHECK(enigma.encrypt(ciphertext) == plaintext);
                CHECK(solutions[0].rotor_position == "PAH");
                CHECK(solutions[0].ring_setting == "GDA");
                // compare plugboards
                Plugboard correct_plugboard(
                    "AK. IE. DV. CQ. BN, MO, PJ. WR. UX", 26);
                for (int i= 0; i < 26; ++i) {
                    CHECK(solutions[0].plugboard->get_wiring(i) ==
                          correct_plugboard.get_wiring(i));
                }
            }
        }
    }

    GIVEN("Enigma with rotors IV, V, VI, reflector UKW and plaintext") {
        Bombe bombe({IV, V, VI}, UKWR);
        bombe.get_setting().stop_on_first_valid= true;
        bombe.get_setting().max_ring_settings  = 30;
        // bombe.get_setting().interactive_wiring_mode= true;
        Enigma enigma({IV, V, VI}, UKWR);
        // enigma.set_verbose(true);
        string plaintext=
            "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
        string crib= "PLAINTEXTTOBEENCRYPTED";
        WHEN("Encrypting with RS:BBA, RP:JYL and steckering") {
            enigma.set_rotor_position("JYL");
            enigma.set_ring_setting("BBA");
            enigma.set_plugboard("AK. IE. DV. CQ. BN, MO, PJ. WR. UX");
            string ciphertext= enigma.encrypt(plaintext);
            bombe.set_ring_setting("AAA");
            THEN("Bombe should find the correct setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, crib);
                // enigma.set_setting(solutions[0]);
                enigma.set_setting(solutions[0]);
                CHECK(enigma.encrypt(ciphertext) == plaintext);
                CHECK(solutions[0].rotor_position == "JYL");
                CHECK(solutions[0].ring_setting == "BBA");
                Plugboard correct_plugboard(
                    "AK. IE. DV. CQ. BN, MO, PJ. WR. UX", 26);
                for (int i= 0; i < 26; ++i) {
                    CHECK(solutions[0].plugboard->get_wiring(i) ==
                          correct_plugboard.get_wiring(i));
                }
            }   // THEN
        }       // WHEN
    }           // GIVEN

    /*
    GIVEN("Enigma with rotors IV, V, VI, reflector UKW and plaintext") {
        Bombe bombe({VII, V, III}, UKWR);
        bombe.get_setting().stop_on_first_valid= true;
        // bombe.get_setting().max_ring_settings      = 30;
        Enigma enigma({IV, V, VI}, UKWR);
        string plaintext=
            "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
        string crib= "PLAINTEXT";
        WHEN("Encrypting with RS:AAA, RP:GHQ and steckering") {
            enigma.set_rotor_position("KOR");
            enigma.set_ring_setting("BCA");
            enigma.set_plugboard("AK. IE. DV. CQ. BN, MO, PJ. WR. UX");
            string ciphertext= enigma.encrypt(plaintext);
            bombe.set_ring_setting("AAA");
            THEN("Bombe should find the correct setting") {
                vector<struct EnigmaSetting> solutions=
                    bombe.analyze(ciphertext, crib);
                enigma.set_setting(solutions[0]);
                cout << "\n\nTEST: " << enigma.encrypt(ciphertext) << "\n";
                CHECK(solutions[0].rotor_position == "KOR");
                CHECK(solutions[0].ring_setting == "BCA");
                Plugboard correct_plugboard(
                    "AK. IE. DV. CQ. BN, MO, PJ. WR. UX", 26);
                for (int i= 0; i < 26; ++i) {
                    REQUIRE(solutions[0].plugboard->get_wiring(i) ==
                            correct_plugboard.get_wiring(i));
                }
            }   // THEN
        }       // WHEN
    }*/

}   // SCENARIO
