#ifndef ENIGMA_H // include guard
#define ENIGMA_H

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <memory>
#include <vector>
#include <initializer_list>
/*
class Plugboard: public Reflector {
}*/



class Rotor {
    protected:
        int *m_wiring_in, *m_wiring_out, *m_notch; //index i goes to value at index i
        int m_wires, m_notches;
        bool m_verbose=false;

    public:
        Rotor();
        Rotor(const int wires);
        Rotor(const string); //construct from string, ABCDEFGHIJKLMNOPQRSTUVWXYZ etc
        Rotor(const string, const string);
        //constexpr Rotor(const string, const string);
        Rotor(Rotor const& copy);
        Rotor& operator=(Rotor rhs);
        void swap(Rotor& s) noexcept;
        ~Rotor();
        //getters
        int  get_wires() const;
        const int* get_wiring_in() const ;
        const int* get_wiring_out() const;
        int  get_wiring_in(int i) const;
        int  get_wiring_out(int i) const;
        const int* get_notch() const;
        int  get_notch(int) const;
        int  get_notches() const;
        //setters
        void set_wiring_in(int, int);
        void set_wiring_out(int, int);
        void set_verbose(bool);
        //other
        int  encrypt_in(int, int) const;
        int  encrypt_out(int, int) const;
        void randomize();
        void print();
        void make_inverse(int* in, int* out, int n);
        bool is_valid();
};

class Reflector: public Rotor{
    public:
        Reflector();
        Reflector(int wires);
        Reflector(string);
        Reflector(string, string);
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
        Cartridge(std::initializer_list<Rotor> rotors, Reflector reflector);
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
                        rotors[k]->set_wiring_in(i, wire);
                        rotors[k]->set_wiring_out(wire,i);
                    }
                    //rotors[k]->print();
                }
                else { //a reflector
                    for (int i=0; i<len; i++) {
                        reflector->set_wiring_in(i, (int) line[i]-(int) 'A');
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
        Enigma(std::initializer_list<Rotor> rotors, Reflector reflector);
        ~Enigma();
        void set_coder();
        void set_verbose(int);
        void turn();
        int get_wires();
        int get_rotors();
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
        int encrypt_without_turning(int m);
        int* encrypt(int* m, int n);
        void print_positions();
        void print();
        vector<int> get_encryption();
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
