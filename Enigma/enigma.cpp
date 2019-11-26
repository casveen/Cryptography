#include "enigma.h"
using namespace std;

//WHEEL
Rotor::Rotor() { }
Rotor::Rotor(int wires): m_wires{wires} {
        m_notches=1;
        m_notch=new int[m_notches]; m_notch[0]=0; //corresponds to notch at A
        //allocate to wiring array
        m_wiring_in= new int[m_wires];
        m_wiring_out=new int[m_wires];
        //make a legal wiring, essentially a substitution cipher
        for(int j=0; j<m_wires; j++) {
            m_wiring_in[j]=j;
            m_wiring_out[j]=j;
        }
    }
Rotor::Rotor(const string in) {
    m_notches=1;
    m_notch=new int[m_notches]; m_notch[0]=0; //corresponds to notch at A
    m_wires=in.length();
    //allocate to wiring array
    m_wiring_in= new int[m_wires];
    m_wiring_out=new int[m_wires];
    int wire;
    for(int i=0; i<m_wires; i++)  {
        wire=(int) in[i]-(int) 'A';
        m_wiring_in[i] =wire;
        m_wiring_out[wire]=i;
    }
}
Rotor::Rotor(const string in, const string notch): Rotor(in) {
    m_notches=notch.length();
    for(int i=0; i<m_notches; i++)  {
        m_notch[i]=(int) notch[i]-(int) 'A';
    }
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
    delete [] m_wiring_in;
    delete [] m_wiring_out;
    delete [] m_notch;
    //cout<<"->rotor "<<_num<<" cleaned up\n";
}
Rotor::Rotor(Rotor const& copy) {
    // Copy constructor, very important, assures there is no shallow copy
    //src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    m_wires     =copy.m_wires;
    m_notches   =copy.m_notches;
    m_wiring_in =new int[m_wires];
    m_wiring_out=new int[m_wires];
    m_notch     =new int[m_notches];
    // Don't need to worry about copying integers.
    // But if the object has a copy constructor then
    // it would also need to worry about throws from the copy constructor.
    std::copy(&copy.m_wiring_in[0], &copy.m_wiring_in[m_wires], m_wiring_in);
    std::copy(&copy.m_wiring_out[0],&copy.m_wiring_out[m_wires],m_wiring_out);
    std::copy(&copy.m_notch[0],     &copy.m_notch[m_notches],   m_notch);
}
Rotor& Rotor::operator=(Rotor rhs) {
    // Pass by value (thus generating a copy)
    rhs.swap(*this); // Now swap data with the copy.
                     // The rhs parameter will delete the array when it
                     // goes out of scope at the end of the function
    return *this;
}
void Rotor::swap(Rotor& s) noexcept {
    using std::swap;
    swap(this->m_wiring_in, s.m_wiring_in);
    swap(this->m_wiring_out,s.m_wiring_out);
    swap(this->m_notches,   s.m_notches);
    swap(this->m_wires ,    s.m_wires);
    swap(this->m_notch ,    s.m_notch);
}
//getters
int  Rotor::get_wires()            const { return m_wires; }
const int* Rotor::get_wiring_in()  const { return m_wiring_in; }
int  Rotor::get_wiring_in(int i)   const { return m_wiring_in[i]; }
const int* Rotor::get_wiring_out() const { return m_wiring_out; }
int  Rotor::get_wiring_out(int i)  const { return m_wiring_out[i]; }
const int* Rotor::get_notch()      const { return m_notch; }
int  Rotor::get_notch(int n)       const { return m_notch[n]; }
int  Rotor::get_notches()          const { return m_notches; }
//setters
void Rotor::set_wiring_in(int pos, int set) {
    m_wiring_in[pos]=set;
}
void Rotor::set_wiring_out(int pos, int set) {
    m_wiring_out[pos]=set;
}
void Rotor::set_verbose(bool set) { m_verbose=set; }
//other
int  Rotor::encrypt_in(int i, int offset) const {
    int out=(m_wiring_in[(i+offset)%m_wires]+m_wires-offset)%m_wires;
    if (m_verbose==true) {
        for (int j=0; j<m_wires; j++) {
            if (j==out) { cout<<"("; }
            cout<<(char) ((m_wiring_in[(j+offset)%m_wires]+m_wires-offset)%m_wires+(int)'A');
            if (j==out) { cout<<")"; }
        }
    }
    return out;
}
int  Rotor::encrypt_out(int i, int offset) const {
    int out=(m_wiring_out[(i+offset)%m_wires]+m_wires-offset)%m_wires;
    if (m_verbose) {
        for (int j=0; j<m_wires; j++) {
            if (j==out) { cout<<"("; }
            cout<<(char) ((m_wiring_out[(j+offset)%m_wires]+m_wires-offset)%m_wires+(int)'A');
            if (j==out) { cout<<")"; }
        }
    }
    return out;
}
void Rotor::randomize() {
    //cout<<"randomizing rotor\n";
    int p1, p2, t;
    //init in as 012345...
    for(int i=0; i<m_wires; i++) { m_wiring_in[i]=i; }
    //mix wires randomly
    for(int i=0; i<m_wires*m_wires; i++) {
        p1=rand()%m_wires;
        p2=rand()%m_wires;
        if (p1!=p2) {
            t=m_wiring_in[p2];
            m_wiring_in[p2]=m_wiring_in[p1];
            m_wiring_in[p1]=t;
        }
        else {
            i--;
        }
    }
    //now that in is randomized, make out the inverse of in
    make_inverse(m_wiring_in, m_wiring_out, m_wires);
}
void Rotor::make_inverse(const int* in, int* out, int n) const {
    //assumes list contains all integers from 0 to n-1
    for(int i=0; i<n; i++) {
        out[in[i]]=i;
    }
}
void Rotor::print() const {
    for (int wire=0; wire<m_wires; wire++) {
        printf("%2d ", m_wiring_in[wire]);
    }
    cout<<"\n";
}
bool Rotor::is_valid() const {
    //is out the inverse? is the mapping surjective?(covered by w spanning all wires)
    for (int w=0; w<m_wires; w++) {
        if (get_wiring_out(get_wiring_in(w))!=w) {
            return false;
        }
    }
    return true;
}
Reflector::Reflector(): Rotor() {
    }
Reflector::Reflector(int wires): Rotor(wires) {
        for(int j=0; j<wires; j++) {
            m_wiring_in[j]=j+1-2*(j%2);
        }
    }
Reflector::Reflector(string wiring): Rotor(wiring) { }
Reflector::Reflector(string wiring, string notches): Rotor(wiring, notches) { }
void Reflector::randomize() {
    int w1, w2, v1, v2;
    for(int k=0; k<m_wires*m_wires; k++) {
        w1=rand()%m_wires;
        w2=rand()%m_wires;
        //cross the wires if possible, otherwise try again
        if (w1!=w2 && m_wiring_in[w1]!=w2) {
            v2       =m_wiring_in[w2];
            v1       =m_wiring_in[w1];
            m_wiring_in[w2]=v1; m_wiring_in[v2]=w1;
            m_wiring_in[w1]=v2; m_wiring_in[v1]=w2;
        }
        else {
            k-=1;
        }
    }
}
bool Reflector::is_valid() const {
    //is out the inverse? is the mapping surjective?(covered by w spanning all wires)
    //same as is valid for WHeel, but uses wiring_in both times
    for (int w=0; w<m_wires; w++) {
        if (get_wiring_in(get_wiring_in(w))!=w) {
            return false;
        }
    }
    return true;

}





//CARTRIDGE
Cartridge::Cartridge() {} //XXX should really not be neccesary...
Cartridge::Cartridge(int rotor_count, int wires): m_rotor_count{rotor_count}, m_wires{wires} {
    m_positions=   new int[rotor_count];
    m_ring_setting=new int[rotor_count];
    //init positions
    reset_positions();
    reset_ring_setting();
    //make random rotors
    m_rotors   =new Rotor*[rotor_count];
    for(int w=0; w<rotor_count; w++) {
        m_rotors[w]=new Rotor(wires);
        m_rotors[w]->randomize();
    }
    //make a reflector
    m_reflector=new Reflector(wires);
    m_reflector->randomize();
}
Cartridge::Cartridge(const std::initializer_list<Rotor> rotors, Reflector reflector) {
    m_wires=((Rotor*)       rotors.begin())->get_wires();
    m_rotor_count= rotors.size();
    m_positions=   new int[m_rotor_count];
    m_ring_setting=new int[m_rotor_count];
    //init positions
    reset_positions();
    reset_ring_setting();
    //set to rotors
    m_rotors   =new Rotor*[m_rotor_count];
    int count=0;
    for (auto r : rotors) { m_rotors[count++]=new Rotor(r); }
    m_reflector=new Reflector(reflector);
}
Cartridge::Cartridge(Cartridge const& copy) {
    //src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    m_wires             =copy.m_wires;
    m_rotor_count       =copy.m_rotor_count;
    m_rotors            =new Rotor*[m_rotor_count];
    m_reflector         =copy.m_reflector;
    m_positions         =new int[m_rotor_count];
    m_ring_setting      =new int[m_rotor_count];
    m_reflector_position=copy.m_reflector_position;
    // Don't need to worry about copying integers.
    // But if the object has a copy constructor then
    // it would also need to worry about throws from the copy constructor.
    m_rotors=copy.m_rotors; //rotors know how to copy, so we good
    std::copy(&copy.m_positions[0],    &copy.m_positions[m_wires],    m_positions);
    std::copy(&copy.m_ring_setting[0], &copy.m_ring_setting[m_wires], m_ring_setting);
}
Cartridge& Cartridge::operator=(Cartridge rhs) { // Pass by value (thus generating a copy)
    rhs.swap(*this); // Now swap data with the copy.
                     // The rhs parameter will delete the array when it
                     // goes out of scope at the end of the function
    return *this;
}
void Cartridge::swap(Cartridge& s) noexcept {
    using std::swap;
    swap(this->m_wires, s.m_wires);
    swap(this->m_rotor_count,s.m_rotor_count);
    swap(this->m_rotors ,s.m_rotors);
    swap(this->m_reflector ,s.m_reflector);
    swap(this->m_positions ,s.m_positions);
    swap(this->m_ring_setting ,s.m_ring_setting);
    swap(this->m_reflector_position ,s.m_reflector_position);
}
Cartridge::~Cartridge() {
    for(int w=0; w<m_rotor_count; w++) {
        delete m_rotors[w];
    }
    delete [] m_rotors;
    delete  m_reflector;
    delete [] m_positions;
    delete [] m_ring_setting;
}
//getters
const Rotor**    Cartridge::get_rotors() const {
    return (const Rotor**) m_rotors;
}
const Reflector* Cartridge::get_reflector() const {
    return m_reflector;
}
const int*       Cartridge::get_ring_setting() const {
    return m_ring_setting;
}
const string     Cartridge::get_ring_setting_as_string() const {
    string out="";
    for (int i=0; i<m_rotor_count; i++) {
        out+=(char) (m_ring_setting[i]+(int) 'A');
    }
    return out;
}
const int*       Cartridge::get_positions() const { return m_positions; }
int              Cartridge::get_positions_as_int() const {
    int turn=0;
    int mult=1;
    for (int p=0; p<m_rotor_count; p++) {
        turn+=m_positions[p]*mult;
        mult*=m_wires;
    }
    return turn;
}
const string     Cartridge::get_positions_as_string() const {
    string out;
    out="";
    for(int w=0; w<m_rotor_count; w++) {
        out+=(char)(m_positions[w]+(int) 'A');
    }
    return out;
}
int              Cartridge::get_reflector_position() const { return m_reflector_position; }
//setters
void Cartridge::set_rotor(int pos, const Rotor* set) {
    m_rotors[pos]=(Rotor*) set; //overwrites. rotor should handle it properly
}
void Cartridge::set_reflector(const Reflector* set) {
    m_reflector=(Reflector*) set; //overwrites. rotor should handle it properly
}
void Cartridge::set_positions(const int* p_in) {
    for (int p=0; p<m_rotor_count; p++) {
        m_positions[p]=p_in[p];
    }
}
void Cartridge::set_positions(const string in) {
    //assumes string is all capital english letters
    for (int p=0; p<m_rotor_count; p++) {
        m_positions[p]=(int) in[p]-(int) 'A';
    }
}
void Cartridge::set_ring_setting(const int* p) {
    m_ring_setting=(int*) p;
}
void Cartridge::set_ring_setting(const string in) {
    for (int i=0; i<m_rotor_count; i++) {
        m_ring_setting[i]=(int) (in[i])-(int) ('A');
    }
}
void Cartridge::set_verbose(bool set)  {
    //set verbose of self, and all my parts
    for(int w=0; w<m_rotor_count; w++) {
        m_rotors[w]->set_verbose(set);
    }
    m_reflector->set_verbose(set);
    m_verbose=set;
}
//other
void Cartridge::reset_positions() {
    for (int p=0; p<m_rotor_count; p++) {
        m_positions[p]=0;
    }
    m_reflector_position=0;
}
void Cartridge::reset_ring_setting() {
    for (int p=0; p<m_rotor_count; p++) {
        m_ring_setting[p]=0;
    }
}
void Cartridge::turn() {
    //single turn
    int carry=1, next;
    for (int p=0; p<m_rotor_count && carry>0; p++) {
        next=m_positions[p]+carry;
        //check m_notch
        carry=0;
        for (int n=0; n<m_rotors[p]->get_notches(); n++) {
            if (m_rotors[p]->get_notch(n)==m_positions[p]) { //carry only if on notch and moving
                carry=1;
            }
        }
        m_positions[p]=next%m_wires;
        //carry=(int) next/m_wires; //only carry if exceeded a notch - ring setting[p]
    }
}
int  Cartridge::encrypt_without_turning(int i) const {
    if (m_verbose) {
        cout<<(char) (i +(int) 'A')<<" ------> ";
        //printf("%s ------> ", );
        for (int j=0; j<m_wires; j++) {
            cout<<(char) (j+(int) 'A');
        }
        cout<<"\n";
        //XXX plugboard!!!
    }
    //forward pass
    for (int rotor=0; rotor<m_rotor_count; rotor++) {
        if (m_verbose) { cout<<"  W"<<rotor<<"("<<(char)(i+(int) 'A')<<")-> "; }
        i=m_rotors[rotor]->encrypt_in(i, m_positions[rotor]);
        if (m_verbose) { cout<<" "<<(char)(m_positions[rotor]+(int)'A')<<"\n"; }
        //
    }
    //reflector
    //i=(m_reflector->get_wiring_in((i+m_reflector_position)%m_wires)+m_wires-m_reflector_position)%m_wires;
    if (m_verbose) { cout<<"  R"<<"("<<(char)(i+(int) 'A')<<")--> "; }
    i=m_reflector->encrypt_in(i, 0);
    if (m_verbose) { cout<<" "<<(char)(m_reflector_position+(int)'A')<<"\n"; }
    //backward pass
    for (int rotor=m_rotor_count-1; rotor>=0; rotor--) {
        if (m_verbose) { cout<<"  W"<<rotor<<"("<<(char)(i+(int) 'A')<<")-> "; }
        i=m_rotors[rotor]->encrypt_out(i, m_positions[rotor]);
        if (m_verbose) { cout<<" "<<(char)(m_positions[rotor]+(int)'A')<<"\n"; }
        //i=(m_rotors[rotor]->get_wiring_out((i+m_positions[rotor])%m_wires)+m_wires-m_positions[rotor])%m_wires;
    }
    return i;
}
void Cartridge::print() const{
    printf("  ");
    for (int rotor=0; rotor<m_rotor_count; rotor++) {
        printf("  W%d", rotor);
    }
    for (int wire=0; wire<m_wires; wire++) {
        printf("\n%2d: ", wire);
        for (int rotor=0; rotor<m_rotor_count; rotor++) {
            printf("%2d  ", m_rotors[rotor]->get_wiring_in(wire));
        }
    }
    printf("\n");
    return;
};
void Cartridge::print_positions() const{
    for (int p=0; p<m_rotor_count; p++) {
        printf("%2d ", m_positions[p]);
    }
    printf("\n");}
void Cartridge::randomize() {
    for (int w=0; w<m_rotor_count; w++) {
        m_rotors[w]->randomize();
    }
}



//ENIGMA ENGINE
Enigma::Enigma(int rotors_number, int wires): m_rotors_number{rotors_number}, m_wires{wires} {
    m_cartridge=new Cartridge(rotors_number, wires);
}
Enigma::Enigma(const std::initializer_list<Rotor> rotors, const Reflector reflector) {
    m_cartridge=    new Cartridge(rotors, reflector);
    m_wires=        ((Rotor*) rotors.begin())->get_wires();
    m_rotors_number=rotors.size();
}
Enigma::~Enigma() {
    delete m_cartridge;
}
//getter
int Enigma::get_wires() const {
    return m_wires;
}
int Enigma::get_rotors() const {
    return m_rotors_number;
}
const int* Enigma::get_rotor_position() const {
    return m_cartridge->get_positions();
}
string Enigma::get_rotor_position_as_string() const {
    return m_cartridge->get_positions_as_string();
}
const int*   Enigma::get_ring_setting() const {
    return m_cartridge->get_ring_setting();
}
string Enigma::get_ring_setting_as_string() const {
    return m_cartridge->get_ring_setting_as_string();
}
vector<int> Enigma::get_encryption() const {
    //return encryption at current step
    //encrypt all letters
    //this one is hardcoded to circumvent verbose branches, and avoid
    //double work due to symmetry
    //init
    vector<int> input(m_wires, -1);
    //get stuff from the cartridge
    const Rotor**    rotors   =m_cartridge->get_rotors();
    const Reflector* reflector=m_cartridge->get_reflector();
    const int*       positions=m_cartridge->get_positions();
    //for each letter, encrypt, unless already encrypted
    int m;
    for(int i=0; i<m_wires; i++) {
        if (input.at(i)==-1) {
            //encrypt i
            m=i;
            //forward
            for (int j=0; j<m_rotors_number; j++) {
                m=rotors[j]->encrypt_in(m, positions[j]);
            }
            //reflect
            m=reflector->encrypt_in(m, m_cartridge->get_reflector_position());
            //backward
            for (int j=m_rotors_number-1; j>=0; j--) {
                m=rotors[j]->encrypt_out(m, positions[j]);
            }
            //set
            input[i]=m;
            input[m]=i;
        }
    }
    return input;
}
//setter
void Enigma::set_coder() {
    //set code for language
}
void Enigma::set_verbose(bool set) {
    m_cartridge->set_verbose(set);
    m_verbose=false;
}
void Enigma::set_rotor_position(const int* in) {
    m_cartridge->set_positions(in);
}
void Enigma::set_rotor_position(const string in) {
    m_cartridge->set_positions(in);
}
void Enigma::set_ring_setting(const string in) {
    m_cartridge->set_ring_setting(in);
}
void Enigma::set_ring_setting(const int* in) {
    m_cartridge->set_ring_setting(in);
}
//other
void Enigma::randomize() {
    m_cartridge->randomize();
}
void Enigma::reset() {
    m_cartridge->reset_positions();
}
void Enigma::turn() {
    m_cartridge->turn();
}
int  Enigma::encrypt(int m) {
    if (m_verbose) {
        //set parts to non-verbose
        m_cartridge->set_verbose(false);
    }
    int c=encrypt_without_turning(m);
    if (m_verbose) {
        cout<<(char) (m+(int)'A')<<" ->";
        if (m==0) { cout<<"["; }
        else      { cout<<" "; }
        //encrypt all possible letters and print
        for (int i=0; i<m_wires; i++) {
            cout<<(char) (m_cartridge->encrypt_without_turning(i)+(int)'A');
            if (i==m || m==m_wires) { cout<<"]"; }
            else if (i==m-1)        { cout<<"["; }
            else                    { cout<<" "; }
        }
        //print posiions
        cout<<" --- "<<get_rotor_position_as_string();
        cout<<" --- "<<get_ring_setting_as_string();
        cout<<"\n";
    }
    m_cartridge->turn();
    return c;
}
int  Enigma::encrypt_without_turning(int m) const {
    return m_cartridge->encrypt_without_turning(m);
}
int* Enigma::encrypt(const int* m, int n) {
    //int digits=ceil(log(n));
    if (m_verbose) {
        cout<<"\n              ";
        for (int i=0; i<m_wires; i++) {
            cout<<(char)(i+(int)'A')<<" ";
        }
        cout<<"    R.POS.  R.SET.";
        cout<<"\n              ";
        for (int i=0; i<m_wires; i++) {
            cout<<"| ";
        }
        cout<<"     |||     |||  ";
        cout<<"\n";
    }

    int* e=new int[n];
    for (int i=0; i<n; i++) {
        if (m_verbose) {
            printf("m[%3d] = ", i);
        }
        e[i]=encrypt(m[i]);
    }
    return e;
}
void Enigma::print_positions() const {
    m_cartridge->print_positions();
}
void Enigma::print() const {
    m_cartridge->print();
}
