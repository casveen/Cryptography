#ifndef ROTORS_H   // include guard
#define ROTORS_H
#include "enigma.h"   //is guarded, so is probably not inserted here

const Rotor         IC   = Rotor("DMTWSILRUYQNKFEJCAZBPGXOHV");
const Rotor         IIC  = Rotor("HQZGPJTMOBLNCIFDYAWVEUSRKX");
const Rotor         IIIC = Rotor("UQNTLSZFMREHDPXKIBVYGJCWOA");
const Rotor         IR   = Rotor("JGDQOXUSCAMIFRVTPNEWKBLZYH");
const Rotor         IIR  = Rotor("NTZPSFBOKMWRCJDIVLAEYUXHGQ");
const Rotor         IIIR = Rotor("JVIUBHTCDYAKEQZPOSGXNRMWFL");
const Reflector     UKWR = Reflector("QYHOGNECVPUZTFDJAXWMKISRBL");   // ref
const Rotor         ETWR = Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
const Rotor         IK   = Rotor("PEZUOHXSCVFMTBGLRINQJWAYDK");
const Rotor         IIK  = Rotor("ZOUESYDKFWPCIQXHMVBLGNJRAT");
const Rotor         IIIK = Rotor("EHRVXGAOBQUSIMZFLYNWKTPDJC");
const Reflector     UKWK = Reflector("IMETCGFRAYSQBZXWLHKDVUPOJN");   // ref
const Rotor         ETWK = Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
const Rotor         I    = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
const Rotor         II   = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
const Rotor         III  = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
const Rotor         IV   = Rotor("ESOVPZJAYQUIRHXLNFTGKDCMWB", "J");
const Rotor         V    = Rotor("VZBRGITYUPSDNHLXAWMJQOFECK", "Z");
const Rotor         VI   = Rotor("JPGVOUMFYQBENHZRDKASXLICTW", "ZM");
const Rotor         VII  = Rotor("NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM");
const Rotor         VIII = Rotor("FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM");
const Rotor         Beta = Rotor("LEYJVCNIXWPBQMDRTAKZGFUHOS");
const Rotor         Gamma= Rotor("FSOKANUERHMBTIYCWLQPZXVGJD");
const Reflector     ReflectorA    = Reflector("EJMZALYXVBWFCRQUONTSPIKHGD");
const Reflector     ReflectorB    = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");
const Reflector     ReflectorC    = Reflector("FVPJIAOYEDRZXWGCTKUQSBNMHL");
const Reflector     ThinReflectorB= Reflector("ENKQAUYWJICOPBLMDXZVFTHRGS");
const Reflector     ThinReflectorC= Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
const Rotor         ETW           = Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const vector<Rotor> allRotors     = {IC,
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
const map<string, Rotor> rotorMap = {{"IC", IC},
                                    {"IIC", IIC},
                                    {"IIIC", IIIC},
                                    {"IR", IR},
                                    {"IIR", IIR},
                                    {"IIIR", IIIR},
                                    {"UKWR", UKWR},
                                    {"ETWR", ETWR},
                                    {"IK", IK},
                                    {"IIK", IIK},
                                    {"IIIK", IIIK},
                                    {"UKWK", UKWK},
                                    {"ETWK", ETWK},
                                    {"I", I},
                                    {"II", II},
                                    {"III", III},
                                    {"IV", IV},
                                    {"V", V},
                                    {"VI", VI},
                                    {"VII", VII},
                                    {"VIII", VIII},
                                    {"beta", Beta},
                                    {"gamma", Gamma},
                                    {"reflectorA", ReflectorA},
                                    {"reflectorB", ReflectorB},
                                    {"reflectorC", ReflectorC},
                                    {"ThinReflectorB", ThinReflectorB},
                                    {"ThinReflectorC", ThinReflectorC},
                                    {"ETW", ETW}}
#endif
