#ifndef ENIGMA_H // include guard
#define ENIGMA_H

#include <iostream>
#include <stdlib.h>
#include <time.h>

class wheel {
    private:
        int* _wiring; //index i goes to value at index i
        int  _wires;
    public:
        int  get_wires();
        int* get_wiring();
        int get_wiring(int i);
        void randomize();
        void print();
        //static Wheel make_random_wheel(int wires);
};

class Cartridge;
class Enigma;

#endif
