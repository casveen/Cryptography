// make some performance runs, recording performance for a bad crib
//#include "bombe.h"
#include "bombe.hpp"
#include "enigma.hpp"
#include "rotors.cpp"   //all rotors,

int main() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({I, II, VII, III}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AA";
    bombe.get_setting().starting_ring_setting   = "AA";
    bombe.get_setting().max_ring_settings       = 3;
    bombe.get_setting().rotor_count             = 2;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, II}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AA");
    enigma.set_ring_setting("AA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions = bombe.analyze(ciphertext, "SOMEUNRELATEDTEXT");
}
