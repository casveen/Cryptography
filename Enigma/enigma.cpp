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
Rotor::Rotor(string in) {
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
Rotor::Rotor(string in, string notch): Rotor(in) {
    m_notches=notch.length();
    for(int i=0; i<m_notches; i++)  {
        m_notch[i]=(int) notch[i]-(int) 'A';
    }
}
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
int  Rotor::get_wires()           { return m_wires; }
int* Rotor::get_wiring_in()       { return m_wiring_in; }
int  Rotor::get_wiring_in(int i)  { return m_wiring_in[i]; }
int* Rotor::get_wiring_out()      { return m_wiring_out; }
int  Rotor::get_wiring_out(int i) { return m_wiring_out[i]; }
int* Rotor::get_notch()           { return m_notch; }
int  Rotor::get_notch(int n)      { return m_notch[n]; }
int  Rotor::get_notches()         { return m_notches; }
int  Rotor::encrypt_in(int i, int offset) {
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
int  Rotor::encrypt_out(int i, int offset) {
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
void Rotor::set_verbose(int set)  { m_verbose=set; }
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
void Rotor::make_inverse(int* in, int* out, int n) {
    //assumes list contains all integers from 0 to n-1
    for(int i=0; i<n; i++) {
        out[in[i]]=i;
    }
}
void Rotor::print() {
    for (int wire=0; wire<m_wires; wire++) {
        printf("%2d: %2d\n", wire, m_wiring_in[wire]);
        }
    return;
}
bool Rotor::is_valid() {
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
bool Reflector::is_valid() {
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
Rotor** Cartridge::get_rotors() {
    return m_rotors;
}
Reflector* Cartridge::get_reflector() {
    return m_reflector;
}
/*void Cartridge::set_positions(int p) {
    reset_positions();
    turn(p);
}*/
void Cartridge::set_positions(int* p_in) {
    for (int p=0; p<m_rotor_count; p++) {
        m_positions[p]=p_in[p];
    }
}
void Cartridge::set_positions(string in) {
    //assumes string is all capital english letters
    for (int p=0; p<m_rotor_count; p++) {
        m_positions[p]=(int) in[p]-(int) 'A';
    }
}
int* Cartridge::get_positions() { return m_positions; }
int  Cartridge::get_positions_as_int() {
    int turn=0;
    int mult=1;
    for (int p=0; p<m_rotor_count; p++) {
        turn+=m_positions[p]*mult;
        mult*=m_wires;
    }
    return turn;
}
string Cartridge::get_positions_as_string() {
    string out;
    out="";
    for(int w=0; w<m_rotor_count; w++) {
        out+=(char)(m_positions[w]+(int) 'A');
    }
    return out;
}
void   Cartridge::set_ring_setting(int* p) {
    m_ring_setting=p;
}
void   Cartridge::set_ring_setting(string in) {
    for (int i=0; i<m_rotor_count; i++) {
        m_ring_setting[i]=(int) (in[i])-(int) ('A');
    }
}
int*   Cartridge::get_ring_setting() {
    return m_ring_setting;
}
string Cartridge::get_ring_setting_as_string() {
    string out="";
    for (int i=0; i<m_rotor_count; i++) {
        out+=(char) (m_ring_setting[i]+(int) 'A');
    }
    return out;
}
void Cartridge::set_verbose(int set)  {
    //set verbose of self, and all my parts
    for(int w=0; w<m_rotor_count; w++) {
        m_rotors[w]->set_verbose(set);
    }
    m_reflector->set_verbose(set);
    m_verbose=set;
}
/*void Cartridge::turn(int t) {
    int carry=t, next;
    for (int p=0; p<m_rotor_count && carry>0; p++) {
        next=m_positions[p]+carry;
        m_positions[p]=next%m_wires;


        carry=m_positions[p]
        //carry=(int) next/m_wires; //only carry if exceeded a notch - ring setting[p]
    }
}*/
//overloaded, single turn
void Cartridge::turn() {
    //{ turn(1); }
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
//pass integer through wires without turning
int  Cartridge::encrypt(int i) {
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
//print the cartridge
void   Cartridge::print() {
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
 //print positions of rotors in cartridge
void   Cartridge::print_positions() {
    for (int p=0; p<m_rotor_count; p++) {
        printf("%2d ", m_positions[p]);
    }
    printf("\n");}
void   Cartridge::randomize() {
    for (int w=0; w<m_rotor_count; w++) {
        m_rotors[w]->randomize();
    }
}



//ENIGMA ENGINE
Enigma::Enigma(int rotors_number, int wires): m_rotors_number{rotors_number}, m_wires{wires} {
    m_cartridge=new Cartridge(rotors_number, wires);
}
Enigma::~Enigma() {
    //cout<<"--->cleaning enigma\n";
    delete m_cartridge;
    //cout<<"--->enigma cleaned up\n";
}
void Enigma::randomize() {
    m_cartridge->randomize();
}
void Enigma::set_coder() {
    //set code for language
}
void Enigma::set_verbose(int set) {
    m_cartridge->set_verbose(set);
    m_verbose=false;
}
void Enigma::set_rotor_position(int* in) {
    m_cartridge->set_positions(in);
}
void Enigma::set_rotor_position(string in) {
    m_cartridge->set_positions(in);
}
string Enigma::get_rotor_position_as_string() {
    return m_cartridge->get_positions_as_string();
}
void   Enigma::set_ring_setting(string in) {
    m_cartridge->set_ring_setting(in);
}
void   Enigma::set_ring_setting(int* in) {
    m_cartridge->set_ring_setting(in);
}
int*   Enigma::get_ring_setting() {
    return m_cartridge->get_ring_setting();
}
string Enigma::get_ring_setting_as_string() {
    return m_cartridge->get_ring_setting_as_string();
}
void Enigma::reset() {
    m_cartridge->reset_positions();
}
int  Enigma::encrypt(int m) {
    if (m_verbose) {
        //set parts to non-verbose
        m_cartridge->set_verbose(false);
    }
    int c=m_cartridge->encrypt(m);
    if (m_verbose) {
        cout<<(char) (m+(int)'A')<<" ->";
        if (m==0) { cout<<"["; }
        else      { cout<<" "; }
        //encrypt all possible letters and print
        for (int i=0; i<m_wires; i++) {
            cout<<(char) (m_cartridge->encrypt(i)+(int)'A');
            if (i==m || m==m_wires) { cout<<"]"; }
            else if (i==m-1)           { cout<<"["; }
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
int* Enigma::encrypt(int* m, int n) {
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
void Enigma::print_positions() {
    m_cartridge->print_positions();
}
void Enigma::print() {
    m_cartridge->print();
}
