// make some performance runs, recording performance for a bad crib
//#include "bombe.h"
#include "bombe.h"
#include "enigma.h"
#include "rotors.cpp"   //all rotors,

int main() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({I, II, III}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = 1;
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
}
