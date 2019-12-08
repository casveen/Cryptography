#include "enigma.h"
using namespace std;

// WHEEL
Rotor::Rotor() {}
Rotor::Rotor(int wires) : m_wires{wires} {
    m_notches = 1;
    m_notch   = new int[m_notches];
    m_notch[0]= 0;   // corresponds to notch at A
    // allocate to wiring array
    m_wiring_in = new int[m_wires];
    m_wiring_out= new int[m_wires];
    // make a legal wiring, essentially a substitution cipher
    for (int j= 0; j < m_wires; j++) {
        m_wiring_in[j] = j;
        m_wiring_out[j]= j;
    }
}
Rotor::Rotor(const string in) {
    m_notches = 1;
    m_notch   = new int[m_notches];
    m_notch[0]= 0;   // corresponds to notch at A
    m_wires   = in.length();
    // allocate to wiring array
    m_wiring_in = new int[m_wires];
    m_wiring_out= new int[m_wires];
    int wire;
    for (int i= 0; i < m_wires; i++) {
        wire              = (int)in[i] - (int)'A';
        m_wiring_in[i]    = wire;
        m_wiring_out[wire]= i;
    }
}
Rotor::Rotor(const string in, const string notch) : Rotor(in) {
    m_notches= notch.length();
    delete[] m_notch;   // XXX Not ood solution
    m_notch= new int[m_notches];
    for (int i= 0; i < m_notches; i++) { m_notch[i]= (int)notch[i] - (int)'A'; }
}
/*constexpr Rotor::Rotor(const string in, const string notch): Rotor(in) {
    m_notches=   notch.length();
    m_notch=     new int[m_notches];
    m_wires=     in.length();
    //allocate to wiring array
    m_wiring_in= new int[m_wires];
    m_wiring_out=new int[m_wires];
    int wire;
    for(int i=0; i<m_wires; i++)  {
        wire=(int) in[i]-(int) 'A';
        m_wiring_in[i] =wire;
        m_wiring_out[wire]=i;
    }
    for(int i=0; i<m_notches; i++)  {
        m_notch[i]=(int) notch[i]-(int) 'A';
    }
}*/
Rotor::~Rotor() {
    delete[] m_wiring_in;
    delete[] m_wiring_out;
    delete[] m_notch;
}
Rotor::Rotor(Rotor const &copy) {
    // cout << "COPYING ROTOR\n";
    // Copy constructor, very important, assures there is no shallow copy
    // src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    m_wires     = copy.m_wires;
    m_notches   = copy.m_notches;
    m_wiring_in = new int[m_wires];
    m_wiring_out= new int[m_wires];
    m_notch     = new int[m_notches];
    // Don't need to worry about copying integers.
    // But if the object has a copy constructor then
    // it would also need to worry about throws from the copy constructor.
    std::copy(&copy.m_wiring_in[0], &copy.m_wiring_in[m_wires], m_wiring_in);
    std::copy(&copy.m_wiring_out[0], &copy.m_wiring_out[m_wires], m_wiring_out);
    std::copy(&copy.m_notch[0], &copy.m_notch[m_notches], m_notch);
}
Rotor &Rotor::operator=(Rotor rhs) {
    // Pass by value (thus generating a copy)
    // cout << "ASSIGN\n";
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
// setters
void Rotor::set_wiring_in(int pos, int set) { m_wiring_in[pos]= set; }
void Rotor::set_wiring_out(int pos, int set) { m_wiring_out[pos]= set; }
void Rotor::set_verbose(bool set) { m_verbose= set; }
// other
inline int Rotor::encrypt_in(int i, int offset) const {
    /*if (m_verbose == true) {
        for (int j= 0; j < m_wires; j++) {
            if (j == i) { cout << "("; }
            cout << (char)((m_wiring_in[(j + offset) % m_wires] + m_wires -
                            offset) %
                               m_wires +
                           (int)'A');
            if (j == i) { cout << ")"; }
        }
    }*/
    return (m_wiring_in[(i + offset) % m_wires] + m_wires - offset) % m_wires;
}
inline int Rotor::encrypt_out(int i, int offset) const {
    /*if (m_verbose) {
for (int j= 0; j < m_wires; j++) {
    if (j == out) { cout << "("; }
    cout << (char)((m_wiring_out[(j + offset) % m_wires] + m_wires -
                    offset) %
                       m_wires +
                   (int)'A');
    if (j == out) { cout << ")"; }
}
}*/
    return (m_wiring_out[(i + offset) % m_wires] + m_wires - offset) % m_wires;
}
void Rotor::encrypt_in_inplace(int *plaintext, int offset, int n) const {
    for (int i= 0; i < n; ++i) {
        plaintext[i]= (m_wiring_in[(plaintext[i] + offset) % m_wires] +
                       m_wires - offset) %
                      m_wires;
    }
}
void Rotor::encrypt_out_inplace(int *plaintext, int offset, int n) const {
    for (int i= 0; i < n; ++i) {
        plaintext[i]= (m_wiring_out[(plaintext[i] + offset) % m_wires] +
                       m_wires - offset) %
                      m_wires;
    }
}

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
    for (int wire= 0; wire < m_wires; wire++) {
        printf("%2d ", m_wiring_in[wire]);
    }
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
Reflector::Reflector() : Rotor() {}
Reflector::Reflector(int wires) : Rotor(wires) {
    for (int j= 0; j < wires; j++) { m_wiring_in[j]= j + 1 - 2 * (j % 2); }
}
Reflector::Reflector(string wiring) : Rotor(wiring) {}
Reflector::Reflector(string wiring, string notches) : Rotor(wiring, notches) {}
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
Plugboard::Plugboard(int t_wires) {
    // allocate to wiring array
    m_wires = t_wires;
    m_wiring= vector<int>(m_wires);
    for (int i= 0; i < m_wires; i++) { m_wiring[i]= i; }
}
Plugboard::Plugboard(const string in, int t_wires) : Plugboard{t_wires} {
    set_wiring(in);
}
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
inline int Plugboard::encrypt(int in) const { return m_wiring[in]; }
void       Plugboard::encrypt_inplace(int *plaintext, int n) const {
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
void        Plugboard::set_wiring(int pos, int set) { m_wiring[pos]= set; }
vector<int> Plugboard::get_wiring() const { return m_wiring; }
int         Plugboard::get_wiring(int i) const { return m_wiring[i]; }

// CARTRIDGE
Cartridge::Cartridge() {}   // XXX should really not be neccesary...
Cartridge::Cartridge(int rotor_count, int wires) :
    m_rotor_count{rotor_count}, m_wires{wires} {
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
Cartridge::Cartridge(const std::initializer_list<Rotor> rotors,
                     Reflector                          reflector) {
    m_wires       = (rotors.begin())->get_wires();
    m_rotor_count = rotors.size();
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
Cartridge::Cartridge(Cartridge const &copy) {
    // src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    m_wires      = copy.m_wires;
    m_rotor_count= copy.m_rotor_count;
    m_rotors     = new Rotor *[m_rotor_count];
    // m_plugboard         =new Plugboard();
    cout << "\n\n\n\n\nWOOOOOOPS\n\n\n\n\n";
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
}
Cartridge &Cartridge::operator=(
    Cartridge rhs) {   // Pass by value (thus generating a copy)
    rhs.swap(*this);   // Now swap data with the copy.
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
}
// getters
struct EnigmaSetting Cartridge::get_setting() const {
    // these are pointers, should be COPIED if used by other cartridge
    struct EnigmaSetting out;
    for (int w= 0; w < m_rotor_count; ++w) {
        out.rotors.push_back(m_rotors[w]);
    }
    out.reflector     = m_reflector;
    out.plugboard     = m_plugboard;
    out.ring_setting  = get_ring_setting_as_string();
    out.rotor_position= get_positions_as_string();
    return out;
}
const Rotor **Cartridge::get_rotors() const { return (const Rotor **)m_rotors; }
const Reflector *Cartridge::get_reflector() const { return m_reflector; }
const int *      Cartridge::get_ring_setting() const { return m_ring_setting; }
const string     Cartridge::get_ring_setting_as_string() const {
    string out= "";
    for (int i= 0; i < m_rotor_count; i++) {
        out+= (char)(m_ring_setting[i] + (int)'A');
    }
    return out;
}
const int *Cartridge::get_positions() const { return m_positions; }
int        Cartridge::get_positions_as_int() const {
    int turn= 0;
    int mult= 1;
    for (int p= 0; p < m_rotor_count; p++) {
        turn+= m_positions[p] * mult;
        mult*= m_wires;
    }
    return turn;
}
const string Cartridge::get_positions_as_string() const {
    string out;
    out= "";
    for (int w= 0; w < m_rotor_count; w++) {
        out+= (char)(m_positions[w] + (int)'A');
    }
    return out;
}
int Cartridge::get_reflector_position() const { return m_reflector_position; }
Plugboard *Cartridge::get_plugboard() const { return m_plugboard; }
// setters
void Cartridge::set_setting(struct EnigmaSetting setting) {
    m_rotor_count= (signed int)setting.rotors.size();   // TODO check if not 0
    m_wires      = setting.rotors[0]->get_wires();      // begin?
    // dealloc everything
    for (int w= 0; w < m_rotor_count; w++) { delete m_rotors[w]; }
    delete[] m_rotors;
    delete m_reflector;
    // delete[] m_positions; //OKAY NOT GOOD!!!
    // delete[] m_ring_setting; //XXX does not handle if init in other size
    delete m_plugboard;
    // alloc and set everything.
    m_rotors= new Rotor *[m_rotor_count];
    for (int r= 0; r < m_rotor_count; ++r) {
        m_rotors[r]= new Rotor(*setting.rotors[r]);
    }
    m_reflector= new Reflector(*setting.reflector);
    m_plugboard= new Plugboard(*setting.plugboard);
    set_positions(setting.rotor_position);
    set_ring_setting(setting.ring_setting);
}
void Cartridge::set_plugboard(const string str) {
    m_plugboard->set_wiring(str);
}
void Cartridge::set_rotor(int pos, const Rotor *set) {
    m_rotors[pos]= (Rotor *)set;   // overwrites. rotor should handle it
                                   // properly
}
void Cartridge::set_reflector(const Reflector *set) {
    m_reflector=
        (Reflector *)set;   // overwrites. rotor should handle it properly
}
void Cartridge::set_positions(const int *p_in) {
    for (int p= 0; p < m_rotor_count; p++) { m_positions[p]= p_in[p]; }
}
void Cartridge::set_positions(const string in) {
    // assumes string is all capital english letters
    if (in.length() < (unsigned int)m_rotor_count ||
        in.length() > (unsigned int)m_rotor_count + 1) {
        cout << "ERROR: position needs " << m_rotor_count << " or "
             << m_rotor_count + 1 << " elements\n";
        return;
    }
    for (int p= 0; p < m_rotor_count; p++) {
        m_positions[p]= (int)in[p] - (int)'A';
    }
    if (in.length() == (unsigned int)m_rotor_count + 1) {
        m_reflector_position= (int)in[m_rotor_count + 1] - (int)'A';
    }
}
void Cartridge::set_ring_setting(const int *p) { m_ring_setting= (int *)p; }
void Cartridge::set_ring_setting(const string in) {
    for (int i= 0; i < m_rotor_count; i++) {
        m_ring_setting[i]= (int)(in[i]) - (int)('A');
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
void Cartridge::turn(int turns) {
    // single turn
    if (turns > 0) {
        for (int t= 0; t < turns; ++t) { turn(); }
    } else {
        for (int t= 0; t < -turns; ++t) {
            int next, carry= -1;
            for (int p= 0; p < m_rotor_count && abs(carry) > 0; p++) {
                next= (m_positions[p] + carry + m_wires) % m_wires;
                // check m_notch
                carry= 0;
                // cout<<"p="<<p<<"\n";
                for (int n= 0; n < m_rotors[p]->get_notches();
                     n++) {   // add one carry per notch passed
                    if (m_rotors[p]->get_notch(n) ==
                        (next + m_ring_setting[p] + m_wires + 1) %
                            m_wires) {   // carry only if on notch and moving
                        carry= -1;
                    }
                }
                m_positions[p]= next;
                // carry=(int) next/m_wires; //only carry if exceeded a notch -
                // ring setting[p]
            }
        }
    }
}
void Cartridge::turn() {
    // single turn
    int carry= 1, next;
    for (int p= 0; p < m_rotor_count && carry > 0; p++) {
        next= m_positions[p] + carry;
        // check m_notch
        carry= 0;
        for (int n= 0; n < m_rotors[p]->get_notches(); n++) {
            if (m_rotors[p]->get_notch(n) ==
                (m_positions[p] + m_ring_setting[p] + 1) %
                    m_wires) {   // carry only if on notch and moving
                carry= 1;
            }
        }
        m_positions[p]= next % m_wires;
        // carry=(int) next/m_wires; //only carry if exceeded a notch - ring
        // setting[p]
    }
}
int Cartridge::plugboard_encrypt(int i) const {
    return m_plugboard->encrypt(i);
}
int Cartridge::encrypt_without_turning(int i) const {
    if (m_verbose) {
        cout << (char)(i + (int)'A') << " ------> ";
        // printf("%s ------> ", );
        for (int j= 0; j < m_wires; j++) { cout << (char)(j + (int)'A'); }
        cout << "\n";
    }   // plug
    // cout<<i<<" - > \n";
    i= m_plugboard->encrypt(i);
    // cout<<i<<"\n";
    // cout<<"OUTSIDE PLUG\n";
    // forward pass
    for (int rotor= 0; rotor < m_rotor_count; rotor++) {
        // m_rotors[rotor]->print();
        if (m_verbose) {
            cout << "  W" << rotor << "(" << (char)(i + (int)'A') << ")-> ";
        }
        // cout<<i<<" - > \n";
        i= m_rotors[rotor]->encrypt_in(i, m_positions[rotor]);
        // cout<<i<<"\n";
        if (m_verbose) {
            cout << " " << (char)(m_positions[rotor] + (int)'A') << "\n";
        }
        //
    }
    // reflector
    // i=(m_reflector->get_wiring_in((i+m_reflector_position)%m_wires)+m_wires-m_reflector_position)%m_wires;
    if (m_verbose) {
        cout << "  R"
             << "(" << (char)(i + (int)'A') << ")--> ";
    }
    i= m_reflector->encrypt_in(i, 0);
    if (m_verbose) {
        cout << " " << (char)(m_reflector_position + (int)'A') << "\n";
    }
    // backward pass
    for (int rotor= m_rotor_count - 1; rotor >= 0; rotor--) {
        if (m_verbose) {
            cout << "  W" << rotor << "(" << (char)(i + (int)'A') << ")-> ";
        }
        i= m_rotors[rotor]->encrypt_out(i, m_positions[rotor]);
        if (m_verbose) {
            cout << " " << (char)(m_positions[rotor] + (int)'A') << "\n";
        }
        // i=(m_rotors[rotor]->get_wiring_out((i+m_positions[rotor])%m_wires)+m_wires-m_positions[rotor])%m_wires;
    }
    // plug back
    i= m_plugboard->encrypt(i);
    return i;
}
void Cartridge::next_ring_setting() {
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
    for (int rotor= 0; rotor < m_rotor_count; rotor++) {
        printf("  W%d", rotor);
    }
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
};
void Cartridge::print_positions() const {
    for (int p= 0; p < m_rotor_count; p++) { printf("%2d ", m_positions[p]); }
    printf("\n");
}
void Cartridge::randomize() {
    for (int w= 0; w < m_rotor_count; w++) { m_rotors[w]->randomize(); }
}

// ENIGMA ENGINE
Enigma::Enigma(int rotors_number, int wires) :
    m_rotors_number{rotors_number}, m_wires{wires} {
    m_cartridge= new Cartridge(rotors_number, wires);
}
Enigma::Enigma(const std::initializer_list<Rotor> rotors,
               const Reflector                    reflector) {
    m_cartridge    = new Cartridge(rotors, reflector);
    m_wires        = (rotors.begin())->get_wires();
    m_rotors_number= rotors.size();
}
Enigma::~Enigma() { delete m_cartridge; }
// getter
struct EnigmaSetting Enigma::get_setting() {
    return m_cartridge->get_setting();
}
int        Enigma::get_wires() const { return m_wires; }
int        Enigma::get_rotors() const { return m_rotors_number; }
const int *Enigma::get_rotor_position() const {
    return m_cartridge->get_positions();
}
const string Enigma::get_rotor_position_as_string() const {
    return m_cartridge->get_positions_as_string();
}
const int *Enigma::get_ring_setting() const {
    return m_cartridge->get_ring_setting();
}
string Enigma::get_ring_setting_as_string() const {
    return m_cartridge->get_ring_setting_as_string();
}
int *Enigma::get_encryption() const {
    // return encryption of all letters, trying to be cache coherent
    // return value must be deleted by client.
    // get stuff from the cartridge
    const Rotor **   rotors    = m_cartridge->get_rotors();
    const Reflector *reflector = m_cartridge->get_reflector();
    const int *      positions = m_cartridge->get_positions();
    const Plugboard *plugboard = m_cartridge->get_plugboard();
    int *            encryption= new int[m_wires];
    for (int i= 0; i < m_wires; ++i) { encryption[i]= i; }
    // plugboard
    // cout << "plug in\n";
    plugboard->encrypt_inplace(encryption, m_wires);
    for (int r= 0; r < m_rotors_number; ++r) {
        // cout << "rotor " << r << " in\n";
        rotors[r]->encrypt_in_inplace(encryption, positions[r], m_wires);
    }
    // cout << "reflector\n";
    reflector->encrypt_in_inplace(
        encryption, m_cartridge->get_reflector_position(), m_wires);
    for (int r= m_rotors_number - 1; r >= 0; --r) {
        // cout << "rotor " << r << " out\n";
        rotors[r]->encrypt_out_inplace(encryption, positions[r], m_wires);
    }
    // cout << "plug out\n";
    plugboard->encrypt_inplace(encryption, m_wires);
    return encryption;
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
            for (int j= 0; j < m_rotors_number; j++) {
                m= rotors[j]->encrypt_in(m, positions[j]);
            }
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
    m_wires        = setting.rotors[0]->get_wires();
    m_cartridge->set_setting(setting);
}
void Enigma::set_coder() {
    // set code for language
}
void Enigma::set_verbose(bool set) {   // XXX bretter impl
    // m_cartridge->set_verbose(false);
    m_verbose= set;
}
void Enigma::set_cartridge_verbose(bool set) {   // XXX bretter impl
    m_cartridge->set_verbose(set);
}
void Enigma::set_rotor_position(const int *in) {
    m_cartridge->set_positions(in);
}
void Enigma::set_rotor_position(const string in) {
    m_cartridge->set_positions(in);
}
void Enigma::set_ring_setting(const string in) {
    m_cartridge->set_ring_setting(in);
}
void Enigma::set_ring_setting(const int *in) {
    m_cartridge->set_ring_setting(in);
}
void Enigma::set_plugboard(const string str) {
    m_cartridge->set_plugboard(str);
}
// other
void Enigma::randomize() { m_cartridge->randomize(); }
void Enigma::reset() { m_cartridge->reset_positions(); }
void Enigma::turn(int turns) { m_cartridge->turn(turns); }
void Enigma::turn() { m_cartridge->turn(); }
int  Enigma::encrypt(int m) {
    // cout<<"encr "<<m<<"\n";
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
        cout << "\n";
    }
    m_cartridge->turn();
    return c;
}
int Enigma::encrypt_without_turning(int m) const {
    return m_cartridge->encrypt_without_turning(m);
}
int *Enigma::encrypt(const int *m, int n) {
    // encrypt an array of ints
    // return value must be freed!
    if (m_verbose) {
        cout << "\n              ";
        for (int i= 0; i < m_wires; i++) {
            cout << (char)(i + (int)'A') << " ";
        }
        cout << "    R.POS.  R.SET.";
        cout << "\n              ";
        for (int i= 0; i < m_wires; i++) { cout << "| "; }
        cout << "     |||     |||  ";
        cout << "\n";
    }
    int *e= new int[n];   // must be freed outside
    for (int i= 0; i < n; i++) {
        if (m_verbose) { printf("m[%3d] = ", i); }
        e[i]= encrypt(m[i]);
    }
    return e;
}
/*int *Enigma::encrypt_inplace(const int *m, int n) {
    // encrypt an array of ints in place
    for (int i= 0; i < n; i++) {
        if (m_verbose) { printf("m[%3d] = ", i); }
        e[i]= encrypt(m[i]);
    }
    return e;
}*/
string Enigma::encrypt(string str) {
    // string to int*
    int *m= new int[str.length()];
    for (unsigned int i= 0; i < str.length(); ++i) {
        m[i]= (int)str[i] - (int)'A';
    }
    int *e= encrypt(m, str.length());   // handled here
    // int to string
    string out= "";
    for (unsigned int i= 0; i < str.length(); ++i) {
        out+= (char)(e[i] + (int)'A');
    }
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
