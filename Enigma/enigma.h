#ifndef ENIGMA_H // include guard
#define ENIGMA_H

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
/*
class Plugboard: public Reflector {
}*/



class Rotor {
    protected:
        int *m_wiring_in, *m_wiring_out, *m_notch; //index i goes to value at index i
        int m_wires, m_notches;
        int _num;
        bool m_verbose=false;

    public:
        Rotor();
        Rotor(int wires);
        Rotor(string); //construct from string, ABCDEFGHIJKLMNOPQRSTUVWXYZ etc
        Rotor(string, string);
        Rotor(Rotor const& copy);
        Rotor& operator=(Rotor rhs);
        void swap(Rotor& s) noexcept;
        ~Rotor();
        int  get_wires();
        int* get_wiring_in();
        int* get_wiring_out();
        int  get_wiring_in(int i);
        int  get_wiring_out(int i);
        int* get_notch();
        int  get_notch(int);
        int  get_notches();
        int  encrypt_in(int, int);
        int  encrypt_out(int, int);
        void set_verbose(int);
        void randomize();
        void print();
        void make_inverse(int* in, int* out, int n);
        bool is_valid();
};

class Reflector: public Rotor{
    public:
        Reflector();
        Reflector(int wires);
        void randomize();
        bool is_valid();
};

class Cartridge {
    private:
        Rotor    **m_rotors;
        Reflector *m_reflector;
        int        m_rotor_count, m_wires, m_reflector_position; //wires?
        int       *m_positions, *m_ring_setting; //ringscthellung, moves the notches in the wheels
        bool       m_verbose=false;
        int       *m_notch_position;

    public:
        Cartridge(); //XXX stupitt
        Cartridge(int rotor_count, int wires); //CONSTRUCTOR, random rotors
        Cartridge(Cartridge const& copy);
        Cartridge& operator=(Cartridge rhs);
        void swap(Cartridge& s) noexcept;
        ~Cartridge();
        Rotor** get_rotors();
        Reflector*  get_reflector();
        void reset_positions();
        void reset_ring_setting();
        //void set_positions(int p);
        void set_positions(int* p);
        void set_positions(string in);
        int* get_positions();
        int  get_positions_as_int();
        string get_positions_as_string();
        void set_ring_setting(int* p);
        void set_ring_setting(string in);
        int* get_ring_setting();
        //int  get_ring_setting_as_int();
        string get_ring_setting_as_string();
        void set_verbose(int);
        void turn(int t);
        void turn(); //overloaded, single turn
        int encrypt(int i); //pass integer through wires without turning
        void print(); //PRINT cartridge
        void print_positions(); //print positions of the rotors
        void randomize();
        Rotor* make_random_rotors(int n, int wires); //make array of n random rotors
        //make cartridge from file, using standard coding A->0, B->1, ...english letters
        /*syntax
        rotors:NUMBER_OF_WHEELS(excl reflector) wires:NUMBER OF WIRES(here, 26)
        ABCDEFGHIJKLMNOPQRSTUVWXYZ   //first rotor, pass-through in this case
        BADCFEHGJILKNMPORQTSVUXWZY   //second rotor,
        ...
        --.--                        //last rotor, must be a reflector, ie symmetric
        */
        static Cartridge* from_file(const char* filename) {
            int rotors_number, wires, k=0, wire;
            string line;
            int len=0;
            ifstream file(filename);
            if (!file) {
                printf("ERROR: Opening file %s failed", filename);
                return nullptr;
            }
            //read first line to get number of rotors and wires
            getline(file, line);
            sscanf(line.c_str(), "rotors:%d wires:%d", &rotors_number, &wires);

            Cartridge *out=new Cartridge(rotors_number, wires);
            Rotor **rotors=out->get_rotors();
            Reflector *reflector=out->get_reflector();
            //read in the rotors
            while (file) {
                getline(file, line);
                //cout<<line<<"\n";
                //cout<<line.length()<<"\n";
                if (k<rotors_number) { //a rotor
                    //cout<<"a rotor, length="<<len<<"\n";
                    for(int i=0; i<wires; i++)  {
                        //cout<<"("<<line[i]<<")---   "<<i<<"->"<<(int) line[i]-(int) 'A'<<"   ---\n";
                        wire=(int) line[i]-(int) 'A';
                        rotors[k]->get_wiring_in()[i] =wire;
                        rotors[k]->get_wiring_out()[wire]=i;
                    }
                    //rotors[k]->print();
                }
                else { //a reflector
                    for (int i=0; i<len; i++) {
                        reflector->get_wiring_in()[i]=(int) line[i]-(int) 'A';
                    }
                }
                k++;
            }
            //out->print();
            return out;
        }
};

class Enigma {
    private:
        Cartridge* m_cartridge;
        int m_rotors_number, m_wires;
        bool m_verbose=false;
        int  *m_rotor_position, m_ring_setting;

    public:
        Enigma(int rotors_number, int wires);
        ~Enigma();
        void set_coder();
        void set_verbose(int);
        void set_rotor_position(string);
        void set_rotor_position(int*);
        int* get_rotor_position();
        string get_rotor_position_as_string();
        void set_ring_setting(string);
        void set_ring_setting(int*);
        int* get_ring_setting();
        string get_ring_setting_as_string();
        //void indicator_procedure_early(string, string);
        //void indicator_procedure_WW2(string, string); //wehrmacht luftwaffe
        //void indicator_procedure_kriegsmarine(string, string);
        void reset();
        void randomize();
        int encrypt(int m);
        int* encrypt(int* m, int n);
        void print_positions();
        void print();
        //FACTORY
        static Enigma make_random_enigma(int rotors, int wires) {
            //cout<<"init enigma\n";
            Enigma *enigma;
            enigma=new Enigma(rotors, wires);
            //cout<<"randomize enigma\n";
            enigma->randomize();
            //cout<<"return enigma\n";
            return *enigma;
        }
};

#endif
