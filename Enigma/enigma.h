#ifndef ENIGMA_H   // include guard
#define ENIGMA_H

using namespace std;
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utility>
#include <vector>
//#include "boost"
#include <algorithm>
#include <sstream>
//#include <iterator>
//#include <sstream>
/*
class Plugboard: public Reflector {
}*/

class Rotor {
  protected:
    int *m_wiring_in, *m_wiring_out,
        *m_notch;   // index i goes to value at index i
    int  m_wires, m_notches;
    bool m_verbose= false;

  public:
    Rotor();
    Rotor(const int wires);
    Rotor(const string);   // construct from string, ABCDEFGHIJKLMNOPQRSTUVWXYZ
                           // etc
    Rotor(const string, const string);
    // constexpr Rotor(const string, const string);
    Rotor(Rotor const &copy);
    Rotor &operator=(Rotor rhs);
    void   swap(Rotor &s) noexcept;
    ~Rotor();
    // getters
    int        get_wires() const;
    const int *get_wiring_in() const;
    const int *get_wiring_out() const;
    int        get_wiring_in(int i) const;
    int        get_wiring_out(int i) const;
    const int *get_notch() const;
    int        get_notch(int) const;
    int        get_notches() const;
    // setters
    void set_wiring_in(int, int);
    void set_wiring_out(int, int);
    void set_verbose(bool);
    // other
    int  encrypt_in(int, int) const;
    void encrypt_in_inplace(int *, int, int) const;
    void encrypt_in_inplace(int *, vector<bool> &, bool, int, int) const;
    int  encrypt_out(int, int) const;
    void encrypt_out_inplace(int *, int, int) const;
    void randomize();
    void encrypt_out_inplace(int *, vector<bool> &, bool, int, int) const;
    void print() const;
    void make_inverse(const int *in, int *out, int n) const;
    bool is_valid() const;
};

class Reflector: public Rotor {
  public:
    Reflector();
    Reflector(int wires);
    Reflector(const string);
    Reflector(const string, const string);
    void randomize();
    bool is_valid() const;
};

class Plugboard {
  private:
    int         m_wires;
    vector<int> m_wiring;

  public:
    // Plugboard();
    Plugboard(int);
    Plugboard(const string, int);
    Plugboard(Plugboard const &copy);
    ~Plugboard();
    Plugboard &operator=(Plugboard);
    void       swap(Plugboard &) noexcept;
    // Plugboard(const char, int);
    int  encrypt(int) const;
    void encrypt_inplace(int *, int) const;
    void reset();   // make identity
    // int* encrypt(const int*) const;
    void set_wiring(const string);
    void set_wiring(int, int);
    // getters
    vector<int> get_wiring() const;
    int         get_wiring(int) const;
};

class Cartridge {
  private:
    Rotor **   m_rotors;
    Reflector *m_reflector;
    int        m_rotor_count, m_wires, m_reflector_position;   // wires?
    int *      m_positions,
        *m_ring_setting;   // ringscthellung, moves the notches in the wheels
    bool m_verbose= false;
    // int       *m_notch_position;
    Plugboard *m_plugboard;

  public:
    Cartridge();                             // XXX stupitt
    Cartridge(int rotor_count, int wires);   // CONSTRUCTOR, random rotors
    Cartridge(const std::initializer_list<Rotor> rotors, Reflector reflector);
    Cartridge(Cartridge const &copy);
    Cartridge &operator=(Cartridge rhs);
    void       swap(Cartridge &s) noexcept;
    ~Cartridge();
    // getters
    struct EnigmaSetting get_setting() const;
    const Rotor **       get_rotors() const;
    const Reflector *    get_reflector() const;
    const int *          get_positions() const;
    Plugboard *          get_plugboard() const;   // hard to handle if const...
    int                  get_positions_as_int() const;
    int                  get_reflector_position() const;
    const int *          get_ring_setting() const;
    const string         get_positions_as_string() const;
    const string         get_ring_setting_as_string() const;
    // setters
    void set_setting(struct EnigmaSetting);
    void set_plugboard(const string);
    void set_rotor(int, const Rotor *);
    void set_reflector(const Reflector *);
    void set_positions(const int *p);
    void set_positions(const string in);
    void set_ring_setting(const int *p);
    void set_ring_setting(const string in);
    void set_verbose(bool);
    // other
    void reset_positions();
    void reset_ring_setting();
    void turn(int);
    void turn();   // overloaded, single turn
    int  encrypt_without_turning(
         int i) const;   // pass integer through wires without turning
    int  plugboard_encrypt(int i) const;
    void next_ring_setting();
    void print() const;             // PRINT cartridge
    void print_positions() const;   // print positions of the rotors
    void randomize();
    // Rotor* make_random_rotors(int n, int wires); //make array of n random
    // rotors

    // make cartridge from file, using standard coding A->0, B->1, ...english
    // letters
    /*syntax
    rotors:NUMBER_OF_WHEELS(excl reflector) wires:NUMBER OF WIRES(here, 26)
    ABCDEFGHIJKLMNOPQRSTUVWXYZ   //first rotor, pass-through in this case
    BADCFEHGJILKNMPORQTSVUXWZY   //second rotor,
    ...
    --.--                        //last rotor, must be a reflector, ie symmetric
    */
    static Cartridge *from_file(const char *filename) {
        int      rotors_number, wires, k= 0, wire;
        string   line;
        int      len= 0;
        ifstream file(filename);
        if (!file) {
            printf("ERROR: Opening file %s failed", filename);
            return nullptr;
        }
        // read first line to get number of rotors and wires
        getline(file, line);
        sscanf(line.c_str(), "rotors:%d wires:%d", &rotors_number, &wires);

        Cartridge *out= new Cartridge(rotors_number, wires);
        // read in the rotors
        while (file) {
            getline(file, line);
            if (k < rotors_number) {   // a rotor
                Rotor *rotor= new Rotor(wires);
                // cout<<"a rotor, length="<<len<<"\n";
                for (int i= 0; i < wires; i++) {
                    // cout<<"("<<line[i]<<")---   "<<i<<"->"<<(int)
                    // line[i]-(int) 'A'<<"   ---\n";
                    wire= (int)line[i] - (int)'A';
                    rotor->set_wiring_in(i, wire);
                    rotor->set_wiring_out(wire, i);
                }
                out->set_rotor(k, rotor);
            } else {   // a reflector
                Reflector *reflector= new Reflector(wires);
                for (int i= 0; i < len; i++) {
                    reflector->set_wiring_in(i, (int)line[i] - (int)'A');
                }
                out->set_reflector(reflector);
            }
            k++;
        }
        return out;
    }
};

struct EnigmaSetting {
    vector<Rotor *> rotors;
    Reflector *     reflector;
    Plugboard *     plugboard;
    string          ring_setting;
    string          rotor_position;
};

class Enigma {
  private:
    Cartridge *m_cartridge;
    int        m_rotors_number, m_wires;
    bool       m_verbose= false;
    // int  *m_rotor_position, m_ring_setting;

  public:
    Enigma(int rotors_number, int wires);
    Enigma(const std::initializer_list<Rotor> rotors,
           const Reflector                    reflector);
    Enigma(struct EnigmaSetting setting);
    ~Enigma();
    // getters
    struct EnigmaSetting   get_setting();
    int                    get_wires() const;
    int                    get_rotors() const;
    const int *            get_rotor_position() const;
    const string           get_rotor_position_as_string() const;
    const int *            get_ring_setting() const;
    string                 get_ring_setting_as_string() const;
    int *                  get_encryption() const;
    void                   get_encryption_inplace(int *) const;
    string                 get_encryption_as_string() const;
    vector<pair<int, int>> get_encryption_onesided() const;
    Cartridge *            get_cartridge() const;
    // setters
    void set_setting(struct EnigmaSetting);
    void set_coder();
    void set_verbose(bool);
    void set_cartridge_verbose(bool);
    void set_rotor_position(const string);
    void set_rotor_position(const int *);
    void set_ring_setting(const string);
    void set_ring_setting(const int *);
    void set_plugboard(const string);
    // other
    string indicator_procedure_early(string);
    string indicator_procedure_WW2(string, string);   // wehrmacht, luftwaffe
    string indicator_procedure_kenngruppenbuch(string, string);   // naval
    string indicator_procedure_verfahrenkenngruppe(string, string);
    // void indicator_procedure_kriegsmarine(string, string);
    void   turn(int);
    void   turn();
    void   reset();
    void   randomize();
    int    encrypt(int m);
    int    encrypt_without_turning(int m) const;
    string encrypt(string);
    int *  encrypt(const int *m, int n);
    void   next_ring_setting();
    void   print_positions() const;
    void   print() const;
    // FACTORY
    static Enigma make_random_enigma(int rotors, int wires) {
        // cout<<"init enigma\n";
        Enigma *enigma;
        enigma= new Enigma(rotors, wires);
        // cout<<"randomize enigma\n";
        enigma->randomize();
        // cout<<"return enigma\n";
        return *enigma;
    }
};

#endif
