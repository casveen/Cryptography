// make some performance runs, recording performance for a bad crib
//#include "bombe.h"
#include "bombe.hpp"
#include "enigma.hpp"
#include "rotors.cpp"   //all rotors,
float benchmark1() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    BombeUnit bombe({I, II, III}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = 10;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI}, UKWR);
    // enigma.set_verbose(true);
    string plaintext=
        "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAA");
    enigma.set_ring_setting("AAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMETEXT");
    return bombe.get_setting().performance_ring_setting_mean;
}

float benchmark2() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    BombeUnit bombe({I, II, III}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = 10;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI}, UKWR);
    // enigma.set_verbose(true);
    string plaintext=
        "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAA");
    enigma.set_ring_setting("AAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXT");
    return bombe.get_setting().performance_ring_setting_mean;
}

float benchmark3() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    BombeUnit bombe({I, II, III}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = 10;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI}, UKWR);
    // enigma.set_verbose(true);
    string plaintext=
        "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAA");
    enigma.set_ring_setting("AAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXTANDSOMEMORE");
    return bombe.get_setting().performance_ring_setting_mean;
}

float benchmark4() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    BombeUnit bombe({I, II, III, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAAA";
    bombe.get_setting().starting_ring_setting   = "AAAA";
    bombe.get_setting().max_ring_settings       = 10;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI, I}, UKWR);
    // enigma.set_verbose(true);
    string plaintext=
        "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAAA");
    enigma.set_ring_setting("AAAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMETEXT");
    return bombe.get_setting().performance_ring_setting_mean;
}

float benchmark5() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    BombeUnit bombe({I, II, III, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAAA";
    bombe.get_setting().starting_ring_setting   = "AAAA";
    bombe.get_setting().max_ring_settings       = 10;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI, I}, UKWR);
    // enigma.set_verbose(true);
    string plaintext=
        "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAAA");
    enigma.set_ring_setting("AAAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXT");
    return bombe.get_setting().performance_ring_setting_mean;
}

float benchmark6() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    BombeUnit bombe({I, II, III, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAAA";
    bombe.get_setting().starting_ring_setting   = "AAAA";
    bombe.get_setting().max_ring_settings       = 10;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI, I}, UKWR);
    // enigma.set_verbose(true);
    string plaintext=
        "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAAA");
    enigma.set_ring_setting("AAAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXTANDDOMEMORE");
    return bombe.get_setting().performance_ring_setting_mean;
}
int main() {
    benchmark1();
    benchmark2();
    benchmark3();
    benchmark4();
    benchmark5();
    benchmark6();
}
