#ifndef ROTORS_H   // include guard
#define ROTORS_H
#include "enigma.hpp"   //is guarded, so is probably not inserted here

const Rotor     IC            = Rotor("DMTWSILRUYQNKFEJCAZBPGXOHV", "A", "IC");
const Rotor     IIC           = Rotor("HQZGPJTMOBLNCIFDYAWVEUSRKX", "A", "IIC");
const Rotor     IR            = Rotor("JGDQOXUSCAMIFRVTPNEWKBLZYH", "A", "IR");
const Rotor     IIIC          = Rotor("UQNTLSZFMREHDPXKIBVYGJCWOA", "A", "IIIC");
const Rotor     IIR           = Rotor("NTZPSFBOKMWRCJDIVLAEYUXHGQ", "A", "IIR");
const Rotor     IIIR          = Rotor("JVIUBHTCDYAKEQZPOSGXNRMWFL", "A", "IIIR");
const Reflector UKWR          = Reflector("QYHOGNECVPUZTFDJAXWMKISRBL", "", "UKWR");
const Rotor     ETWR          = Rotor("QWERTZUIOASDFGHJKPYXCVBNML", "", "ETWR");
const Rotor     IK            = Rotor("PEZUOHXSCVFMTBGLRINQJWAYDK", "A", "IK");
const Rotor     IIK           = Rotor("ZOUESYDKFWPCIQXHMVBLGNJRAT", "A", "IIK");
const Rotor     IIIK          = Rotor("EHRVXGAOBQUSIMZFLYNWKTPDJC", "A", "IIIK");
const Reflector UKWK          = Reflector("IMETCGFRAYSQBZXWLHKDVUPOJN", "", "UKWK");   // ref
const Rotor     ETWK          = Rotor("QWERTZUIOASDFGHJKPYXCVBNML", "", "ETWK");
const Rotor     I             = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q", "I");
const Rotor     II            = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E", "II");
const Rotor     III           = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V", "III");
const Rotor     IV            = Rotor("ESOVPZJAYQUIRHXLNFTGKDCMWB", "J", "IV");
const Rotor     V             = Rotor("VZBRGITYUPSDNHLXAWMJQOFECK", "Z", "V");
const Rotor     VI            = Rotor("JPGVOUMFYQBENHZRDKASXLICTW", "ZM", "VI");
const Rotor     VII           = Rotor("NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM", "VII");
const Rotor     VIII          = Rotor("FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM", "VIII");
const Rotor     BETA          = Rotor("LEYJVCNIXWPBQMDRTAKZGFUHOS", "", "BETA");
const Rotor     GAMMA         = Rotor("FSOKANUERHMBTIYCWLQPZXVGJD", "", "GAMMA");
const Reflector REFLECTORA    = Reflector("EJMZALYXVBWFCRQUONTSPIKHGD", "", "REFLECTORA");
const Reflector REFLECTORB    = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT", "", "REFELCTORB");
const Reflector REFLECTORC    = Reflector("FVPJIAOYEDRZXWGCTKUQSBNMHL", "", "REFLECTORC");
const Reflector THINREFLECTORB= Reflector("ENKQAUYWJICOPBLMDXZVFTHRGS", "", "THINREFLECTORB");
const Reflector THINREFLECTORC= Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ", "", "THINREFELCTORC");
const Rotor     ETW           = Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "", "ETW");
// VECTORS
const vector<Rotor> ALLROTORS= {IC, IIC, IIIC, IR,   IIR,  IIIR, UKWR,  ETWR,
                                IK, IIK, IIIK, UKWK, ETWK, I,    II,    III,
                                IV, V,   VI,   VII,  VIII, BETA, GAMMA, ETW};

const vector<Reflector> ALLREFLECTORS= {REFLECTORA,     REFLECTORB, REFLECTORC, THINREFLECTORB,
                                        THINREFLECTORC, UKWK,       UKWR};

// MAPS
const map<string, pair<Rotor, string>> COMMERCIALROTORMAP= {
    {"IC", {IC, "Commercial I"}},
    {"IIC", {IIC, "Commercial II"}},
    {"IIIC", {IIIC, "Commercial III"}},
    {"ETW", {ETWR, "Commercial Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> ROCKETROTORMAP= {
    {"IR", {IR, "German Railway(Rocket) I"}},
    {"IIR", {IIR, "German Railway(Rocket) II"}},
    {"IIIR", {IIIR, "German Railway(Rocket) III"}},
    {"ETWR", {ETWR, "German Railway(Rocket) Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> SWISSKROTORMAP= {
    {"IK", {IK, "Swiss K I"}},
    {"IIK", {IIK, "Swiss K II"}},
    {"IIIK", {IIIK, "Swiss K III"}},
    {"ETWK", {ETWK, "Swiss K Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> KRIEGSMARINEROTORMAP= {
    {"I", {I, "Wehrmacht/Kriegsmarine I"}},
    {"II", {II, "Wehrmacht/Kriegsmarine II"}},
    {"III", {III, "Wehrmacht/Kriegsmarine III"}},
    {"IV", {IV, "Wehrmacht/Kriegsmarine IV"}},
    {"V", {V, "Wehrmacht/Kriegsmarine V"}},
    {"VI", {VI, "Kriegsmarine VI"}},
    {"VII", {VII, "Kriegsmarine VII"}},
    {"VIII", {VIII, "Kriegsmarine VIII"}},
    {"BETA", {BETA, "Kriegsmarine Greek Rotor(Zusatswalzen) Beta"}},
    {"GAMMA", {GAMMA, "Kriegsmarine Greek Rotor(Zusatswalzen) Gamma"}},
    {"ETW", {ETWR, "Standard Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> WEHRMACHTROTORMAP= {
    {"I", {I, "Wehrmacht/Kriegsmarine I"}},
    {"II", {II, "Wehrmacht/Kriegsmarine II"}},
    {"III", {III, "Wehrmacht/Kriegsmarine III"}},
    {"IV", {IV, "Wehrmacht/Kriegsmarine IV"}},
    {"V", {V, "Wehrmacht/Kriegsmarine V"}},

    {"ETW", {ETWR, "Standard Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> ALLROTORMAP= {
    {"IC", {IC, "Commercial I"}},
    {"IIC", {IIC, "Commercial II"}},
    {"IIIC", {IIIC, "Commercial III"}},
    {"IR", {IR, "German Railway(Rocket) I"}},
    {"IIR", {IIR, "German Railway(Rocket) II"}},
    {"IIIR", {IIIR, "German Railway(Rocket) III"}},
    {"ETWR", {ETWR, "German Railway(Rocket) Entry Wheel(Eintrittswalze)"}},
    {"IK", {IK, "Swiss K I"}},
    {"IIK", {IIK, "Swiss K II"}},
    {"IIIK", {IIIK, "Swiss K III"}},
    {"ETWK", {ETWK, "Swiss K Entry Wheel(Eintrittswalze)"}},
    {"I", {I, "Wehrmacht/Kriegsmarine I"}},
    {"II", {II, "Wehrmacht/Kriegsmarine II"}},
    {"III", {III, "Wehrmacht/Kriegsmarine III"}},
    {"IV", {IV, "Wehrmacht/Kriegsmarine IV"}},
    {"V", {V, "Wehrmacht/Kriegsmarine V"}},
    {"VI", {VI, "Kriegsmarine VI"}},
    {"VII", {VII, "Kriegsmarine VII"}},
    {"VIII", {VIII, "Kriegsmarine VIII"}},
    {"BETA", {BETA, "Kriegsmarine Greek Rotor(Zusatswalzen) Beta"}},
    {"GAMMA", {GAMMA, "Kriegsmarine Greek Rotor(Zusatswalzen) Gamma"}},
    {"ETW", {ETW, "Standard Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Reflector, string>> ALLREFLECTORMAP= {
    {"UKWR", {UKWR, "German Railway(Rocket) Reflector(Umkehrwalze)"}},
    {"UKWK", {UKWK, "Swiss K Reflector(Umkehrwalze)"}},
    {"REFLECTORA", {REFLECTORA, "Wehrmacht/Luftwaffe Wide Reflector A"}},
    {"REFLECTORB", {REFLECTORB, "Wehrmacht/Luftwaffe Wide Reflector B"}},
    {"REFLECTORC", {REFLECTORC, "Wehrmacht/Luftwaffe Wide Reflector C"}},
    {"THINREFLECTORB", {THINREFLECTORB, "Kriegsmarine Thin Reflector B"}},
    {"THINREFLECTORC", {THINREFLECTORC, "Kriegsmarine Thin Reflector C"}}};

#endif
