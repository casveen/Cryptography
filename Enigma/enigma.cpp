#include "enigma.h"
using namespace std;
static int cc=0;


//WHEEL
Rotor::Rotor() { }
Rotor::Rotor(int wires):_wires{wires} {
        _notches=1;
        _notch=new int[_notches]; _notch[0]=0; //corresponds to notch at A
        //allocate to wiring array
        _wiring_in= new int[_wires];
        _wiring_out=new int[_wires];
        //make a legal wiring, essentially a substitution cipher
        for(int j=0; j<_wires; j++) {
            _wiring_in[j]=j;
            _wiring_out[j]=j;
        }
    }
Rotor::Rotor(string in) {
    _notches=1;
    _notch=new int[_notches]; _notch[0]=0; //corresponds to notch at A
    _wires=in.length();
    //allocate to wiring array
    _wiring_in= new int[_wires];
    _wiring_out=new int[_wires];
    int wire;
    for(int i=0; i<_wires; i++)  {
        wire=(int) in[i]-(int) 'A';
        _wiring_in[i] =wire;
        _wiring_out[wire]=i;
    }
}
Rotor::Rotor(string in, string notch): Rotor(in) {
    _notches=notch.length();
    _notch=new int[_notches];
    for(int i=0; i<_notches; i++)  {
        _notch[i]=(int) notch[i]-(int) 'A';
    }
}
Rotor::~Rotor() {
    delete _wiring_in;
    delete _wiring_out;
    delete _notch;
    //cout<<"->rotor "<<_num<<" cleaned up\n";
}
Rotor::Rotor(Rotor const& copy) {
    // Copy constructor, very important, assures there is no shallow copy
    //src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    _wires     =copy._wires;
    _notches   =copy._notches;
    _wiring_in =new int[_wires];
    _wiring_out=new int[_wires];
    _notch     =new int[_notches];
    // Don't need to worry about copying integers.
    // But if the object has a copy constructor then
    // it would also need to worry about throws from the copy constructor.
    std::copy(&copy._wiring_in[0], &copy._wiring_in[_wires], _wiring_in);
    std::copy(&copy._wiring_out[0],&copy._wiring_out[_wires],_wiring_out);
    std::copy(&copy._notch[0],     &copy._notch[_notches],    _notch);
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
    swap(this->_wiring_in, s._wiring_in);
    swap(this->_wiring_out,s._wiring_out);
    swap(this->_notches,   s._notches);
    swap(this->_wires ,    s._wires);
    swap(this->_notch ,    s._notch);
}
int  Rotor::get_wires()           { return _wires; }
int* Rotor::get_wiring_in()       { return _wiring_in; }
int  Rotor::get_wiring_in(int i)  { return _wiring_in[i]; }
int* Rotor::get_wiring_out()      { return _wiring_out; }
int  Rotor::get_wiring_out(int i) { return _wiring_out[i]; }
int* Rotor::get_notch()           { return _notch; }
int  Rotor::get_notches()         { return _notches; }
void Rotor::randomize() {
    //cout<<"randomizing rotor\n";
    int p1, p2, t;
    //init in as 012345...
    for(int i=0; i<_wires; i++) { _wiring_in[i]=i; }
    //mix wires randomly
    for(int i=0; i<_wires*_wires; i++) {
        p1=rand()%_wires;
        p2=rand()%_wires;
        if (p1!=p2) {
            t=_wiring_in[p2];
            _wiring_in[p2]=_wiring_in[p1];
            _wiring_in[p1]=t;
        }
        else {
            i--;
        }
    }
    //now that in is randomized, make out the inverse of in
    _wiring_out=make_inverse(_wiring_in, _wires);

int* Rotor::make_inverse(int* in, int n) {
    //assumes list contains all integers from 0 to n-1
    int* out=(int*) malloc(n*sizeof*out);
    for(int i=0; i<n; i++) {
        out[in[i]]=i;
    }
    return out;
}
void Rotor::print() {
    for (int wire=0; wire<_wires; wire++) {
        printf("%2d: %2d\n", wire, _wiring_in[wire]);
        }
    return;
}
bool Rotor::is_valid() {
    //is out the inverse? is the mapping surjective?(covered by w spanning all wires)
    for (int w=0; w<_wires; w++) {
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
            _wiring_in[j]=j+1-2*(j%2);
        }
    }
void Reflector::randomize() {
    int w1, w2, v1, v2;
    for(int k=0; k<_wires*_wires; k++) {
        w1=rand()%_wires;
        w2=rand()%_wires;
        //cross the wires if possible, otherwise try again
        if (w1!=w2 && _wiring_in[w1]!=w2) {
            v2       =_wiring_in[w2];
            v1       =_wiring_in[w1];
            _wiring_in[w2]=v1; _wiring_in[v2]=w1;
            _wiring_in[w1]=v2; _wiring_in[v1]=w2;
        }
        else {
            k-=1;
        }
    }
}
bool Reflector::is_valid() {
    //is out the inverse? is the mapping surjective?(covered by w spanning all wires)
    //same as is valid for WHeel, but uses wiring_in both times
    for (int w=0; w<_wires; w++) {
        if (get_wiring_in(get_wiring_in(w))!=w) {
            return false;
        }
    }
    return true;

}



//CARTRIDGE
Cartridge::Cartridge() {} //XXX should really not be neccesary...
Cartridge::Cartridge(int rotor_count, int wires): _rotor_count{rotor_count}, _wires{wires} {
    _positions=new int[rotor_count];
    //init positions
    reset_positions();
    //make random rotors
    _rotors   =new Rotor*[rotor_count];
    for(int w=0; w<rotor_count; w++) {
        //cout<<"CONSTR CARTRIDGE: making a rotor\n";
        _rotors[w]=new Rotor(wires);
        //cout<<"CONSTR CARTRIDGE: randomizing\n";
        _rotors[w]->randomize();
    }
    _reflector=new Reflector(wires);
    _reflector->randomize();
    //_reflector->print();
    //print();
}
Cartridge::Cartridge(Cartridge const& copy) {
    //src::https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
    _wires             =copy._wires;
    _rotor_count       =copy._rotor_count;
    _rotors            =new Rotor*[_rotor_count];
    _reflector         =copy._reflector;
    _positions         =new int[_rotor_count];
    _reflector_position=copy._reflector_position;
    // Don't need to worry about copying integers.
    // But if the object has a copy constructor then
    // it would also need to worry about throws from the copy constructor.
    _rotors=copy._rotors; //rotors know how to copy, so we good
    std::copy(&copy._positions[0], &copy._positions[_wires], _positions);
}
Cartridge& Cartridge::operator=(Cartridge rhs) { // Pass by value (thus generating a copy)
    rhs.swap(*this); // Now swap data with the copy.
                     // The rhs parameter will delete the array when it
                     // goes out of scope at the end of the function
    return *this;
}
void Cartridge::swap(Cartridge& s) noexcept {
    using std::swap;
    swap(this->_wires, s._wires);
    swap(this->_rotor_count,s._rotor_count);
    swap(this->_rotors ,s._rotors);
    swap(this->_reflector ,s._reflector);
    swap(this->_positions ,s._positions);
    swap(this->_reflector_position ,s._reflector_position);
}
Cartridge::~Cartridge() {
    //cout<<"-->cleaning cartridge\n";
    for(int w=0; w<_rotor_count; w++) {
        delete _rotors[w];
    }
    delete [] _rotors;
    delete _reflector;
    //cout<<"-->cartridge cleaned up\n";
}
void Cartridge::reset_positions() {
    for (int p=0; p<_rotor_count; p++) {
        _positions[p]=0;
    }
    _reflector_position=0;
}
Rotor** Cartridge::get_rotors() {
    return _rotors;
}
Reflector* Cartridge::get_reflector() {
    return _reflector;
}
void Cartridge::set_positions(int p) {
    reset_positions();
    turn(p);
}
void Cartridge::set_positions(int* p_in) {
    for (int p=0; p<_rotor_count; p++) {
        _positions[p]=p_in[p];
    }
}
void Cartridge::set_positions(string in) {
    //assumes string is all capital english letters
    for (int p=0; p<_rotor_count; p++) {
        _positions[p]=(int) in[p]-(int) 'A';
    }
}
int* Cartridge::get_positions() { return _positions; }
int  Cartridge::get_positions_as_int() {
    int turn=0;
    int mult=1;
    for (int p=0; p<_rotor_count; p++) {
        turn+=_positions[p]*mult;
        mult*=_wires;
    }
    return turn;
}
void Cartridge::turn(int t) {
    int carry=t, next;
    for (int p=0; p<_rotor_count && carry>0; p++) {
        next=_positions[p]+carry;
        _positions[p]=next%_wires;
        carry=(int) next/_wires;
    }
}
//overloaded, single turn
void Cartridge::turn() { turn(1); }
//pass integer through wires without turning
int  Cartridge::encrypt(int i) {
    //forward pass + reflector(last)
    //printf("%2d ", i);
    for (int rotor=0; rotor<_rotor_count; rotor++) {
        //printf("(%2d) ", (i+_positions[rotor])%_wires);
        i=(_rotors[rotor]->get_wiring_in((i+_positions[rotor])%_wires)+_wires-_positions[rotor])%_wires;
        //printf("-> %2d ", i);
    }
    //reflector
    i=(_reflector->get_wiring_in((i+_reflector_position)%_wires)+_wires-_reflector_position)%_wires;
    //backward pass
    for (int rotor=_rotor_count-1; rotor>=0; rotor--) {
        //printf("(%2d) ", (i+_positions[rotor])%_wires);
        i=(_rotors[rotor]->get_wiring_out((i+_positions[rotor])%_wires)+_wires-_positions[rotor])%_wires;
        //printf("-> %2d ", i);
    }
    //printf("\n");
    return i;
}
//print the cartridge
void   Cartridge::print() {
    printf("  ");
    for (int rotor=0; rotor<_rotor_count; rotor++) {
        printf("  W%d", rotor);
    }
    for (int wire=0; wire<_wires; wire++) {
        printf("\n%2d: ", wire);
        for (int rotor=0; rotor<_rotor_count; rotor++) {
            printf("%2d  ", _rotors[rotor]->get_wiring_in(wire));
        }
    }
    printf("\n");
    return;
};
 //print positions of rotors in cartridge
void   Cartridge::print_positions() {
    for (int p=0; p<_rotor_count; p++) {
        printf("%2d ", _positions[p]);
    }
    printf("\n");}
void   Cartridge::randomize() {
    for (int w=0; w<_rotor_count; w++) {
        _rotors[w]->randomize();
    }
}



//ENIGMA ENGINE
Enigma::Enigma(int rotors_number, int wires): _rotors_number{rotors_number}, _wires{wires} {
    _cartridge=new Cartridge(rotors_number, wires);
}
Enigma::~Enigma() {
    //cout<<"--->cleaning enigma\n";
    delete _cartridge;
    //cout<<"--->enigma cleaned up\n";
}
void Enigma::randomize() {
    _cartridge->randomize();
}
void Enigma::set_coder() {
    //set code for language
}
void Enigma::reset() {
    _cartridge->reset_positions();
}
int  Enigma::encrypt(int m) {
    int c=_cartridge->encrypt(m);
    _cartridge->turn();
    return c;
}
int* Enigma::encrypt(int* m, int n) {
    int* e=(int*) malloc(n*sizeof(e));
    for (int i=0; i<n; i++) {
        e[i]=encrypt(m[i]);
    }
    return e;
}
void Enigma::print_positions() {
    _cartridge->print_positions();
}
void Enigma::print() {
    _cartridge->print();
}



/*
int main() {
    srand(time(NULL));
    //Rotor rotor=Rotor::make_random_rotor(20);
    //rotor.print();
    Cartridge cartridge=Cartridge::make_random_cartridge(3,8);
    //cartridge.print();
    //5test pos
    /*for (int t=0; t<100; t++) {
        cartridge.print_positions();
        cartridge.turn();
    }
    for (int t=0; t<8; t++) {
        printf("%2d -> %2d\n", t, cartridge.encrypt(t));
    }
    //cartridge.randomize();
    //cartridge.print();
    return 0;
}*/

//rotor good, cart good, turn good, encrypt in cartridge good
