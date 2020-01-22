#ifndef BOMBE_H // include guard
#define BOMBE_H

const bool DEFAULT_USE_CONFIGURATION_GRID = false;

using namespace std;
#include "enigma.hpp"
//#include "queue"
#include <chrono>
#include <cstddef>
#include <iostream>

class DiagonalBoard {
private:
  vector<vector<Wire *>> m_bundles;
  // wiring
  class Wire {
  private:
    vector<Wire *> m_connections;
    bool m_live = false; // 0: dead, 1: live, -1: just activated
  public:
    ~Wire();
    void flow(); // make wire live, and also wires connected to it
    bool get_live() const;
    void set_live(bool);
    void kill(); // set to 0
    void reset();
    void connect(Wire *w);
    vector<Wire *> *get_connections();
  };

public:
  DiagonalBoard(int);
  ~DiagonalBoard();
  Wire *get_wire(int, int) const;
  void activate(int, int);
  void connect(int, int, int, int);
  void print() const;
  void print_live() const;
  void print_connections() const;
  void connect_enigma(int *, int, int);
  void wipe();
  void reset();
  bool bundle_contradiction(int) const;
  int bundle_sum(int) const;
};

class ConfigurationGrid {
  /*The Configuration grid is owned by a bombeunit, but its precomputation used
  for bookkeeping is done by the bombe, ie the class that delegates the work to
  the units. Throghout the checking of all rotor positions and ring settings,
  there are a lot of ring settings and rotor positions that have the same
  confiduration of the enigma throughout encryption. Therefore there will
  usually be lots of configurations that encrypts the crib to the given
  ciphertext. The configuration board keeps track of which configurations are
  already searched. This approach slows down the individual checks, but will
  drastically lower the amount of checks done at the cost of a lot of
  bookkeeping. This class is probably the source of the largest meemory
  footprint in the program.*/
private:
  int m_letters;
  int m_rotor_count;
  int m_crib_length = -1;
  long int m_total_configurations = 0;
  long int m_checked_configurations = 0;
  unsigned int m_rotor_configuration_count = 0;
  // vector of rotor positions, element i corresponds to column i of grid
  vector<vector<shint>> m_all_rotor_positions;
  // satisfies
  // m_all_rotor_positions[m_all_rotor_positions_inverse[rotor_position_string_to_int(rotor_position)]]=rotor_position
  // TODO make a narrower hash function
  vector<unsigned int> m_all_rotor_positions_inverse;
  // rows are ring setting(num=letter^rotors) columns are rotor positions
  // is true if the given ring setting and rotor posiiton is checked
  vector<bool> m_checked;
  // used in setchecked, stored here to avoid unnecessary frees and allocs
  // the m_ prefix is not used as it technically is not used as an attribute

  vector<shint> current_ring_setting;

public:
  bool print_on_next = true;
  ConfigurationGrid(Enigma &enigma);
  void reset_checked();
  bool get_checked(const int *ring_setting, const vector<int> &rotor_position);
  void set_checked(vector<vector<shint>>::const_iterator position);
  void set_crib_length(int);
  unsigned long int get_total_configurations() const;
  unsigned long int get_checked_configurations() const;
  unsigned int get_rotor_position_count() const;
  void find_unchecked() const;
  unsigned int ring_setting_string_to_int(const string &);
  unsigned int rotor_position_string_to_int(const string &);
  unsigned int ring_setting_array_to_int(const int *);
  unsigned int rotor_position_array_to_int(const int *);
  unsigned int rotor_position_vector_to_int(const vector<int> &);
  unsigned int string_to_int_hash(const string &str);
  unsigned int vector_to_int_hash(const vector<shint> &);
  unsigned int array_to_int_hash(const int *);
  unsigned int array_to_int_reverse_hash(const int *);
};

struct BombeUnitSetting {
  // if true, stops as soon as a valid config is found
  bool stop_on_first_valid = false;
  // bool   verbose;
  bool only_one_candidate = false;
  int max_ring_settings = 26 * 26 * 26 * 26;
  string starting_ring_setting = "AAAA"; // TODO should adapt to more rotors
  string starting_rotor_positions = "AAAA";
  bool interactive_wiring_mode = false;
  const bool time_performance = true;       // XXX can only be set in code...
  double performance_ring_setting_mean = 0; // performance of a rs run
  double performance_ring_setting_var = 0;
  int records_ring_setting = 0;
  const bool debug_doublecheck_show_wiring = false;
};

class BombeUnit {
private:
  string m_identifier = "A bombeunit";
  int m_letters = 26, m_rotor_count = 3;
  DiagonalBoard *m_diagonal_board;
  vector<int *> m_enigma_encryptions;
  ConfigurationGrid *m_configuration_grid;

  Enigma *m_enigma;
  bool m_verbose = false;
  const bool m_use_configuration_grid = DEFAULT_USE_CONFIGURATION_GRID;
  // rotor positions
  // track encryptions
  struct BombeUnitSetting m_setting;
  // this object is used to keep track of enigma settings explored

public:
  BombeUnit(
      const std::initializer_list<Rotor> rotors, const Reflector reflector,
      const bool use_configuration_board = DEFAULT_USE_CONFIGURATION_GRID);
  BombeUnit(
      const vector<Rotor> rotors, const Reflector reflector,
      const bool use_configuration_board = DEFAULT_USE_CONFIGURATION_GRID);
  BombeUnit(struct EnigmaSetting, const bool use_configuration_board =
                                      DEFAULT_USE_CONFIGURATION_GRID);
  ~BombeUnit();
  // setters
  void set_identifier(string);
  void set_ring_setting(const string);
  void set_rotor_position(const string);
  // getters
  struct BombeUnitSetting &get_setting();
  string get_identifier() const;
  // other

  void init_enigma_encryptions(int, vector<string> &, vector<vector<shint>> &);
  void reset_diagonal_board();
  vector<struct EnigmaSetting> analyze(const string &, const string &, int,
                                       int);
  void setup_diagonal_board(const string &, const string &);
  bool check_one_wire(int);
  bool bundle_contradiction(int);
  void print_encryptions() const;
  void print_performance() const;
  bool doublecheck_and_get_plugboard();
  bool tripplecheck(const string &, const string &, int, vector<string> &);
  void interactive_wirechecking();
  vector<string> get_special_rotor_positions();
  void print_progress(int, int, int);
  // void           get_equivalent_settings(vector<int>);
};

struct BombeSetting {
  // if true, stops as soon as a valid config is found
  bool stop_on_first_valid = false;
  // bool   verbose;
  bool only_one_candidate = false;
  bool only_one_configuration = false;
  int max_ring_settings = 26 * 26 * 26 * 26;
  int rotor_count = 3;
  string starting_ring_setting = "EPEL"; // TODO should adapt to more rotors
  string starting_rotor_positions = "AAAA";
  const bool time_performance = true;       // XXX can only be set in code...
  double performance_ring_setting_mean = 0; // performance of a rs run
  double performance_ring_setting_var = 0;
  int records_ring_setting = 0;
  int records_bombeunits = 0;
  double performance_unit_run_mean = 0;
  double performance_unit_run_var = 0;
  int records_unit_run = 0;
};

class Bombe {
private:
  int m_letters = 26;
  bool m_verbose = false;
  const bool m_using_configuration_board = true;
  vector<BombeUnit> m_units;
  vector<Rotor> m_rotors;
  vector<vector<Rotor>> m_rotor_configurations;
  vector<Reflector> m_reflector;
  const bool m_use_configuration_grid = DEFAULT_USE_CONFIGURATION_GRID;
  // ofstream                m_outstream;
  struct BombeSetting m_setting;
  struct BombeUnitSetting m_unit_setting;

public:
  Bombe(const initializer_list<Rotor> rotors, const Reflector reflector,
        const bool use_configuration_grid = DEFAULT_USE_CONFIGURATION_GRID);
  Bombe(vector<Rotor> rotors, vector<Reflector> reflector,
        const bool use_configuration_grid = DEFAULT_USE_CONFIGURATION_GRID);
  // Bombe(struct EnigmaSetting enigma_setting);
  vector<struct EnigmaSetting> analyze_unit(const string &, const string &,
                                            vector<Rotor> &, Reflector &, int,
                                            int);
  vector<int> probable_search(const string &, const string &);
  vector<struct EnigmaSetting> analyze(const string &, const string &);
  int find_most_wired_letter(const string &, const string &);
  // void                         set_outstream(ostream &);
  string preprocess(string) const;
  // void                         banburismus(const string, const string crib);
  // float index_of_coincidence(const string, const string);
  struct BombeSetting &get_setting();
};
#endif
