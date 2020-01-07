#ifndef BOMBE_H   // include guard
#define BOMBE_H

using namespace std;
#include "enigma.hpp"
//#include "queue"
#include <chrono>

// wiring
class Wire {
  private:
    vector<Wire *> m_connections;
    bool           m_live= false;   // 0: dead, 1: live, -1: just activated
  public:
    ~Wire();
    void            flow();   // make wire live, and also wires connected to it
    bool            get_live() const;
    void            set_live(bool);
    void            kill();   // set to 0
    void            reset();
    void            connect(Wire *w);
    vector<Wire *> *get_connections();
};

class DiagonalBoard {
  private:
    vector<vector<Wire *>> m_bundles;

  public:
    DiagonalBoard(int);
    ~DiagonalBoard();
    Wire *get_wire(int, int) const;
    void  activate(int, int);
    void  connect(int, int, int, int);
    void  print() const;
    void  print_live() const;
    void  print_connections() const;
    void  connect_enigma(int *, int, int);
    void  wipe();
    void  reset();
    bool  bundle_contradiction(int) const;
    int   bundle_sum(int) const;
};

struct BombeUnitSetting {
    // if true, stops as soon as a valid config is found
    bool stop_on_first_valid= false;
    // bool   verbose;
    bool       only_one_candidate           = false;
    int        max_ring_settings            = 26 * 26;
    string     starting_ring_setting        = "AAA";   // TODO should adapt to more rotors
    string     starting_rotor_positions     = "AAA";
    bool       interactive_wiring_mode      = false;
    const bool time_performance             = true;   // XXX can only be set in code...
    double     performance_ring_setting_mean= 0;      // performance of a rs run
    double     performance_ring_setting_var = 0;
    int        records_ring_setting         = 0;
};

class BombeUnit {
  private:
    int            m_letters= 26, m_rotor_count= 3;
    DiagonalBoard *m_diagonal_board;
    vector<int *>  m_enigma_encryptions;
    Enigma *       m_enigma;
    bool           m_verbose= false;
    // rotor positions
    // track encryptions
    struct BombeUnitSetting m_setting;

  public:
    BombeUnit(const std::initializer_list<Rotor> rotors, const Reflector reflector);
    BombeUnit(const vector<Rotor> rotors, const Reflector reflector);
    BombeUnit(struct EnigmaSetting);
    ~BombeUnit();
    // setters
    void set_ring_setting(const string);
    void set_rotor_position(const string);
    // getters
    struct BombeUnitSetting &get_setting();
    // other
    vector<int>                  probable_search(const string &, const string &);
    int                          find_most_wired_letter(const string &, const string &);
    void                         init_enigma_encryptions(int, vector<string> &);
    void                         reset_diagonal_board();
    vector<struct EnigmaSetting> analyze(const string &, const string &);
    void                         setup_diagonal_board(const string &, const string &);
    bool                         check_one_wire(int);
    bool                         bundle_contradiction(int);
    void                         print_encryptions() const;
    void                         print_performance() const;
    bool                         doublecheck_and_get_plugboard();
    bool           tripplecheck(const string &, const string &, int, vector<string> &);
    void           interactive_wirechecking();
    vector<string> get_special_rotor_positions();
};

struct BombeSetting {
    // if true, stops as soon as a valid config is found
    bool stop_on_first_valid= false;
    // bool   verbose;
    bool       only_one_candidate      = false;
    bool       only_one_configuration  = false;
    int        max_ring_settings       = 26 * 26;
    int        rotor_count             = 3;
    string     starting_ring_setting   = "AAA";   // TODO should adapt to more rotors
    string     starting_rotor_positions= "AAA";
    const bool time_performance        = true;   // XXX can only be set in code...
};

class Bombe {
  private:
    vector<BombeUnit>       m_units;
    vector<Rotor>           m_rotors;
    vector<vector<Rotor>>   m_rotor_configurations;
    Reflector *             m_reflector;
    struct BombeSetting     m_setting;
    struct BombeUnitSetting m_unit_setting;

  public:
    Bombe(const initializer_list<Rotor> rotors, const Reflector reflector);
    // Bombe(struct EnigmaSetting enigma_setting);
    vector<struct EnigmaSetting> analyze(const string &, const string &);
    // void                         banburismus(const string, const string crib);
    // float index_of_coincidence(const string, const string);
    struct BombeSetting &get_setting();
};
#endif
