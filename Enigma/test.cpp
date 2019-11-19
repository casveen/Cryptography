#include "enigma.h"
using namespace std;



const int MAX_TESTS=1;
const int MESSAGE_SIZE=1000;
const int WIRES =100;
const int WHEELS=100;


//test if wires cross correctly
bool test_wiring() {
    Wheel wheel=Wheel::make_random_wheel(WIRES);
    //test if wires cross correctly
    for (int w=0; w<WIRES; w++) {
        if (wheel.get_wiring(wheel.get_wiring(w))!=w or wheel.get_wiring(w)==w)
            return false;
    }
    return true;
}

//test if enigma encryption is symmetric,
//ie encrypting a letter twice should give same letter
bool test_encryption_symmetry() {
    Enigma enigma=Enigma::make_random_enigma(WHEELS, WIRES);
    int m, c, r;
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
    Enigma enigma=Enigma::make_random_enigma(WHEELS, WIRES);
    //make random message
    int* m=(int*) malloc(MESSAGE_SIZE*sizeof*m);
    for (int i=0; i<MESSAGE_SIZE; i++) { m[i]=rand()%WIRES; }
    int* c=enigma.encrypt(m, MESSAGE_SIZE); //ciphertext
    enigma.reset();          //reset
    int* r=enigma.encrypt(c, MESSAGE_SIZE); //recrypted
    //r should be m
    for (int i=0; i<MESSAGE_SIZE; i++) {
        if (m[i]!=r[i]) {
            printf("m[%2d]=%2d   =/=   %2d=encrypt(encrypt(m[%2d]))\n", i, m[i], r[i], i);
            return false;
        }
    }
    return true;
}


int main() {
    //TEST WIRES
    /*
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
    */

    //TEST CARTRIDGE
    success=true;
    for (int t=0; t<MAX_TESTS; t++) {
        cout<<t<<"\n";
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
    if (success)
        printf("\rDECRYPTION WORKS                      \n");
}
