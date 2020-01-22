#include "enigma.hpp"
#include <cmath>
using namespace std;

// ROTOR
Rotor::Rotor() : Rotor(0, 0, "DEFAULTCONSTRUCTED") {
    // default constructor
}
Rotor::Rotor(int wires, int notches /*1*/, string name /*CUSTOM*/) :
    m_wires{wires}, m_notches{notches}, m_name{name} {
    // base constructor
    m_notch= new int[m_notches];
    // make ok spread of notched from A
    for (int j= 0; j < m_notches; ++j) { m_notch[j]= (m_wires / m_notches) * j; }
    // allocate to wiring array
    m_wiring_in = new int[m_wires];
    m_wiring_out= new int[m_wires];
    // make identity wiring
    for (int j= 0; j < m_wires; j++) {
        m_wiring_in[j] = j;
        m_wiring_out[j]= j;
    }
}
Rotor::Rotor(const string in, int notches /*1*/, string name /*CUSTOM*/) :
    Rotor{(int)in.length(), notches, name} {
    for (int i= 0; i < m_wires; i++) {
        int wire          = (int)in[i] - (int)'A';
        m_wiring_in[i]    = wire;
        m_wiring_out[wire]= i;
    }
}
Rotor::Rotor(const string in, const string notch, string name) :
    Rotor(in, (int)notch.length(), name) {
    for (int i= 0; i < m_notches; i++) { m_notch[i]= (int)notch[i] - (int)'A'; }
}

Rotor::~Rotor() {
    // cout << "somewhere, a rotor died(" << get_wiring_in(0) << ") \n";
    delete[] m_wiring_in;
    delete[] m_wiring_out;
    delete[] m_notch;
}
Rotor::Rotor(Rotor const &copy) {
    // cout << "COPYING ROTOR\n";
    // Copy constructor, very important, assures there is no shallow copy
    // src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    // cout << "COPYING ROTOR; m_wires:" << copy.m_wires << "\n";
    m_wires= copy.m_wires;
    // cout << "COPYING ROTOR; m_notches:" << copy.m_notches << "\n";
    m_notches= copy.m_notches;
    // cout << "COPYING ROTOR; m_name:" << copy.m_name << "\n";
    m_name= copy.m_name;
    // cout << "COPYING ROTOR; make in:"
    //     << "\n";
    m_wiring_in= new int[m_wires];
    // cout << "COPYING ROTOR; make out:"
    //     << "\n";
    m_wiring_out= new int[m_wires];
    // cout << "COPYING ROTOR; make notch:"
    //     << "\n";
    m_notch= new int[m_notches];
    // Don't need to worry about copying integers.
    // But if the object has a copy constructor then
    // it would also need to worry about throws from the copy constructor.
    // cout << "COPYING ROTOR; copying in: \n";
    std::copy(&copy.m_wiring_in[0], &copy.m_wiring_in[m_wires], m_wiring_in);
    // cout << "COPYING ROTOR; copying out: \n";
    std::copy(&copy.m_wiring_out[0], &copy.m_wiring_out[m_wires], m_wiring_out);
    // cout << "COPYING ROTOR; copying notch: \n";
    std::copy(&copy.m_notch[0], &copy.m_notch[m_notches], m_notch);
    // cout << "COPYING ROTOR: all done: \n";
}
Rotor &Rotor::operator=(Rotor rhs) {
    // Pass by value (thus generating a copy)
    rhs.swap(*this);   // Now swap data with the copy.
                       // The rhs parameter will delete the array when it
                       // goes out of scope at the end of the function
    return *this;
}
void Rotor::swap(Rotor &s) noexcept {
    using std::swap;
    swap(this->m_wiring_in, s.m_wiring_in);
    swap(this->m_wiring_out, s.m_wiring_out);
    swap(this->m_notches, s.m_notches);
    swap(this->m_wires, s.m_wires);
    swap(this->m_notch, s.m_notch);
    swap(this->m_name, s.m_name);
}
// getters
int        Rotor::get_wires() const { return m_wires; }
const int *Rotor::get_wiring_in() const { return m_wiring_in; }
int        Rotor::get_wiring_in(int i) const { return m_wiring_in[i]; }
const int *Rotor::get_wiring_out() const { return m_wiring_out; }
int        Rotor::get_wiring_out(int i) const { return m_wiring_out[i]; }
const int *Rotor::get_notch() const { return m_notch; }
int        Rotor::get_notch(int n) const { return m_notch[n]; }
int        Rotor::get_notches() const { return m_notches; }
string     Rotor::get_name() const { return m_name; }
// setters
void Rotor::set_wiring_in(int pos, int set) { m_wiring_in[pos]= set; }
void Rotor::set_wiring_out(int pos, int set) { m_wiring_out[pos]= set; }
void Rotor::set_verbose(bool set) { m_verbose= set; }
// other
int Rotor::encrypt_in(int i, int offset) const {
    int out= (m_wiring_in[(i + offset) % m_wires] + 2 * m_wires - offset) % m_wires;
    if (m_verbose == true) {   // make const
        for (int j= 0; j < m_wires; j++) {
            if (j == i) { cout << "("; }
            cout << (char)((m_wiring_in[(j + offset) % m_wires] + 2 * m_wires - offset) % m_wires +
                           (int)'A');
            if (j == i) { cout << ")"; }
        }
    }
    return out;
}
int Rotor::encrypt_out(int i, int offset) const {
    int out= (m_wiring_out[(i + offset) % m_wires] + 2 * m_wires - offset) % m_wires;
    if (m_verbose) {
        for (int j= 0; j < m_wires; j++) {
            if (j == out) { cout << "("; }
            cout << (char)((m_wiring_in[(j + offset) % m_wires] + 2 * m_wires - offset) % m_wires +
                           (int)'A');
            if (j == out) { cout << ")"; }
        }
    }
    return out;
}
void Rotor::encrypt_in_inplace(int *plaintext, int offset, int n) const {
    for (int i= 0; i < n; ++i) {
        plaintext[i]= (m_wiring_in[(plaintext[i] + offset) % m_wires] + m_wires - offset) % m_wires;
    }
}
// NO PERFORMANCE GAIN
/*void Rotor::encrypt_in_inplace(int *plaintext, vector<bool> &flag, bool
flagged, int offset, int n) const {
    // encrypt plaintext[i] only if flag[i]==flagged
    int set= 0;
    for (int i= 0; i < n; ++i) {
        if (flag[i] == flagged) {
            set= (m_wiring_in[(plaintext[i] + offset) % m_wires] + m_wires -
                  offset) %
                 m_wires;
            plaintext[i]  = set;
            plaintext[set]= i;
            flag[set]     = !flagged;
            flag[i]       = !flagged;
        }
    }
}*/
void Rotor::encrypt_out_inplace(int *plaintext, int offset, int n) const {
    for (int i= 0; i < n; ++i) {
        plaintext[i]=
            (m_wiring_out[(plaintext[i] + offset) % m_wires] + m_wires - offset) % m_wires;
    }
}
// NO PERFORMANCE GAIN
/*void Rotor::encrypt_out_inplace(int *plaintext, vector<bool> &flag,
                                bool flagged, int offset, int n) const {
    // encrypt plaintext[i] only if flag[i]==flagged
    int set= 0;
    for (int i= 0; i < n; ++i) {
        if (flag[i] == flagged) {
            set= (m_wiring_out[(plaintext[i] + offset) % m_wires] + m_wires -
                  offset) %
                 m_wires;
            plaintext[i]  = set;
            plaintext[set]= i;
            flag[set]     = !flagged;
            flag[i]       = !flagged;
        }
    }
}*/

void Rotor::randomize() {
    int p1, p2, t;
    // init in as 012345...
    for (int i= 0; i < m_wires; i++) { m_wiring_in[i]= i; }
    // mix wires randomly
    for (int i= 0; i < m_wires * m_wires; i++) {
        p1= rand() % m_wires;
        p2= rand() % m_wires;
        if (p1 != p2) {
            t              = m_wiring_in[p2];
            m_wiring_in[p2]= m_wiring_in[p1];
            m_wiring_in[p1]= t;
        } else {
            i--;
        }
    }
    // now that in is randomized, make out the inverse of in
    make_inverse(m_wiring_in, m_wiring_out, m_wires);
}
void Rotor::make_inverse(const int *in, int *out, int n) const {
    // assumes list contains all integers from 0 to n-1
    for (int i= 0; i < n; i++) { out[in[i]]= i; }
}
void Rotor::print() const {
    for (int wire= 0; wire < m_wires; wire++) { printf("%2d ", m_wiring_in[wire]); }
    cout << "\n";
}
bool Rotor::is_valid() const {
    // is out the inverse? is the mapping surjective?(covered by w spanning all
    // wires)
    for (int w= 0; w < m_wires; w++) {
        if (get_wiring_out(get_wiring_in(w)) != w) { return false; }
    }
    return true;
}
// Reflector::Reflector() : Rotor() {}
Reflector::Reflector() : Reflector(0) {
    // m_wires  = 0;
    // m_notches= 0;
    // cout << "default reflector\n";
}
Reflector::Reflector(int wires) : Rotor(wires) {
    for (int j= 0; j < wires; j++) { m_wiring_in[j]= j + 1 - 2 * (j % 2); }
}
Reflector::Reflector(string wiring) : Rotor(wiring, "", "CUSTOM") {}
Reflector::Reflector(string wiring, string notches, string name) : Rotor(wiring, notches, name) {}
Reflector::Reflector(Reflector const &copy) : Rotor(copy) {}
Reflector &Reflector::operator=(Reflector rhs) {
    // cout << "copy assign reflector\n";
    Rotor::operator=(rhs);
    // cout << "copy assign end\n";
    return *this;
}

void Reflector::randomize() {
    int w1, w2, v1, v2;
    for (int k= 0; k < m_wires * m_wires; k++) {
        w1= rand() % m_wires;
        w2= rand() % m_wires;
        // cross the wires if possible, otherwise try again
        if (w1 != w2 && m_wiring_in[w1] != w2) {
            v2             = m_wiring_in[w2];
            v1             = m_wiring_in[w1];
            m_wiring_in[w2]= v1;
            m_wiring_in[v2]= w1;
            m_wiring_in[w1]= v2;
            m_wiring_in[v1]= w2;
        } else {
            k-= 1;
        }
    }
}
bool Reflector::is_valid() const {
    // is out the inverse? is the mapping surjective?(covered by w spanning all
    // wires) same as is valid for WHeel, but uses wiring_in both times
    for (int w= 0; w < m_wires; w++) {
        int m= get_wiring_in(w);
        if (get_wiring_in(m) != w && w != m) { return false; }
    }
    return true;
}

// Plugboard::Plugboard() {}
// read from string, format AB.CD.       wires A to B, C to D etc
Plugboard::Plugboard() {}
Plugboard::Plugboard(int t_wires) {
    // allocate to wiring array
    m_wires = t_wires;
    m_wiring= vector<int>(m_wires);
    for (int i= 0; i < m_wires; i++) { m_wiring[i]= i; }
}
Plugboard::Plugboard(const string in, int t_wires) : Plugboard{t_wires} { set_wiring(in); }
Plugboard::~Plugboard() {
    m_wiring.clear();
    m_wiring.shrink_to_fit();
}
Plugboard::Plugboard(Plugboard const &copy) {
    // cout << "COPYING PLUGBOARD\n";
    m_wires = copy.m_wires;
    m_wiring= vector<int>(copy.m_wiring);
}
Plugboard &Plugboard::operator=(Plugboard rhs) {
    // Pass by value (thus generating a copy)
    rhs.swap(*this);   // Now swap data with the copy.
                       // The rhs parameter will delete the array when it
                       // goes out of scope at the end of the function
    return *this;
}
void Plugboard::swap(Plugboard &s) noexcept {
    using std::swap;
    swap(this->m_wiring, s.m_wiring);
    swap(this->m_wires, s.m_wires);
}
int  Plugboard::encrypt(int in) const { return m_wiring[in]; }
void Plugboard::encrypt_inplace(int *plaintext, int n) const {
    for (int i= 0; i < n; ++i) { plaintext[i]= m_wiring[plaintext[i]]; }
}
void Plugboard::reset() {
    // identity
    for (int i= 0; i < m_wires; i++) { m_wiring[i]= i; }
}
void Plugboard::set_wiring(const string in) {
    reset();
    // set wiring
    string str= string(in);
    // remove whitespace of copy
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    // str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    int                      from, to;
    std::string              pair;      // Have a buffer string
    std::stringstream        ss(str);   // Insert the string into a stream
    std::vector<std::string> pairs;     // Create vector to hold our words
    while (getline(ss, pair, '.')) {
        from          = (int)pair[0] - (int)'A';
        to            = (int)pair[1] - (int)'A';
        m_wiring[from]= to;
        m_wiring[to]  = from;
    }
    // XXX check validity
}
void         Plugboard::set_wiring(int pos, int set) { m_wiring[pos]= set; }
vector<int> &Plugboard::get_wiring() { return m_wiring; }
int          Plugboard::get_wiring(int i) const { return m_wiring[i]; }
void         Plugboard::print() const {
    for (int wire= 0; wire < m_wires; wire++) { printf("%2d ", m_wiring[wire]); }
    cout << "\n";
}

// CARTRIDGE
// Cartridge::Cartridge() {}   // XXX should really not be neccesary...
Cartridge::Cartridge(int rotor_count, int wires) :
    m_rotor_count{rotor_count}, m_wires{wires}, m_trivial_stator(true) {
    m_positions   = new int[rotor_count];
    m_ring_setting= new int[rotor_count];
    // init positions
    reset_positions();
    reset_ring_setting();
    // make random rotors
    m_rotors= new Rotor *[rotor_count];
    for (int w= 0; w < rotor_count; w++) {
        m_rotors[w]= new Rotor(wires);
        m_rotors[w]->randomize();
    }
    // make a plugboard
    m_plugboard= new Plugboard(wires);
    // make a reflector
    m_reflector= new Reflector(wires);
    m_reflector->randomize();
}
Cartridge::Cartridge(const std::initializer_list<Rotor> rotors, Reflector reflector,
                     const bool trivial_stator /*true*/) :
    Cartridge(vector<Rotor>(rotors), reflector, trivial_stator) {}
Cartridge::Cartridge(const vector<Rotor> rotors, Reflector reflector,
                     const bool trivial_stator /*true*/) :
    m_rotor_count(rotors.size()),
    m_wires((rotors.begin())->get_wires()), m_trivial_stator(trivial_stator) {
    m_positions   = new int[m_rotor_count];
    m_ring_setting= new int[m_rotor_count];
    // init positions
    reset_positions();
    reset_ring_setting();
    // set to rotors
    m_rotors = new Rotor *[m_rotor_count];
    int count= 0;
    for (auto r : rotors) { m_rotors[count++]= new Rotor(r); }
    m_plugboard= new Plugboard(m_wires);
    m_reflector= new Reflector(reflector);
}
Cartridge::Cartridge(Rotor stator, const std::initializer_list<Rotor> rotors, Reflector reflector) :
    Cartridge(rotors, reflector, false) {
    m_stator= new Rotor(stator);
}
Cartridge::Cartridge(struct EnigmaSetting setting) : m_trivial_stator(setting.trivial_stator) {
    m_rotor_count= (signed int)setting.rotors.size();   // TODO check if not 0
    m_wires      = setting.rotors[0].get_wires();       // begin?
    // TODO set_setting instead?
    // alloc and set everything.
    m_positions   = new int[m_rotor_count];
    m_ring_setting= new int[m_rotor_count];
    m_rotors      = new Rotor *[m_rotor_count];
    for (int r= 0; r < m_rotor_count; ++r) { m_rotors[r]= new Rotor(setting.rotors[r]); }
    if (!setting.trivial_stator) {
        cout << "stator set\n";
        m_stator= new Rotor(setting.stator);
    }
    m_reflector= new Reflector(setting.reflector);
    m_plugboard= new Plugboard(setting.plugboard);
    set_ring_setting(setting.ring_setting);
    set_rotor_position(setting.rotor_position);
    // cout << m_trivial_stator << "\n";
}
/*
Cartridge::Cartridge(Cartridge const &copy) {
    //
src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    m_wires      = copy.m_wires;
    m_rotor_count= copy.m_rotor_count;
    m_rotors     = new Rotor *[m_rotor_count];
    // m_plugboard         =new Plugboard();
    m_reflector         = copy.m_reflector;
    m_positions         = new int[m_rotor_count];
    m_ring_setting      = new int[m_rotor_count];
    m_reflector_position= copy.m_reflector_position;
    // Don't need to worry about copying integers.
    // But if the object has a copy constructor then
    // it would also need to worry about throws from the copy constructor.
    m_rotors= copy.m_rotors;   // rotors know how to copy, so we good
    std::copy(&copy.m_positions[0], &copy.m_positions[m_wires], m_positions);
    std::copy(&copy.m_ring_setting[0], &copy.m_ring_setting[m_wires],
              m_ring_setting);
}*/
Cartridge &Cartridge::operator=(Cartridge rhs) {   // Pass by value (thus generating a copy)
    rhs.swap(*this);                               // Now swap data with the copy.
                       // The rhs parameter will delete the array when it
                       // goes out of scope at the end of the function
    return *this;
}
void Cartridge::swap(Cartridge &s) noexcept {
    using std::swap;
    swap(this->m_wires, s.m_wires);
    swap(this->m_rotor_count, s.m_rotor_count);
    swap(this->m_rotors, s.m_rotors);
    swap(this->m_reflector, s.m_reflector);
    swap(this->m_positions, s.m_positions);
    swap(this->m_ring_setting, s.m_ring_setting);
    swap(this->m_reflector_position, s.m_reflector_position);
    swap(this->m_plugboard, s.m_plugboard);
}
Cartridge::~Cartridge() {
    for (int w= 0; w < m_rotor_count; w++) { delete m_rotors[w]; }
    delete[] m_rotors;
    delete m_reflector;
    delete[] m_positions;
    delete[] m_ring_setting;
    delete m_plugboard;
    if (!m_trivial_stator) delete m_stator;
}
// GETTERS
// get elements
struct EnigmaSetting Cartridge::get_setting() const {
    struct EnigmaSetting out;
    for (int w= 0; w < m_rotor_count; ++w) {
        out.rotors.push_back(Rotor(*m_rotors[w]));   // make a copy
    }
    out.reflector= Reflector(*m_reflector);
    if (!m_trivial_stator) { out.stator= Rotor(*m_stator); }
    out.plugboard     = Plugboard(*m_plugboard);
    out.ring_setting  = get_ring_setting_as_string();
    out.rotor_position= get_rotor_position_as_string();
    return out;
}
// const Rotor **Cartridge::get_rotors() const {  }
const Reflector *Cartridge::get_reflector() const { return m_reflector; }
Plugboard *      Cartridge::get_plugboard() const { return m_plugboard; }
// GET value
int          Cartridge::get_reflector_position() const { return m_reflector_position; }
const int *  Cartridge::get_ring_setting() const { return m_ring_setting; }
const string Cartridge::get_ring_setting_as_string() const {
    // write in reverse
    string out= "";
    for (int i= m_rotor_count - 1; i >= 0; --i) { out+= (char)(m_ring_setting[i] + (int)'A'); }
    return out;
}
const int *  Cartridge::get_positions() const { return m_positions; }
const string Cartridge::get_positions_as_string() const {
    string out;
    out= "";
    for (int i= m_rotor_count - 1; i >= 0; --i) { out+= (char)(m_positions[i] + (int)'A'); }
    return out;
}
const string Cartridge::get_rotor_position_as_string() const {
    string out= "";
    for (int i= m_rotor_count - 1; i >= 0; --i) {
        out+= (char)((m_positions[i] + m_ring_setting[i] + m_wires) % m_wires + (int)'A');
    }
    return out;
}
bool Cartridge::get_if_trivial_stator() const { return m_trivial_stator; }
/*int        Cartridge::get_positions_as_int() const {
    int turn= 0;
    int mult= 1;
    for (int p= 0; p < m_rotor_count; p++) {
        turn+= m_positions[p] * mult;
        mult*= m_wires;
    }
    return turn;
}*/
void Cartridge::get_encryption_inplace(int *encryption) const {
    // crucial that this function is optimal. Used by Bombe
    copy(&m_plugboard->get_wiring()[0], &m_plugboard->get_wiring()[m_wires], encryption);
    if (!m_trivial_stator) { m_stator->encrypt_in_inplace(encryption, 0, m_wires); }
    for (int r= 0; r < m_rotor_count; ++r) {
        m_rotors[r]->encrypt_in_inplace(encryption, m_positions[r], m_wires);
    }
    m_reflector->encrypt_in_inplace(encryption, m_reflector_position, m_wires);
    for (int r= m_rotor_count - 1; r >= 0; --r) {
        m_rotors[r]->encrypt_out_inplace(encryption, m_positions[r], m_wires);
    }
    if (!m_trivial_stator) {   // stator, if nontrivial
        m_stator->encrypt_out_inplace(encryption, 0, m_wires);
    }
    m_plugboard->encrypt_inplace(encryption, m_wires);
}

// SETTERS
// SET value
void Cartridge::set_setting(struct EnigmaSetting setting) {
    m_rotor_count= (signed int)setting.rotors.size();   // TODO check if not 0
    m_wires      = setting.rotors[0].get_wires();       // begin?
    // dealloc everything
    for (int w= 0; w < m_rotor_count; w++) { delete m_rotors[w]; }
    delete[] m_rotors;
    delete m_reflector;
    delete m_plugboard;
    // alloc and set everything.
    m_rotors= new Rotor *[m_rotor_count];
    for (int r= 0; r < m_rotor_count; ++r) { m_rotors[r]= new Rotor(setting.rotors[r]); }
    m_reflector= new Reflector(setting.reflector);
    m_plugboard= new Plugboard(setting.plugboard);
    set_ring_setting(setting.ring_setting);
    set_rotor_position(setting.rotor_position);
}
void Cartridge::set_plugboard(const string str) { m_plugboard->set_wiring(str); }
void Cartridge::set_rotor(int pos, const Rotor *set) {
    m_rotors[pos]= (Rotor *)set;   // overwrites. rotor should handle it
                                   // properly
}
void Cartridge::set_reflector(const Reflector *set) {
    m_reflector= (Reflector *)set;   // overwrites. rotor should handle it properly
}
void Cartridge::set_positions(const int *p_in) {
    for (int p= 0; p < m_rotor_count; p++) { m_positions[p]= p_in[p]; }
}
void Cartridge::set_rotor_position(const string rotor_positions) {
    // rotor positions are the letters shown in the window of the enigma
    // actually sets positions, which depend on the ring setting
    // assumes string is all capital english letters, and that ring setting is
    // set
    unsigned int n= rotor_positions.length();
    if (n < (unsigned int)m_rotor_count || n > (unsigned int)m_rotor_count + 1) {
        cerr << "WARNING: position needs " << m_rotor_count << " or " << m_rotor_count + 1
             << " elements. rotor positions unchanged\n";
        return;
    }
    for (int p= 0; p < m_rotor_count; p++) {
        m_positions[p]=
            (((int)rotor_positions[n - p - 1] - (int)'A') - m_ring_setting[p] + m_wires) % m_wires;
    }
    if (n == (unsigned int)m_rotor_count + 1) {
        m_reflector_position= (int)rotor_positions[0] - (int)'A';
    }
}
void Cartridge::set_ring_setting(const int *p) { m_ring_setting= (int *)p; }
void Cartridge::set_ring_setting(const string in) {
    for (int i= 0; i < m_rotor_count; ++i) {
        m_ring_setting[i]= (int)(in[m_rotor_count - i - 1]) - (int)('A');
    }
}
void Cartridge::set_verbose(bool set) {
    // set verbose of self, and all my parts
    for (int w= 0; w < m_rotor_count; w++) { m_rotors[w]->set_verbose(set); }
    m_reflector->set_verbose(set);
    m_verbose= set;
}
// other
void Cartridge::reset_positions() {
    for (int p= 0; p < m_rotor_count; p++) { m_positions[p]= 0; }
    m_reflector_position= 0;
}
void Cartridge::reset_ring_setting() {
    for (int p= 0; p < m_rotor_count; p++) { m_ring_setting[p]= 0; }
}

/* unturning is not safe as turning is not an onto function
void Cartridge::turn(int turns) {
    // does not need to be optimized
    // single turn
    if (turns > 0) {
        for (int t= 0; t < turns; ++t) { turn(); }
    } else {
        for (int t= 0; t < -turns; ++t) {
            bool in_notch= false;   // previous_two_after_notch= false,
            // bool current_in_notch= false;
            // bool previous_turned = true;   //, previous_previous_turned=
            // false;
            bool carry= 1, carry_p;
            // bool next_after_notch= false;
            // m_positions[0]= (m_positions[0] - 1 + m_wires) % m_wires;
            string previous_position = get_rotor_position_as_string();
            string suggested_position= get_rotor_position_as_string();
            string found_position    = get_rotor_position_as_string();
            string unturned_position = get_rotor_position_as_string();
            for (int p= 0; p < m_rotor_count; ++p) {
                // unturn p
                m_positions[p]    = (m_positions[p] - 1 + m_wires) % m_wires;
                suggested_position= get_rotor_position_as_string();
                cout << "\nSUGGEST: " << suggested_position << "\n";
                // turn
                turn();
                unturned_position= get_rotor_position_as_string();
                cout << "RETURNED: " << unturned_position << "\n";
                // check
                if (unturned_position.substr(m_rotor_count - p - 1, p + 1) ==
                    previous_position.substr(m_rotor_count - p - 1, p + 1)) {
                    found_position= suggested_position;
                    cout << "YES\n";
                } else {
                    cout << "NO\n";
                }
                set_rotor_position(found_position);

                // use turn algorithm to turn only this letter
            }
            set_rotor_position(found_position);

            cout << "\nPREVIOUS: " << previous_position;
            cout << " -U-> " << found_position;
            string temp= found_position;
            turn();
            string rturn= get_rotor_position_as_string();
            set_rotor_position(temp);
            cout << " -T-> " << rturn << "\n";

        }   // FOR turn
    }       // ELSE turn>0
}*/ //UNSAFE!!!

void Cartridge::turn() {
    // single turn, should be optimal
    int  carry= 1, carry_p= 0;
    bool in_notch= false;
    for (int p= 0; p < m_rotor_count; p++) {
        for (int n= 0; n < m_rotors[p]->get_notches(); n++) {
            // optimize here
            in_notch= ((m_positions[p] + m_ring_setting[p]) % m_wires == m_rotors[p]->get_notch(n));
            if (in_notch) break;
        }
        m_positions[p]= (m_positions[p] + carry) % m_wires;
        carry_p       = carry;
        carry         = 0;
        if (in_notch) {
            if (carry_p != 1 && p != m_rotor_count - 1) {
                m_positions[p]= (m_positions[p] + 1) % m_wires;
            }
            carry= 1;
        }
    }
}
int Cartridge::plugboard_encrypt(int i) const { return m_plugboard->encrypt(i); }

int Cartridge::encrypt_without_turning(int i) const {
    vector<int> encryption_stepwise= encrypt_stepwise(i);
    return encryption_stepwise[encryption_stepwise.size() - 1];
}
vector<int> Cartridge::encrypt_stepwise(int i) const {
    // encrypt m, showing its value throughout encryption
    // Base method for encryption
    vector<int> path;
    path.push_back(i);
    i= m_plugboard->encrypt(i);
    path.push_back(i);
    if (!m_trivial_stator) {
        i= m_stator->encrypt_in(i, 0);
        path.push_back(i);
    }
    for (int rotor= 0; rotor < m_rotor_count; rotor++) {
        i= m_rotors[rotor]->encrypt_in(i, m_positions[rotor]);
        path.push_back(i);
    }
    i= m_reflector->encrypt_in(i, 0);
    path.push_back(i);
    for (int rotor= m_rotor_count - 1; rotor >= 0; rotor--) {
        i= m_rotors[rotor]->encrypt_out(i, m_positions[rotor]);
        path.push_back(i);
    }
    if (!m_trivial_stator) {
        i= m_stator->encrypt_out(i, 0);
        path.push_back(i);
    }
    i= m_plugboard->encrypt(i);
    path.push_back(i);
    return path;
}
void Cartridge::next_ring_setting() {
    // odometer style
    int carry= 1, next;
    for (int p= 0; p < m_rotor_count && carry > 0; p++) {
        next             = (m_ring_setting[p] + carry) % m_wires;
        next == 0 ? carry= 1 : carry= 0;
        m_ring_setting[p]           = next;
    }
}
void Cartridge::print() const {
    printf("  ");
    printf("  P ");
    for (int rotor= 0; rotor < m_rotor_count; rotor++) { printf("  W%d", rotor); }
    for (int wire= 0; wire < m_wires; wire++) {
        printf("\n%2d: ", wire);
        // print plugboard
        printf("%2d  ", m_plugboard->encrypt(wire));
        for (int rotor= 0; rotor < m_rotor_count; rotor++) {
            printf("%2d  ", m_rotors[rotor]->get_wiring_in(wire));
        }
    }
    printf("\n");
    return;
}
void Cartridge::print_positions() const {
    for (int p= 0; p < m_rotor_count; p++) { printf("%2d ", m_positions[p]); }
    printf("\n");
}
void Cartridge::randomize() {
    for (int w= 0; w < m_rotor_count; w++) { m_rotors[w]->randomize(); }
}

// ENIGMA ENGINE
Enigma::Enigma(int rotors_number, int wires) : m_rotors_number{rotors_number}, m_wires{wires} {
    m_cartridge= new Cartridge(rotors_number, wires);
}
Enigma::Enigma(const std::initializer_list<Rotor> rotors, const Reflector reflector) :
    Enigma(vector<Rotor>(rotors), reflector) {}
Enigma::Enigma(const vector<Rotor> rotors, const Reflector reflector) {
    m_cartridge    = new Cartridge(rotors, reflector);
    m_wires        = (rotors.begin())->get_wires();
    m_rotors_number= rotors.size();
}
Enigma::Enigma(struct EnigmaSetting setting) {
    m_rotors_number= (signed int)setting.rotors.size();   // TODO check if not 0
    m_wires        = setting.rotors[0].get_wires();
    m_cartridge    = new Cartridge(setting);
}
Enigma::~Enigma() { delete m_cartridge; }

// getter
struct EnigmaSetting Enigma::get_setting() {
    return m_cartridge->get_setting();
}
int        Enigma::get_wires() const { return m_wires; }
int        Enigma::get_rotors() const { return m_rotors_number; }
const int *Enigma::get_positions() const { return m_cartridge->get_positions(); }
/*const int *Enigma::get_rotor_position() const {
return m_cartridge->get_rotor_position();
}*/
const string Enigma::get_rotor_position_as_string() const {
    return m_cartridge->get_rotor_position_as_string();
}
const int *Enigma::get_ring_setting() const { return m_cartridge->get_ring_setting(); }
string     Enigma::get_ring_setting_as_string() const {
    return m_cartridge->get_ring_setting_as_string();
}

int *Enigma::get_encryption() const {
    // return encryption of all letters, trying to be cache coherent
    // return value must be deleted by client.
    // get stuff from the cartridge
    // XXX take identity, encrypt in place for containment of code
    const Rotor **   rotors    = m_cartridge->get_rotors();
    const Reflector *reflector = m_cartridge->get_reflector();
    const int *      positions = m_cartridge->get_positions();
    const Plugboard *plugboard = m_cartridge->get_plugboard();
    int *            encryption= new int[m_wires];
    for (int i= 0; i < m_wires; ++i) { encryption[i]= i; }

    plugboard->encrypt_inplace(encryption, m_wires);
    for (int r= 0; r < m_rotors_number; ++r) {
        rotors[r]->encrypt_in_inplace(encryption, positions[r], m_wires);
    }
    reflector->encrypt_in_inplace(encryption, m_cartridge->get_reflector_position(), m_wires);
    for (int r= m_rotors_number - 1; r >= 0; --r) {
        rotors[r]->encrypt_out_inplace(encryption, positions[r], m_wires);
    }
    plugboard->encrypt_inplace(encryption, m_wires);
    return encryption;
}
void Enigma::get_encryption_inplace(int *encryption) const {
    // return encryption of all letters, trying to be cache coherent
    // encryption is assumed to conatin m_wires integers
    // encrypts in place onto encryption, no allocation
    m_cartridge->get_encryption_inplace(encryption);
}
void Enigma::get_encryption_inplace_lazy(int *encryption) const {
    // return encryption of all letters
    // does the same as inplace, but with one less encryption
    // if only one wheel turns, use the previous encryption
    // input encryption must only differ in rotor 0 by one posisiton
    // e      =P W0^(-1) W1^(-1) W2^(-1) R W2 W1 W0 P m=>
    // W0 P e =          W1^(-1) W2^(-1) R W2 W1 W0 P m
    // XXX ignores stator, ok if trivial
    const Rotor **rotors= m_cartridge->get_rotors();
    // const Reflector *reflector= m_cartridge->get_reflector();
    const int *      positions= m_cartridge->get_positions();
    const Plugboard *plugboard= m_cartridge->get_plugboard();
    int *            indexes  = new int[m_wires];
    for (int i= 0; i < m_wires; ++i) { indexes[i]= i; }
    // peel away plug and rotor_0 encryption from the back(2 encryptions)
    plugboard->encrypt_inplace(encryption, m_wires);
    rotors[0]->encrypt_out_inplace(encryption, (positions[0] - 1 + m_wires) % m_wires, m_wires);
    // encryption is now W0 P e = W1^(-1) W2^(-1) R W2 W1 W0 P m
    // encrypt indexes to peel away rotor_0 and plugboard from front
    plugboard->encrypt_inplace(indexes, m_wires);
    rotors[0]->encrypt_in_inplace(indexes, (positions[0] - 1 + m_wires) % m_wires, m_wires);
    // indexes= W0 P
    // encrypt = W1^(-1) W2^(-1) R W2 W1 indexes, write to indexes, then copy
    // back to encrypt
    for (int i= 0; i < m_wires; ++i) { indexes[i]= encryption[indexes[i]]; }
    memcpy(encryption, indexes, m_wires * sizeof(int));
    // apply W0 with proper position
    for (int i= 0; i < m_wires; ++i) { indexes[i]= i; }
    rotors[0]->encrypt_out_inplace(encryption, positions[0], m_wires);
    plugboard->encrypt_inplace(encryption, m_wires);
    // encryption=P W0^(-1) W1^(-1) W2^(-1) R W2 W1, now apply this to W0 P m
    plugboard->encrypt_inplace(indexes, m_wires);
    rotors[0]->encrypt_in_inplace(indexes, positions[0], m_wires);
    for (int i= 0; i < m_wires; ++i) { indexes[i]= encryption[indexes[i]]; }
    memcpy(encryption, indexes, m_wires * sizeof(int));

    delete[] indexes;
}
string Enigma::get_encryption_as_string() const {
    int *  encryption= get_encryption();
    string out       = "";
    for (int i= 0; i < m_wires; ++i) { out+= (char)(encryption[i] + (int)'A'); }
    return out;
}
vector<pair<int, int>> Enigma::get_encryption_onesided() const {   // TODO &
    // return encryption at current step
    // encrypt all letters
    // this one is hardcoded to circumvent verbose branches, and avoid
    // double work due to symmetry
    // init
    vector<int>            flags(m_wires, false);
    vector<pair<int, int>> wires;
    // get stuff from the cartridge
    const Rotor **   rotors   = m_cartridge->get_rotors();
    const Reflector *reflector= m_cartridge->get_reflector();
    const int *      positions= m_cartridge->get_positions();
    // for each letter, encrypt, unless already encrypted
    int m;
    for (int i= 0; i < m_wires; i++) {
        if (!flags.at(i)) {
            // encrypt i
            m= i;
            // plugboard in
            m= m_cartridge->plugboard_encrypt(m);
            // forward
            for (int j= 0; j < m_rotors_number; j++) { m= rotors[j]->encrypt_in(m, positions[j]); }
            // reflect
            m= reflector->encrypt_in(m, m_cartridge->get_reflector_position());
            // backward
            for (int j= m_rotors_number - 1; j >= 0; j--) {
                m= rotors[j]->encrypt_out(m, positions[j]);
            }
            // plugboard out
            m= m_cartridge->plugboard_encrypt(m);
            // set
            wires.push_back(make_pair(i, m));
            flags.at(i)= true;
            flags.at(m)= true;
        }
    }
    return wires;
}
Cartridge *Enigma::get_cartridge() const { return m_cartridge; }
// setter
void Enigma::set_setting(struct EnigmaSetting setting) {
    m_rotors_number= (signed int)setting.rotors.size();   // TODO check if not 0
    m_wires        = setting.rotors[0].get_wires();
    m_cartridge->set_setting(setting);
}
void Enigma::set_coder() {
    // set code for language
}
void Enigma::set_verbose(bool set) {   // XXX bretter impl
    // m_cartridge->set_verbose(false);
    m_verbose= set;
}
void Enigma::set_cartridge_verbose(bool set) {   // XXX better impl
    m_cartridge->set_verbose(set);
}
void Enigma::set_positions(const int *in) { m_cartridge->set_positions(in); }
void Enigma::set_rotor_position(const string in) { m_cartridge->set_rotor_position(in); }
void Enigma::set_ring_setting(const string in) { m_cartridge->set_ring_setting(in); }
void Enigma::set_ring_setting(const int *in) { m_cartridge->set_ring_setting(in); }
void Enigma::set_plugboard(const string str) { m_cartridge->set_plugboard(str); }
// other
void Enigma::randomize() { m_cartridge->randomize(); }
void Enigma::reset() { m_cartridge->reset_positions(); }
// void Enigma::turn(int turns) { m_cartridge->turn(turns); }
void Enigma::turn() { m_cartridge->turn(); }
int  Enigma::encrypt(int m) {
    m_cartridge->turn();
    // cout<<"encr "<<m<<"\n";

    /*if (m_verbose && m_verbose_wheels) {}
    if (m_verbose) {
        cout << (char)(i + (int)'A') << " ------> ";
        for (int j= 0; j < m_wires; j++) { cout << (char)(j + (int)'A'); }
        cout << "\n";
    }   // plug
    i= m_plugboard->encrypt(i);
    // forward pass
    for (int rotor= 0; rotor < m_rotor_count; rotor++) {
        if (m_verbose) {
            cout << "      W" << rotor << "(" << (char)(i + (int)'A') << ")-> ";
        }

        i= m_rotors[rotor]->encrypt_in(i, m_positions[rotor]);

        if (m_verbose) {
            cout << " ["
                 << get_rotor_position_as_string()[m_rotor_count - rotor]
                 << "]\n";
        }
    }
    // reflector
    if (m_verbose) {
        cout << "      R"
             << "(" << (char)(i + (int)'A') << ")--> ";
    }

    i= m_reflector->encrypt_in(i, 0);

    if (m_verbose) {
        cout << " " << (char)(m_reflector_position + (int)'A') << "\n";
    }
    // backward pass
    for (int rotor= m_rotor_count - 1; rotor >= 0; rotor--) {
        if (m_verbose) {
            cout << "      W" << rotor << "(" << (char)(i + (int)'A') << ")-> ";
        }

        i= m_rotors[rotor]->encrypt_out(i, m_positions[rotor]);

        if (m_verbose) {
            cout << " ["
                 << get_rotor_position_as_string()[m_rotor_count - rotor]
                 << "]\n";
        }
    }
    // plug back
    i= m_plugboard->encrypt(i);
    return i;*/

    if (m_verbose) {
        // set parts to non-verbose
        m_cartridge->set_verbose(false);
    }

    int c= encrypt_without_turning(m);

    if (m_verbose) {
        cout << (char)(m + (int)'A') << " ->";
        if (m == 0) {
            cout << "[";
        } else {
            cout << " ";
        }
        // encrypt all possible letters and print
        for (int i= 0; i < m_wires; i++) {
            cout << (char)(m_cartridge->encrypt_without_turning(i) + (int)'A');
            if (i == m || m == m_wires) {
                cout << "]";
            } else if (i == m - 1) {
                cout << "[";
            } else {
                cout << " ";
            }
        }
        // print posiions
        cout << " --- " << get_rotor_position_as_string();
        cout << " --- " << get_ring_setting_as_string();
        cout << " --- " << m_cartridge->get_positions_as_string();
        cout << "\n";
    }

    return c;
}

const std::string wc("\033[0;31m");
const std::string rc("\033[0;33m");
const std::string gc("\033[1;30m");
const std::string bgc("\033[0;40m");
const std::string bgr("\033[0;40m");

string line_begin(int left_in, int left_out, int wire, int pos= -1, int wires= 26) {
    string line= "";
    line+=
        (left_in == wire) ? wc + "-->--+" + rc : (left_out == wire) ? wc + "--<--+" + rc : "     |";
    if (pos != -1) {
        line+= (char)((pos + wire + wires) % wires + (int)'A');
    } else {
        line+= " ";
    }
    // line+= "|";
    //----PLUG START
    line+= (left_in == wire || left_out == wire) ? wc + "+" + rc : "|";
    line+= ((left_in == wire || left_out == wire) ? wc + "-" + rc : " ");
    return line;
}
string line_mid(int left_in, int right_in, int left_out, int right_out, int wire) {
    string line= "";
    if (((left_in == right_in) && left_in == wire) || ((left_out == right_out) && left_out == wire))
        return wc + "---" + rc;
    // first column
    if (wire >= min(left_in, right_in) && wire <= max(left_in, right_in)) {
        if (wire == left_out) {
            line+= wc + "+" + rc;
        } else {
            line+= wc + "|" + rc;
        }
    } else if (wire == left_out) {
        line+= wc + "-" + rc;
    } else {
        line+= " ";
    }
    // second column
    if (wire == left_out) {
        line+= wc + "-" + rc;
    } else if (wire == right_in) {
        line+= wc + "-" + rc;
    } else {
        line+= " ";
    }
    // third column
    if (wire >= min(left_out, right_out) && wire <= max(left_out, right_out)) {
        if (wire == right_in) {
            line+= wc + "+" + rc;
        } else {
            line+= wc + "|" + rc;
        }
    } else if (wire == right_in) {
        line+= wc + "-" + rc;
    } else {
        line+= " ";
    }

    return line;
}
string line_end(int right_in, int right_out, int wire, int pos, int wires, int notches,
                const int *notch, int rs) {
    string line= "";
    // CHECK IF NOTCH
    bool notched= false;
    for (int n= 0; n < notches; ++n) {
        if ((wire + notch[n]) % wires == (-pos + rs + wires) % wires) {
            notched= true;
            break;
        }
    }

    line+= (right_in == wire || right_out == wire) ? wc + "-+" + rc : " |";
    line+= notched ? "X" : " ";
    line+= (right_in == wire || right_out == wire) ? wc + "+" + rc : "|";
    return line;
}
string line_all(int left_in, int right_in, int left_out, int right_out, int wire, int pos= -1,
                int notches= 0, const int *notch= {0}, int rs= 0, int wires= 26) {

    return line_begin(left_in, left_out, wire, pos, wires) +
           line_mid(left_in, right_in, left_out, right_out, wire) +
           line_end(right_in, right_out, wire, pos, wires, notches, notch, rs);
}
int Enigma::encrypt_verbose_exploded(int m) const {
    // show exploded view of the cartridge, and the pathway m takes in
    const bool trivial_stator= m_cartridge->get_if_trivial_stator();
    string     line, total;
    m_cartridge->turn();
    vector<int> path= m_cartridge->encrypt_stepwise(m);
    int         pl  = (int)path.size();

    // DESC
    line= "\n   ";
    line+= "      PLUGBOARD  ";
    if (!trivial_stator) line+= "     STATOR    ";
    for (int i= 0; i < m_rotors_number; ++i) { line+= "      ROTOR " + to_string(i) + "   "; }
    line+= "    REFLECTOR  \n";
    total+= line;
    // top rotorline 1
    string outerline= "        ";
    string unit     = "-----------     ";
    outerline+= unit;   // PLUG TOP
    if (!trivial_stator) {
        outerline+= unit;   // STATOR TOP
    }
    for (int r= 0; r < m_rotors_number; ++r) { outerline+= unit; /*ROTOR TOP*/ }
    outerline+= "---------  \n";   // REFLECTOR TOP
    total+= outerline;
    // top rotor line 2
    string midline= "        ";
    unit          = "| ------- |     ";
    midline+= unit;   // PLUG TOP
    if (!trivial_stator) {
        midline+= unit;   // STATOR TOP
    }
    for (int r= 0; r < m_rotors_number; ++r) { midline+= unit; /*ROTOR TOP*/ }
    midline+= "| ----- |   \n";   // REFLECTOR TOP
    total+= midline;

    // ROTORS TOP
    for (int wire= 0; wire < m_wires; ++wire) {
        line= "";
        if (wire == path[0]) {
            line+= wc + (char)(wire + (int)'A') + "->" + rc;
        } else if (wire == path[pl - 1]) {
            line+= wc + (char)(wire + (int)'A') + "-<" + rc;
        } else {
            line+= rc + ((char)(wire + (int)'A')) + rc + "  ";
        }
        // PLUGBOARD
        line+= line_all(path[0], path[1], path[pl - 1], path[pl - 2], wire);
        if (!trivial_stator) {
            // STATOR
            line+= line_all(path[1], path[2], path[pl - 2], path[pl - 3], wire);
        }
        // ROTORS
        for (int r= 0; r < m_rotors_number; ++r) {
            const Rotor *rotor= m_cartridge->get_rotors()[r];
            int          i    = 1 + (trivial_stator ? 0 : 1) + r;
            int          j    = pl - i - 2;   // from other dir
            line+= line_all(path[i], path[i + 1], path[j + 1], path[j], wire,
                            get_rotor_position_as_string()[r] - (int)'A', rotor->get_notches(),
                            rotor->get_notch(), get_ring_setting()[r], m_wires);
        }
        // REFLECTOR
        int i= pl / 2 - 1;
        line+= line_begin(path[i], path[i + 1], wire, -1, m_wires);
        line+= (wire >= min(path[i], path[i + 1]) && wire <= max(path[i], path[i + 1]))
                   ? wc + "| " + rc
                   : "  ";
        //----REFLECTOR END
        line+= "| |";
        total+= line + "\n";
    }
    cout << total + midline + outerline;
    cout << get_rotor_position_as_string();
    return path[pl - 1];
}
int Enigma::encrypt_without_turning(int m) const { return m_cartridge->encrypt_without_turning(m); }
int *Enigma::encrypt(const int *m, int n) {
    // encrypt an array of ints
    // return value must be freed!
    if (m_verbose && !m_verbose_exploded) {
        cout << "\n              ";
        for (int i= 0; i < m_wires; i++) { cout << (char)(i + (int)'A') << " "; }
        cout << "    R.POS.  R.SET.";
        cout << "\n              ";
        for (int i= 0; i < m_wires; i++) { cout << "| "; }
        cout << "     |||     |||  ";
        cout << "\n";
    }
    int *e= new int[n];   // must be freed outside
    for (int i= 0; i < n; i++) {
        if (m_verbose) { printf("m[%3d] = ", i); }
        if (m_verbose_exploded) {
            e[i]= encrypt_verbose_exploded(m[i]);
        } else {
            e[i]= encrypt(m[i]);
        }
    }
    return e;
}
void Enigma::encrypt(istream &in, ostream &out) {
    // read in line for line, format line, encrypt and write to ostream.
    string line;
    while (getline(in, line)) { out << encrypt(preprocess(line)) << "\n"; }
}
string Enigma::preprocess(string in) const {
    // remove everything that is not an alpha
    std::regex nonalpha("[^a-zA-Z]");
    in= regex_replace(in, nonalpha, "");
    // convert string to upper case
    std::for_each(in.begin(), in.end(), [](char &c) { c= ::toupper(c); });
    return in;
}
string Enigma::encrypt(string str) {
    // string to int*
    int *m= new int[str.length()];
    for (unsigned int i= 0; i < str.length(); ++i) { m[i]= (int)str[i] - (int)'A'; }
    int *e= encrypt(m, str.length());   // handled here
    // int to string
    string out= "";
    for (unsigned int i= 0; i < str.length(); ++i) { out+= (char)(e[i] + (int)'A'); }
    delete[] e;
    delete[] m;
    return out;
}
void   Enigma::next_ring_setting() { m_cartridge->next_ring_setting(); }
void   Enigma::print_positions() const { m_cartridge->print_positions(); }
void   Enigma::print() const { m_cartridge->print(); }
string Enigma::indicator_procedure_early(string str) {
    string out= encrypt(str);
    out+= encrypt(str);
    set_rotor_position(str);
    return out;
}
string Enigma::indicator_procedure_WW2(string start_pos, string message_key) {

    set_rotor_position(start_pos);
    // m_cartridge->set_verbose(true);
    string encrypted_message_key= encrypt(message_key);
    set_rotor_position(message_key);
    // str_1 has to have length equal to amount of wheels

    // string out=encrypt(str);
    // out+=encrypt(str);
    // set_rotor_position(str);
    return start_pos + encrypted_message_key;
}
vector<vector<shint>> Enigma::get_all_rotor_positions() {
    // cout << "get all positions\n";
    vector<vector<shint>> rotor_positions;
    vector<shint>         rotor_position;
    string                initial_ring_setting  = get_ring_setting_as_string();
    string                initial_rotor_position= get_rotor_position_as_string();
    string                set                   = "";
    for (int i= 0; i < m_rotors_number; ++i) { set+= "A"; }
    set_ring_setting(set);
    for (int t= 0; t < m_wires; ++t) { turn(); }   // to ensure not in a impossible rotor posiiton
    string first_rotor_position= get_rotor_position_as_string();
    rotor_position.clear();
    for (char c : get_rotor_position_as_string()) { rotor_position.push_back((shint)c - (int)'A'); }
    rotor_positions.push_back(rotor_position);
    turn();
    int turns= 1;

    while (get_rotor_position_as_string() != first_rotor_position) {
        rotor_position.clear();
        for (char c : get_rotor_position_as_string()) {
            rotor_position.push_back((shint)c - (int)'A');
        }
        rotor_positions.push_back(rotor_position);
        turn();
        turns++;
    }
    // reset state
    set_ring_setting(initial_ring_setting);
    set_rotor_position(initial_rotor_position);
    // cout << "\ngot all positions\n";
    return rotor_positions;
}

int Enigma::compute_total_permutations_brute_force() {
    // get setting, turn until we have the same setting
    // cout << "BRUTE FORCE PERMUTATIONS\n";
    string initial_rotor_position= get_rotor_position_as_string();
    turn();
    int turns= 1;
    while (get_rotor_position_as_string() != initial_rotor_position) {

        // cout << "BRUTE FORCE PERMUTATIONS       " << get_rotor_position_as_string() << " - "
        //     << initial_rotor_position << "\n";
        turn();
        turns++;
        if (turns > pow(m_wires, m_rotors_number)) {
            initial_rotor_position= get_rotor_position_as_string();
            turn();
            turns= 1;
        }
    }
    return turns;
}
/*
int main() {
    const Rotor I=                 Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
    const Rotor II=                Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
    const Rotor III=               Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
    const Reflector UKWR=          Reflector("QYHOGNECVPUZTFDJAXWMKISRBL");
//ref Enigma enigma({I, II, III}, UKWR); enigma.set_verbose(true);
    //enigma.set_plugboard("AB.CD.EF.GH.IJ.KL.MN.OP.QR.ST");
    cout<<enigma.encrypt("THISISAPLAINTEXTMESSAGETOBEENCIPHEREDWITHTHEENIGMAANDITISVERYVERYLONG");
}
*/

// 16900=26*25*26; 422500=26*25*25*26
// 8112 I VII III 26*24*26/2
// ok makes sense
