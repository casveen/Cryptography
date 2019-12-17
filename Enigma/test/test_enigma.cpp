// main() provided by Catch in file test.cpp.
#include "catch.hpp"
#include "enigma.hpp"
#include "rotors.cpp" //all rotors,

const int MAX_TESTS = 100;
const int MESSAGE_SIZE = 26 * 10;
const int WIRES = 26;
const int WHEELS = 4;

TEST_CASE("Testing wiring of all known commonly used rotors") {
  // cout << "HHHHHHHHHH\n";
  for (Rotor rotor : ALLROTORS) {
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
    // allocate c, r
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

TEST_CASE("Testing turning of wheels on condrete example") {
  Enigma enigma = Enigma({I, II, III}, UKWR);
  // example from http://users.telenet.be/d.rijmenants/en/enigmatech.htm
  enigma.set_ring_setting("AAA");
  enigma.set_rotor_position("KDO");
  vector<string> correct_step_sequence = {"KDO", "KDP", "KDQ", "KER",
                                          "LFS", "LFT", "LFU"};
  int sequence_length = correct_step_sequence.size();
  vector<string> my_step_sequence;
  my_step_sequence.push_back(enigma.get_rotor_position_as_string());
  for (int t = 0; t < sequence_length - 1; ++t) {
    enigma.turn();
    my_step_sequence.push_back(enigma.get_rotor_position_as_string());
  }
  // compare
  for (int i = 0; i < sequence_length; ++i) {
    REQUIRE(correct_step_sequence[i] == my_step_sequence[i]);
  }
}

TEST_CASE("Testing turning of wheels on condrete example 2") {
  Enigma enigma = Enigma({III, II, I}, UKWR);
  enigma.set_ring_setting("AAA");
  enigma.set_rotor_position("ADU");
  vector<string> correct_step_sequence = {"ADU", "ADV", "AEW", "BFX", "BFY"};
  int sequence_length = correct_step_sequence.size();
  vector<string> my_step_sequence;
  my_step_sequence.push_back(enigma.get_rotor_position_as_string());
  for (int t = 0; t < sequence_length - 1; ++t) {
    enigma.turn();
    my_step_sequence.push_back(enigma.get_rotor_position_as_string());
  }
  // compare
  for (int i = 0; i < sequence_length; ++i) {
    REQUIRE(correct_step_sequence[i] == my_step_sequence[i]);
  }
}

TEST_CASE("Testing encryption of some conscrete examples") {
  Enigma enigma = Enigma({I, II, III}, UKWR);
  // example from http://users.telenet.be/d.rijmenants/en/enigmatech.htm
  enigma.set_ring_setting("AAA");
  enigma.set_rotor_position("KDO");
  vector<string> correct_step_sequence = {"KDO", "KDP", "KDQ", "KER",
                                          "LFS", "LFT", "LFU"};
  int sequence_length = correct_step_sequence.size();
  vector<string> my_step_sequence;
  my_step_sequence.push_back(enigma.get_rotor_position_as_string());
  for (int t = 0; t < sequence_length - 1; ++t) {
    enigma.turn();
    my_step_sequence.push_back(enigma.get_rotor_position_as_string());
    cout << enigma.get_rotor_position_as_string() << "\n";
  }
  // compare
  for (int i = 0; i < sequence_length; ++i) {
    REQUIRE(correct_step_sequence[i] == my_step_sequence[i]);
  }
  /* unturn
  for (int t= sequence_length - 2; t > 0; --t) {
      enigma.turn(-1);
      my_step_sequence[t]= enigma.get_rotor_position_as_string();
      cout << enigma.get_rotor_position_as_string() << "\n";
  }
  // compare
  for (int i= 0; i < sequence_length; ++i) {
      REQUIRE(correct_step_sequence[i] == my_step_sequence[i]);
  }*/
}

/*
TEST_CASE("Testing unturning of wheels with lots of variations") {
    Enigma         enigma       = Enigma({I, VI, II}, UKWR);
    vector<string> ring_settings= {"AAA", "VSK", "QOO", "APE", "ASL", "UUE",
                                   "CPE", "SSS", "BAR", "EUR", "AKJ", "WLD"};
    vector<string> step_sequence;
    enigma.set_ring_setting("AAA");
    enigma.set_rotor_position("AAA");
    string previous= "AAA";
    for (string ring_setting : ring_settings) {
        previous= enigma.get_ring_setting_as_string();
        enigma.set_ring_setting(ring_setting);
        enigma.set_rotor_position(previous);
        for (int t= 0; t < MAX_TESTS * 10; ++t) {
            enigma.turn();
            step_sequence.push_back(enigma.get_rotor_position_as_string());
        }
        for (int t= 0; t < MAX_TESTS * 10; ++t) {
            REQUIRE(enigma.get_rotor_position_as_string() ==
                    step_sequence[MAX_TESTS * 10 - t - 1]);
            enigma.turn(-1);
        }
        step_sequence.clear();
    }
}
*/
/*
TEST_CASE(
    "Testing unturning of wheels with lots of variations and many rotors") {
    Enigma         enigma       = Enigma({I, VI, II, IV, V, VIII, VII}, UKWR);
    vector<string> ring_settings= {"AAWLXVA", "VSWSODK", "QDLEPOO", "WAXVSPE",
                                   "ASWBELF", "UUERDTE", "CPESSJS", "SXDWBSS",
                                   "BAWUERQ", "EXVQCUR", "AKELKRJ", "WERBDLD"};
    vector<string> step_sequence;
    enigma.set_ring_setting("AAAAAAA");
    enigma.set_rotor_position("AAAAAAA");
    for (string ring_setting : ring_settings) {
        enigma.set_rotor_position(
            enigma.get_ring_setting_as_string());   // semi-random
        enigma.set_ring_setting(ring_setting);
        for (int t= 0; t < MAX_TESTS * 100; ++t) {
            cout << enigma.get_rotor_position_as_string() << " -T-> ";
            enigma.turn();
            cout << enigma.get_rotor_position_as_string() << "\n";
            step_sequence.push_back(enigma.get_rotor_position_as_string());
        }
        for (int t= 0; t < MAX_TESTS * 100; ++t) {

            REQUIRE(enigma.get_rotor_position_as_string() ==
                    step_sequence[MAX_TESTS * 100 - t - 1]);
            cout << enigma.get_rotor_position_as_string() << " -U-> ";
            enigma.turn(-1);
            cout << enigma.get_rotor_position_as_string() << "\n";
        }
        step_sequence.clear();
    }
}
*/
TEST_CASE("Testing a concrete example; encryption of a sample of the Donitz "
          "message") {
  // test if the enigma can encrypt/decrypt a portion of the Karl donitz
  // message
  // Reflector ThinReflectorC= Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
  Enigma enigma({VIII, VI, V, BETA}, THINREFLECTORC);
  enigma.set_plugboard("AE.BF.CM.DQ.HU.JN.LX.PR.SZ.VW");
  enigma.set_ring_setting("EPEL");
  enigma.set_rotor_position("CDSZ");
  string encryption = enigma.encrypt(
      "LANOTCTOUARBBFPMHPHGCZXTDYGAHGUFXGEWKBLKGJWLQXXTGPJJAVTOCKZFSLPPQIHZFX"
      "OEBWIIEKFZLCLOAQJULJOYHSSMBBGWHZANVOIIPYRBRTDJQDJJOQKCXWDNBBTYVXLYTAPG"
      "VEATXSONPNYNQFUDBBHHVWEPYEYDOHNLXKZDNWRHDUWUJUMWWVIIWZXIVIUQDRHYMNCYEF"
      "UAPNHOTKHKGDNPSAKNUAGHJZSMJBMHVTREQEDGXHLZWIFUSKDQVELNMIMITHBHDBWVHDFY"
      "HJOQIHORTDJDBWXEMEAYXGYQXOHFDMYUXXNOJAZRSGHPLWMLRECWWUTLRTTVLBHYOORGLG"
      "OWUXNXHMHYFAACQEKTHSJW");
  REQUIRE(encryption ==
          "KRKRALLEXXFOLGENDESISTSOFORTBEKANNTZUGEBENXXICHHABEFOLGELNBEBEFEHL"
          "ERHALTENXXJANSTERLEDESBISHERIGXNREICHSMARSCHALLSJGOERINGJSETZTDERF"
          "UEHRERSIEYHVRRGRZSSADMIRALYALSSEINENNACHFOLGEREINXSCHRIFTLSCHEVOLL"
          "MACHTUNTERWEGSXABSOFORTSOLLENSIESAEMTLICHEMASSNAHMENVERFUEGENYDIES"
          "ICHAUSDERGEGENWAERTIGENLAGEERGEBENXGEZXREICHSLEITEIKKTULPEKKJBORMA"
          "NNJXXOBXDXMMMDURNHFKSTXKOMXADMXUUUBOOIEXKP");
}

/*
string ciphertext=
"LANOTCTOUARBBFPMHPHGCZXTDYGAHGUFXGEWKBLKGJWLQXXTGPJJAVTOCKZFSLPPQIHZFXOEBWIIEKFZLCLOAQJULJOYHSSMBBGWHZANVOIIPYRBRTDJQDJJOQKCXWDNBBTYVXLYTAPGVEATXSONPNYNQFUDBBHHVWEPYEYDOHNLXKZDNWRHDUWUJUMWWVIIWZXIVIUQDRHYMNCYEFUAPNHOTKHKGDNPSAKNUAGHJZSMJBMHVTREQEDGXHLZWIFUSKDQVELNMIMITHBHDBWVHDFYHJOQIHORTDJDBWXEMEAYXGYQXOHFDMYUXXNOJAZRSGHPLWMLRECWWUTLRTTVLBHYOORGLGOWUXNXHMHYFAACQEKTHSJW"

RBBFPMHPHGCZXTDYGAHGUFXGEWKBLKGJWLQXXTGPJJAVTOCKZFSLPPQIHZFXOEBWIIEKFZLCLOAQJULJOYHSSMBBGWHZANVOIIPYRBRTDJQDJJOQKCXWDNBBTYVXLYTAPGVEATXSONPNYNQFUDBBHHVWEPYEYDOHNLXKZDNWRHDUWUJUMWWVIIWZXIVIUQDRHYMNCYEFUAPNHOTKHKGDNPSAKNUAGHJZSMJBMHVTREQEDGXHLZWIFUSKDQVELNMIMITHBHDBWVHDFYHJOQIHORTDJDBWXEMEAYXGYQXOHFDMYUXXNOJAZRSGHPLWMLRECWWUTLRTTVLBHYOORGLGOWUXNXHMHYFAACQEKTHSJW

KRKRALLEXXFOLGENDESISTSOFORTBEKANNTZUGEBENXXICHHABEFOLGELNBEBEFEHLERHALTENXXJANSTERLEDESBISHERIGXNREICHSMARSCHALLSJGOERINGJSETZTDERFUEHRERSIEYHVRRGRZSSADMIRALYALSSEINENNACHFOLGEREINXSCHRIFTLSCHEVOLLMACHTUNTERWEGSXABSOFORTSOLLENSIESAEMTLICHEMASSNAHMENVERFUEGENYDIESICHAUSDERGEGENWAERTIGENLAGEERGEBENXGEZXREICHSLEITEIKKTULPEKKJBORMANNJXXOBXDXMMMDURNHFKSTXKOMXADMXUUUBOOIEXKP
*/
