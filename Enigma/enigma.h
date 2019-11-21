#ifndef ENIGMA_H // include guard
#define ENIGMA_H

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

class Wheel {
    protected:
        int* _wiring_in, *_wiring_out; //index i goes to value at index i
        int  _wires;
        int  _num;

    public:
        Wheel();
        Wheel(int wires);
        Wheel(Wheel const& copy);
        Wheel& operator=(Wheel rhs);
        void swap(Wheel& s) noexcept;
        ~Wheel();
        int  get_wires();
        int* get_wiring_in();
        int* get_wiring_out();
        int  get_wiring_in(int i);
        int  get_wiring_out(int i);
        void randomize();
        void print();
        int* make_inverse(int* in, int n);
};

class Reflector: public Wheel{
    public:
        Reflector();
        Reflector(int wires);
        void randomize();
};

class Cartridge {
    private:
        Wheel    **_wheels;
        Reflector *_reflector;
        int        _wheel_count, _wires, _reflector_position; //wires?
        int       *_positions;

    public:
        Cartridge(); //XXX stupitt
        Cartridge(int wheel_count, int wires); //CONSTRUCTOR, random wheels
        Cartridge(Cartridge const& copy);
        Cartridge& operator=(Cartridge rhs);
        void swap(Cartridge& s) noexcept;
        ~Cartridge();
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
        //make cartridge from file, using standard coding A->0, B->1, ...english letters
        /*syntax
        rotors:NUMBER_OF_WHEELS(excl reflector) wires:NUMBER OF WIRES(here, 26)
        ABCDEFGHIJKLMNOPQRSTUVWXYZ   //first wheel, pass-through in this case
        BADCFEHGJILKNMPORQTSVUXWZY   //second wheel,
        ...
        --.--                        //last wheel, must be a reflector, ie symmetric
        */
        static Cartridge from_file(char* filename) {
            Cartridge out;
            ssize_t    read;
            int wheels_number, wires;
            char * line=NULL;
            size_t len=0;
            FILE*  file = fopen(filename, "r");
            if (file==NULL) {
                printf("ERROR: Opening file %s failed", filename);
                return out;
            }
            //read first line to get number of wheels and wires
            read = getline(&line, &len, file);
            sscanf(line, "rotors:%d wires:%d", &wheels_number, &wires);
            Wheel* wheels=(Wheel*) malloc(wheels_number*sizeof*wheels);
            //read in the rotors
            int* wiring=(int*) malloc(wires*sizeof*wiring);
            while ((read = getline(&line, &len, file)) != -1) {
                Wheel wheel=Wheel(wires);
                for (int i=0; i<len; i++) {
                    wheel.get_wiring_in()[i]=(int) line[i]-(int) 'A';
                    wheel.get_wiring_out()[(int) line[i]-(int) 'A']=i;
                }
                wheel.print();
            }
            return out;
        }
};

class Enigma {
    private:
        Cartridge* _cartridge;
        int _wheels_number, _wires;

    public:
        Enigma(int wheels_number, int wires);
        ~Enigma();
        void set_coder();
        void reset();
        void randomize();
        int encrypt(int m);
        int* encrypt(int* m, int n);
        void print_positions();
        void print();
        //FACTORY
        static Enigma make_random_enigma(int wheels, int wires) {
            cout<<"init enigma\n";
            Enigma *enigma;
            enigma=new Enigma(wheels, wires);
            cout<<"randomize enigma\n";
            enigma->randomize();
            cout<<"return enigma\n";
            return *enigma;
        }
};

#endif
