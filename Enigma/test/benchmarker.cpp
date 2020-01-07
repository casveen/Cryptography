// make some performance runs, recording performance for a bad crib
//#include "bombe.h"
int MAX_RING_SETTINGS= 50;
#include "bombe.hpp"
#include "enigma.hpp"
#include "rotors.cpp"   //all rotors,

struct BombeSetting benchmark1() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({I, VII, III}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAA");
    enigma.set_ring_setting("AAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions = bombe.analyze(ciphertext, "SOMETEXT");
    return bombe.get_setting();
}

struct BombeSetting benchmark2() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({II, III, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAA");
    enigma.set_ring_setting("AAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions = bombe.analyze(ciphertext, "SOMEUNRELATEDTEXT");
    return bombe.get_setting();
}

struct BombeSetting benchmark3() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({III, IV, V}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAA");
    enigma.set_ring_setting("AAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXTANDSOMEMORE");
    return bombe.get_setting();
}

struct BombeSetting benchmark4() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({IV, V, VI}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAA";
    bombe.get_setting().starting_ring_setting   = "AAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAA");
    enigma.set_ring_setting("AAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXTANDSOMEMOREUNRELATEDTEXTTHATISUNRELATED");
    return bombe.get_setting();
}

struct BombeSetting benchmark5() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({I, VII, VI, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAAA";
    bombe.get_setting().starting_ring_setting   = "AAAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI, I}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAAA");
    enigma.set_ring_setting("AAAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions = bombe.analyze(ciphertext, "SOMETEXT");
    return bombe.get_setting();
}

struct BombeSetting benchmark6() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({I, II, III, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAAA";
    bombe.get_setting().starting_ring_setting   = "AAAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI, I}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAAA");
    enigma.set_ring_setting("AAAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions = bombe.analyze(ciphertext, "SOMEUNRELATEDTEXT");
    return bombe.get_setting();
}

struct BombeSetting benchmark7() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({I, II, III, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAAA";
    bombe.get_setting().starting_ring_setting   = "AAAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({IV, V, VI, I}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYLONG";
    enigma.set_rotor_position("AAAA");
    enigma.set_ring_setting("AAAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXTANDDOMEMORE");
    return bombe.get_setting();
}

struct BombeSetting benchmark8() {
    // make up a setting and use bombe on something completely unrelated,
    // record the performance
    Bombe bombe({I, II, III, IV}, UKWR);
    bombe.get_setting().starting_rotor_positions= "AAAA";
    bombe.get_setting().starting_ring_setting   = "AAAA";
    bombe.get_setting().max_ring_settings       = MAX_RING_SETTINGS;
    bombe.get_setting().only_one_candidate      = true;
    bombe.get_setting().only_one_configuration  = true;
    bombe.get_setting().stop_on_first_valid     = false;
    // bombe.get_setting().time_performance        = true;
    // bombe.get_setting().stop_on_first_valid= true;
    Enigma enigma({I, VII, VI, II}, UKWR);
    // enigma.set_verbose(true);
    string plaintext= "THISISAPLAINTEXTTOBEENCRYPTEDWITHTHEENIGMAANDISVERYVERYVERYLONG";
    //"SOMEUNRELATEDTEXTANDSOMEMOREUNRELATEDTEXTTHATISUNRELATED"
    enigma.set_rotor_position("AAAA");
    enigma.set_ring_setting("AAAA");
    enigma.set_plugboard("");
    string                       ciphertext= enigma.encrypt(plaintext);
    vector<struct EnigmaSetting> solutions=
        bombe.analyze(ciphertext, "SOMEUNRELATEDTEXTANDSOMEMOREUNRELATEDTEXTTHATSUNRELATED");
    return bombe.get_setting();
}

int main() {
    printf("----------------------------------------------------------\n");
    printf("|                          MEAN       VAR        RECORDS |\n");
    struct BombeSetting setting1= benchmark1();
    printf("| 3 ROTOR SMALL CRIB       %6.2E   %6.2E   %7d |\n",
           setting1.performance_ring_setting_mean, setting1.performance_ring_setting_var,
           setting1.records_ring_setting);
    struct BombeSetting setting2= benchmark2();
    printf("| 3 ROTOR MEDIUM CRIB      %6.2E   %6.2E   %7d |\n",
           setting2.performance_ring_setting_mean, setting2.performance_ring_setting_var,
           setting2.records_ring_setting);
    struct BombeSetting setting3= benchmark3();
    printf("| 3 ROTOR LARGE CRIB       %6.2E   %6.2E   %7d |\n",
           setting3.performance_ring_setting_mean, setting3.performance_ring_setting_var,
           setting3.records_ring_setting);
    struct BombeSetting setting4= benchmark4();
    printf("| 3 ROTOR VERY LARGE CRIB  %6.2E   %6.2E   %7d |\n",
           setting4.performance_ring_setting_mean, setting4.performance_ring_setting_var,
           setting4.records_ring_setting);
    struct BombeSetting setting5= benchmark5();
    printf("| 4 ROTOR SMALL CRIB       %6.2E   %6.2E   %7d |\n",
           setting5.performance_ring_setting_mean, setting5.performance_ring_setting_var,
           setting5.records_ring_setting);
    struct BombeSetting setting6= benchmark6();
    printf("| 4 ROTOR MEDIUM CRIB      %6.2E   %6.2E   %7d |\n",
           setting6.performance_ring_setting_mean, setting6.performance_ring_setting_var,
           setting6.records_ring_setting);
    struct BombeSetting setting7= benchmark7();
    printf("| 4 ROTOR LARGE CRIB       %6.2E   %6.2E   %7d |\n",
           setting7.performance_ring_setting_mean, setting7.performance_ring_setting_var,
           setting7.records_ring_setting);
    struct BombeSetting setting8= benchmark8();
    printf("| 4 ROTOR VERY LARGE CRIB  %6.2E   %6.2E   %7d |\n",
           setting8.performance_ring_setting_mean, setting8.performance_ring_setting_var,
           setting8.records_ring_setting);
    printf("---------------------------------------------------\n");
}
