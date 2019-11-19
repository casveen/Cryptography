#ifndef ENIGMA_H // include guard
#define ENIGMA_H

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <time.h>

class Wheel {
    protected:
        int* _wiring; //index i goes to value at index i
        int  _wires;
    public:
        Wheel();
        Wheel(int wires);
        int  get_wires();
        int* get_wiring();
        int get_wiring(int i);
        void randomize();
        void print();
        static Wheel make_random_wheel(int wires) {
            Wheel wheel=Wheel(wires);
            wheel.randomize();
            return wheel;
        }
};

class Reflector: public Wheel{
    public:
        Reflector();
        Reflector(int wires);
        void randomize();
        static Reflector make_random_reflector(int wires) {
            Reflector reflector=Reflector(wires);
            cout<<"randomizing reflector\n";
            reflector.randomize();
            cout<<"reflector randomized\n";
            return reflector;
        }
};

class Cartridge {
    private:
        Wheel*     _wheels;
        Reflector* _reflector;
        int        _wheel_count, _wires, _reflector_position; //wires?
        int*       _positions;

    public:
        Cartridge(); //XXX stupitt
        Cartridge(int wheel_count, int wires); //CONSTRUCTOR, random wheels
        void reset_positions();
        void set_positions(int p);
        void set_positions(int* p);
        int* get_positions();
        int  get_positions_as_int();
        void turn(int t);
        void turn(); //overloaded, single turn
        int encrypt(int i); //pass integer through wires without turning
        void print(); //PRINT cartridge
        void print_positions(); //print positions of the wheels
        void randomize();
        Wheel* make_random_wheels(int n, int wires); //make array of n random wheels
        //FACTORY
        static Cartridge make_random_cartridge(int wheels, int wires) {
            Cartridge cartridge=Cartridge(wheels, wires);
            return cartridge;
        }
};

class Enigma {
    private:
        Cartridge* _cartridge;
        int _wheels_number, _wires;

    public:
        Enigma(int wheels_number, int wires);
        void set_coder();
        void reset();
        void randomize();
        int encrypt(int m);
        int* encrypt(int* m, int n);
        void print_positions();
        void print();
        //FACTORY
        static Enigma make_random_enigma(int wheels, int wires) {
            Enigma enigma=Enigma(wheels, wires);
            enigma.randomize();
            return enigma;
        }
};

#endif
