#include "enigma.h"
using namespace std;

const int MAX_TESTS=1;
const int MESSAGE_SIZE=26*10;
const int WIRES =26;
const int WHEELS=4;

//test if wires cross correctly
bool test_wiring() {
    Rotor rotor=Rotor(WIRES);
    rotor.randomize();
    //test if wires cross correctly, in then out should return input.
    //in and out should hit all numbers, which is covered by the above test
    for (int w=0; w<WIRES; w++) {
        if (rotor.get_wiring_out(rotor.get_wiring_in(w))!=w) {
            return false;
        }
    }
    return true;
}

//test if enigma encryption is symmetric,
//ie encrypting a letter twice should give same letter
bool test_encryption_symmetry() {
    Enigma enigma=Enigma(WHEELS, WIRES);
    enigma.randomize();
    enigma.set_verbose(false);
    int c, r;
    for (int w=0; w<WIRES; w++) {
        c=enigma.encrypt(w);
        enigma.reset();
        r=enigma.encrypt(c);
        enigma.reset();
        if (r!=w or c==w)
            return false;
    }
    return true;
}

//test if encrypt(encrypt(m)) (with intermediate reset) returns m
bool test_decryption() {
    Enigma enigma=Enigma(WHEELS, WIRES);
    //make random message
    int* m=new int[MESSAGE_SIZE];
    for (int i=0; i<MESSAGE_SIZE; i++) { m[i]=rand()%WIRES; }
    int* c=enigma.encrypt(m, MESSAGE_SIZE); //ciphertext
    enigma.reset();          //reset
    int* r=enigma.encrypt(c, MESSAGE_SIZE); //recrypted
    //r should be m
    for (int i=0; i<MESSAGE_SIZE; i++) {
        if (m[i]!=r[i]) {
            printf("m[%2d]=%2d   =/=   %2d=encrypt(encrypt(m[%2d]))", i, m[i], r[i], i);
            delete [] m;
            delete [] c;
            delete [] r;
            return false;
        }
    }
    delete [] m;
    delete [] c;
    delete [] r;
    return true;
}

bool test_wikipedia_example() {
    //test if the enigma can encrypt/decrypt a portion of the Karl donitz message
    static Rotor V=                 Rotor("VZBRGITYUPSDNHLXAWMJQOFECK", "Z");
    static Rotor VI=                Rotor("JPGVOUMFYQBENHZRDKASXLICTW", "ZM");
    static Rotor VIII=              Rotor("FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM");
    static Rotor Beta=              Rotor("LEYJVCNIXWPBQMDRTAKZGFUHOS");
    static Reflector ThinReflectorC=Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
    Enigma enigma({VIII, VI, V, Beta}, ThinReflectorC);
    enigma.set_plugboard("AE.BF.CM.DQ.HU.JN.LX.PR.SZ.VW");
    enigma.set_rotor_position("ZPOY");
    enigma.set_ring_setting("KTDC");
    return (enigma.encrypt("RBBFPMHPHGCZXTDYGAHGUFXGEWKBLKGJ")=="FOLGENDESISTSOFORTBEKANNTZUGEBEN");
}

int main() {
    //Cartridge::from_file("rotors.txt");
    srand(time(NULL));

    //TEST WIRES
    //Rotor::count=0;
    bool success=true;
    for (int t=0; t<MAX_TESTS; t++) {
        printf("\rTESTING WIRING (%3.0f %%)", ((t+1)/(float) MAX_TESTS)*100);
        if (test_wiring()==false) {
            printf("\nIMPROPER WIRING IN SINGE WHEEL FOUND\n");
            success=false;
            break;
        }
    }
    if (success)
        printf("\rWIRES WORK                      \n");


    //TEST CARTRIDGE
    success=true;
    for (int t=0; t<MAX_TESTS; t++) {
        printf("\rTESTING CARTRIDGE TOTAL WIRING (%3.0f %%)", ((t+1)/(float) MAX_TESTS)*100);
        if (test_encryption_symmetry()==false) {
            printf("\nIMPROPER WIRING IN TOTAL CARTRIDGE\n");
            success=false;
            break;
        }
    }
    if (success)
        printf("\rCARTRIDGE WORKS                        \n");


    //TEST DECRYPTION
    success=true;
    for (int t=0; t<MAX_TESTS; t++) {
        printf("\rTESTING DECRYPTION (%3.0f %%)", ((t+1)/(float) MAX_TESTS)*100);
        if (test_decryption()==false) {
            printf("\nUNABLE TO DECRYPT\n");
            success=false;
            break;
        }
    }
    if (success) {
        printf("\rDECRYPTION WORKS                      \n");
    }


    //TEST EXAMPLE
    if (test_wikipedia_example()) {
        printf("\rENIGMA WORKS ON DONITZ SAMPLE         \n");
    }

        //print a known rotor
        //IC.print();

    /*
    static Rotor IC=                Rotor("DMTWSILRUYQNKFEJCAZBPGXOHV");
    static Rotor IIC=               Rotor("HQZGPJTMOBLNCIFDYAWVEUSRKX");
    static Rotor IIIC=              Rotor("UQNTLSZFMREHDPXKIBVYGJCWOA");
    static Rotor IR=                Rotor("JGDQOXUSCAMIFRVTPNEWKBLZYH");
    static Rotor IIR=               Rotor("NTZPSFBOKMWRCJDIVLAEYUXHGQ");
    static Rotor IIIR=              Rotor("JVIUBHTCDYAKEQZPOSGXNRMWFL");
    static Reflector UKWR=      Reflector("QYHOGNECVPUZTFDJAXWMKISRBL"); //ref
    static Rotor ETWR=              Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
    static Rotor IK=                Rotor("PEZUOHXSCVFMTBGLRINQJWAYDK");
    static Rotor IIK=               Rotor("ZOUESYDKFWPCIQXHMVBLGNJRAT");
    static Rotor IIIK=              Rotor("EHRVXGAOBQUSIMZFLYNWKTPDJC");
    static Reflector UKWK=      Reflector("IMETCGFRAYSQBZXWLHKDVUPOJN"); //ref
    static Rotor ETWK=              Rotor("QWERTZUIOASDFGHJKPYXCVBNML");
    static Rotor I=                 Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
    static Rotor II=                Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
    static Rotor III=               Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
    static Rotor IV=                Rotor("ESOVPZJAYQUIRHXLNFTGKDCMWB", "J");
    static Rotor V=                 Rotor("VZBRGITYUPSDNHLXAWMJQOFECK", "Z");
    static Rotor VI=                Rotor("JPGVOUMFYQBENHZRDKASXLICTW", "ZM");
    static Rotor VII=               Rotor("NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM");
    static Rotor VIII=              Rotor("FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM");
    static Rotor Beta=              Rotor("LEYJVCNIXWPBQMDRTAKZGFUHOS"); //XXXwhitespace for none?
    static Rotor Gamma=             Rotor("FSOKANUERHMBTIYCWLQPZXVGJD");
    static Rotor ReflectorA=        Rotor("EJMZALYXVBWFCRQUONTSPIKHGD");
    static Rotor ReflectorB=        Rotor("YRUHQSLDPXNGOKMIEBFZCWVJAT");
    static Rotor ReflectorC=        Rotor("FVPJIAOYEDRZXWGCTKUQSBNMHL");
    static Reflector ThinReflectorB=Reflector("ENKQAUYWJICOPBLMDXZVFTHRGS");
    static Reflector ThinReflectorC=Reflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
    static Rotor ETW=               Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    */
    //I.print();
    //Enigma enigma({I, II, III}, UKWR);
    //cout<<"encrypting\n";
    //enigma.print();
    //vector<int> encryption=enigma.get_encryption();
    //for (auto i : encryption) { cout<<i<<" "; }
    //cout<<"\n";
    //cout<<enigma.encrypt_without_turning(0);
    //cout<<"\n";
    //A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z
    //0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
    //for (auto i : {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}) { cout<<enigma.encrypt(i)<<" "; }
    //cout<<"\n";
    //Plugboard plugboard("AB. CD. EF. WQ", 26);     26-13=13
    //Enigma enigma({VIII, VI, V, Beta}, ThinReflectorC);
    //enigma.set_verbose(true);
    //enigma.set_plugboard("AE.BF.CM.DQ.HU.JN.LX.PR.SZ.VW");
    //enigma.set_rotor_position("ZPOY");
    //cout<<"ring set\n";
    //enigma.set_ring_setting("KTDC");
    //cout<<"rotor set\n";
    //enigma.print();
    //string str=enigma.encrypt("RBBFPMHPHGCZXTDYGAHGUFXGEWK");
    //cout<<str;
    //cout<<"begining indicator procedure\n";
    //cout<<enigma.indicator_procedure_WW2("DUHF", "TETO")<<"\n";
    //enigma.print_positions();KLMN


    /*
    //OK!
    enigma.set_rotor_position("ZPOY"); // 26 16 15 25
    enigma.set_ring_setting("KTDC");
    cout<<enigma.encrypt("RBBFPMHPHGCZXTDYGAHGUFXGE");
    //OK!
    enigma.set_rotor_position("CQOY"); // correst for wiki example
    enigma.set_ring_setting("NUDC");
    cout<<enigma.encrypt("GE");

    //RING POSITION NEEDS TO BE TAKEN INTO ACCOUNT FOR CORRECT STEPPING!
    //yes
    //read positions(the numbers in wiki example) directly
    //then insert ringsthellung for correct notching
    */
}
