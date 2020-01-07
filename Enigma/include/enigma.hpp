#ifndef ENIGMA_H   // include guard
#define ENIGMA_H

// KDO KDP, KDQ, KER, LFS, LFT, LFU
// electrical signal runs through the rotors.
// These points can be specified in terms of which letter appears in the window
// when the knock-on occurs RFWKA
// should position update if ring setting is changed? or vice versa?

using namespace std;
#include <algorithm>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <typeinfo>
#include <utility>
#include <vector>
//#inlcude "alphabet"

class Rotor {
    /* A rotor is the basic component of the enigma.
    The rotor represents a simple substitution cipher, but the substitutions
    can be offset by a given mount, represented by that the rotor rotates during
    encryption.*/
  protected:
    int *m_wiring_in, *m_wiring_out,
        *m_notch;                // determines at which positions the next rotor is engaged
    int    m_wires, m_notches;   // typically 26 and 1
    bool   m_verbose= false;     // prints entire encryption under encryption
    string m_name   = "CUSTOM";

  public:
    Rotor();
    Rotor(const int, const int= 1, string= "CUSTOM");
    Rotor(const string, const int= 1, string= "CUSTOM");
    Rotor(const string, const string, string= "CUSTOM");
    Rotor(Rotor const &copy);
    Rotor &operator=(Rotor rhs);
    void   swap(Rotor &s) noexcept;
    virtual ~Rotor();
    // getters
    int        get_wires() const;
    const int *get_wiring_in() const;
    const int *get_wiring_out() const;
    int        get_wiring_in(int i) const;
    int        get_wiring_out(int i) const;
    const int *get_notch() const;
    int        get_notch(int) const;
    int        get_notches() const;
    string     get_name() const;
    // setters
    void set_wiring_in(int, int);
    void set_wiring_out(int, int);
    void set_verbose(bool);
    // other
    int  encrypt_in(int, int) const;
    void encrypt_in_inplace(int *, int, int) const;
    // void encrypt_in_inplace(int *, vector<bool> &, bool, int, int) const;
    int  encrypt_out(int, int) const;
    void encrypt_out_inplace(int *, int, int) const;
    void randomize();
    // void encrypt_out_inplace(int *, vector<bool> &, bool, int, int) const;
    void print() const;
    void make_inverse(const int *in, int *out, int n) const;
    bool is_valid() const;
};

class Reflector: public Rotor {
    /*A form of rotor, but it usually does not rotate. A reflector has a
    symmetric wiring, meaning that if A is wired to K, K is also wired to A*/
  public:
    Reflector();
    Reflector(int wires);
    Reflector(const string);
    Reflector(const string, const string, string= "CUSTOM");
    Reflector(Reflector const &copy);
    Reflector &operator=(Reflector rhs);
    void       randomize();
    bool       is_valid() const;
};

class Plugboard {
    /*the plugboard is, like the reflector, a symmetric substitution cipher. The
    plugboard switches up two -and-two letters, typically 10. is the first and
    last encryption to take place in the encryption of a single letter with the
    enigma*/
  private:
    int         m_wires;
    vector<int> m_wiring;

  public:
    Plugboard();
    Plugboard(int);
    Plugboard(const string, int);
    Plugboard(Plugboard const &copy);
    ~Plugboard();
    Plugboard &operator=(Plugboard);
    void       swap(Plugboard &) noexcept;
    int        encrypt(int) const;
    void       encrypt_inplace(int *, int) const;
    void       reset();   // make identity
    // setters
    void set_wiring(const string);
    void set_wiring(int, int);
    // getters
    vector<int> &get_wiring();
    int          get_wiring(int) const;
    void         print() const;
};

class Cartridge {
    /*The crtridge is a class for containing all the parts of an enigma.
    Handles the flow of current from one encryption element to the next*/
  private:
    Rotor **   m_rotors;
    Rotor *    m_stator;
    Plugboard *m_plugboard;
    Reflector *m_reflector;
    int        m_rotor_count, m_wires, m_reflector_position;
    // positions refers to the actual positioning of a rotor relative to the
    // stator, not rotor position which is the letter shown in the window of the
    // enigma. Ring setting is which input in the rotor core the A wire goes to
    // position= rotor_position-ring_setting
    int *m_positions, *m_ring_setting;
    // if verbose, prints entire path of encryption when passing through
    // elements
    bool       m_verbose= false;
    const bool m_trivial_stator;

  public:
    Cartridge(int, int);   // CONSTRUCTOR, random rotors
    Cartridge(const initializer_list<Rotor>, Reflector, const bool= true);
    Cartridge(const vector<Rotor>, Reflector, const bool= true);
    Cartridge(Rotor, const initializer_list<Rotor>, Reflector);
    Cartridge(struct EnigmaSetting);
    Cartridge &operator=(Cartridge rhs);
    void       swap(Cartridge &s) noexcept;
    ~Cartridge();
    // GETTERS
    // elements
    struct EnigmaSetting get_setting() const;
    inline const Rotor **get_rotors() const { return (const Rotor **)m_rotors; }
    const Reflector *    get_reflector() const;
    Plugboard *          get_plugboard() const;   // hard to handle if const...
    // get values
    int          get_reflector_position() const;
    const int *  get_ring_setting() const;
    const string get_ring_setting_as_string() const;
    const int *  get_positions() const;
    const string get_positions_as_string() const;
    const string get_rotor_position_as_string() const;   // computed, not stored
    // get other
    void get_encryption_inplace(int *) const;
    bool get_if_trivial_stator() const;
    // SETTERS
    // set elements
    void set_setting(struct EnigmaSetting);
    void set_plugboard(const string);
    void set_rotor(int, const Rotor *);
    void set_reflector(const Reflector *);
    // set values
    void set_positions(const int *p);
    void set_rotor_position(const string in);
    void set_ring_setting(const int *p);
    void set_ring_setting(const string in);
    void set_verbose(bool);
    // other
    void        reset_positions();
    void        reset_ring_setting();
    void        turn(int);
    void        turn();
    int         encrypt_without_turning(int i) const;
    vector<int> encrypt_stepwise(int) const;
    int         plugboard_encrypt(int i) const;
    void        next_ring_setting();
    void        print() const;
    void        print_positions() const;
    void        randomize();
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
    /*static Cartridge *from_file(const char *filename) {
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
    }*/
};

struct EnigmaSetting {
    vector<Rotor> rotors;
    Reflector     reflector;
    Rotor         stator;
    bool          trivial_stator= true;
    Plugboard     plugboard;
    string        ring_setting;
    string        rotor_position;
};

class Enigma {
  private:
    string     description= "A general implementation of an enigma";
    Cartridge *m_cartridge;
    int        m_rotors_number, m_wires;
    bool       m_verbose= false, m_verbose_exploded= false, m_verbose_wheels= false,
         m_verbose_cartridge= false;

  public:
    Enigma(int rotors_number, int wires);
    Enigma(const std::initializer_list<Rotor> rotors, const Reflector reflector);
    Enigma(const vector<Rotor> rotors, const Reflector reflector);
    Enigma(struct EnigmaSetting setting);
    ~Enigma();
    // getters
    struct EnigmaSetting get_setting();
    int                  get_wires() const;
    int                  get_rotors() const;
    const int *          get_positions() const;
    // const int *            get_rotor_position() const;
    const string           get_rotor_position_as_string() const;
    const int *            get_ring_setting() const;
    string                 get_ring_setting_as_string() const;
    int *                  get_encryption() const;
    void                   get_encryption_inplace(int *) const;
    void                   get_encryption_inplace_lazy(int *) const;
    string                 get_encryption_as_string() const;
    vector<pair<int, int>> get_encryption_onesided() const;
    Cartridge *            get_cartridge() const;
    // setters
    void set_setting(struct EnigmaSetting);
    void set_coder();
    void set_verbose(bool);
    void set_cartridge_verbose(bool);
    void set_positions(const int *);
    void set_rotor_position(const string);
    void set_ring_setting(const string);
    void set_ring_setting(const int *);
    void set_plugboard(const string);
    // other
    string indicator_procedure_early(string);
    string indicator_procedure_WW2(string, string);               // wehrmacht, luftwaffe
    string indicator_procedure_kenngruppenbuch(string, string);   // naval
    string indicator_procedure_verfahrenkenngruppe(string, string);
    // void indicator_procedure_kriegsmarine(string, string);
    // void   turn(int);
    void   turn();
    void   reset();
    void   randomize();
    int    encrypt(int m);
    int    encrypt_without_turning(int m) const;
    string encrypt(string);
    int *  encrypt(const int *m, int n);
    int    encrypt_verbose_exploded(int m) const;
    void   next_ring_setting();
    void   print_positions() const;
    void   print() const;
    string preprocess(string in) const;
    void   encrypt(istream &, ostream &);
    int    compute_total_permutations_brute_force();
    // FACTORY
    /*
    static Enigma make_random_enigma(int rotors, int wires) {
        // cout<<"init enigma\n";
        Enigma *enigma;
        enigma= new Enigma(rotors, wires);
        // cout<<"randomize enigma\n";
        enigma->randomize();
        return *enigma;
    }
    */
};

#endif
