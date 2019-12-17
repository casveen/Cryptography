#ifndef ROTORS_H // include guard
#define ROTORS_H
#include "enigma.hpp" //is guarded, so is probably not inserted here

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
const Rotor BETA = Rotor("LEYJVCNIXWPBQMDRTAKZGFUHOS");
const Rotor GAMMA = Rotor("FSOKANUERHMBTIYCWLQPZXVGJD");
const Reflector REFLECTORA = Reflector("EJMZALYXVBWFCRQUONTSPIKHGD");
const Reflector REFLECTORB = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");
const Reflector REFLECTORC = Reflector("FVPJIAOYEDRZXWGCTKUQSBNMHL");
const Reflector THINREFLECTORB = Reflector("ENKQAUYWJICOPBLMDXZVFTHRGS");
const Reflector THINREFLECTORC = Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
const Rotor ETW = Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
// VECTORS
const vector<Rotor> ALLROTORS = {IC, IIC, IIIC, IR,   IIR,  IIIR, UKWR,  ETWR,
                                 IK, IIK, IIIK, UKWK, ETWK, I,    II,    III,
                                 IV, V,   VI,   VII,  VIII, BETA, GAMMA, ETW};

const vector<Reflector> ALLREFLECTORS = {
    REFLECTORA,     REFLECTORB, REFLECTORC, THINREFLECTORB,
    THINREFLECTORC, UKWK,       UKWR};

// MAPS
const map<string, pair<Rotor, string>> COMMERCIALROTORMAP = {
    {"IC", {IC, "Commercial I"}},
    {"IIC", {IIC, "Commercial II"}},
    {"IIIC", {IIIC, "Commercial III"}},
    {"ETW", {ETWR, "Commercial Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> ROCKETROTORMAP = {
    {"IR", {IR, "German Railway(Rocket) I"}},
    {"IIR", {IIR, "German Railway(Rocket) II"}},
    {"IIIR", {IIIR, "German Railway(Rocket) III"}},
    {"ETWR", {ETWR, "German Railway(Rocket) Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> SWISSKROTORMAP = {
    {"IK", {IK, "Swiss K I"}},
    {"IIK", {IIK, "Swiss K II"}},
    {"IIIK", {IIIK, "Swiss K III"}},
    {"ETWK", {ETWK, "Swiss K Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> KRIEGSMARINEROTORMAP = {
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

const map<string, pair<Rotor, string>> WEHRMACHTROTORMAP = {
    {"I", {I, "Wehrmacht/Kriegsmarine I"}},
    {"II", {II, "Wehrmacht/Kriegsmarine II"}},
    {"III", {III, "Wehrmacht/Kriegsmarine III"}},
    {"IV", {IV, "Wehrmacht/Kriegsmarine IV"}},
    {"V", {V, "Wehrmacht/Kriegsmarine V"}},

    {"ETW", {ETWR, "Standard Entry Wheel(Eintrittswalze)"}}};

const map<string, pair<Rotor, string>> ALLROTORMAP = {
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

const map<string, pair<Reflector, string>> ALLREFLECTORMAP = {
    {"UKWR", {UKWR, "German Railway(Rocket) Reflector(Umkehrwalze)"}},
    {"UKWK", {UKWK, "Swiss K Reflector(Umkehrwalze)"}},
    {"REFLECTORA", {REFLECTORA, "Wehrmacht/Luftwaffe Wide Reflector A"}},
    {"REFLECTORB", {REFLECTORB, "Wehrmacht/Luftwaffe Wide Reflector B"}},
    {"REFLECTORC", {REFLECTORC, "Wehrmacht/Luftwaffe Wide Reflector C"}},
    {"THINREFLECTORB", {THINREFLECTORB, "Kriegsmarine Thin Reflector B"}},
    {"THINREFLECTORC", {THINREFLECTORC, "Kriegsmarine Thin Reflector C"}}};

#endif
