#include "enigma.h"
using namespace std;

//WHEEL
Wheel::Wheel() { }
Wheel::Wheel(int wires) {
        _wires=wires;
        //allocate to wiring array
        _wiring=(int*) malloc(_wires*sizeof*_wiring);
        //make a legal wiring, essentially a substitution cipher
        for(int j=0; j<_wires; j++) {
            _wiring[j]=-1;
        }
    }
int  Wheel::get_wires()      { return _wires; }
int* Wheel::get_wiring()     { return _wiring; }
int  Wheel::get_wiring(int i) { return _wiring[i]; }
void Wheel::randomize() {
    int set=0, at=0;
    while (set<_wires) {
        at=rand()%_wires;
        if (_wiring[at]==-1) { _wiring[at]=set++; }
    }
}
void Wheel::print() {
    for (int wire=0; wire<_wires; wire++) {
        printf("%2d: %2d\n", wire, _wiring[wire]);
        }
    return;
}
Reflector::Reflector(): Wheel() {
    }
Reflector::Reflector(int wires): Wheel(wires) {
        for(int j=0; j<wires; j++) {
            _wiring[j]=j+1-2*(j%2);
        }
    }
void Reflector::randomize() {
    int w1, w2, v1, v2;
    for(int k=0; k<_wires*_wires; k++) {
        w1=rand()%_wires;
        w2=rand()%_wires;
        //cross the wires if possible, otherwise try again
        if (w1!=w2 && _wiring[w1]!=w2) {
            v2       =_wiring[w2];
            v1       =_wiring[w1];
            _wiring[w2]=v1; _wiring[v2]=w1;
            _wiring[w1]=v2; _wiring[v1]=w2;
        }
        else {
            k-=1;
        }
    }
}



//CARTRIDGE
Cartridge::Cartridge() {} //XXX should really not be neccesary...
Cartridge::Cartridge(int wheel_count, int wires) {
    _wheel_count=wheel_count;
    _wires=wires;
    _positions=(int*) malloc(_wheel_count*sizeof*_positions);
    //init positions
    reset_positions();
    //make wheels
    _wheels   =make_random_wheels(wheel_count, wires);
    _reflector=new Reflector();
    *_reflector=Reflector::make_random_reflector(wires);
}
void Cartridge::reset_positions() {
    for (int p=0; p<_wheel_count; p++) {
        _positions[p]=0;
    }
    _reflector_position=0;
}
void Cartridge::set_positions(int p) {
    reset_positions();
    turn(p);
}
void Cartridge::set_positions(int* p_in) {
    for (int p=0; p<_wheel_count; p++) {
        _positions[p]=p_in[p];
    }
}
int* Cartridge::get_positions() { return _positions; }
int  Cartridge::get_positions_as_int() {
    int turn=0;
    int mult=1;
    for (int p=0; p<_wheel_count; p++) {
        turn+=_positions[p]*mult;
        mult*=_wires;
    }
    return turn;
}
void Cartridge::turn(int t) {
    int carry=t, next;
    for (int p=0; p<_wheel_count && carry>0; p++) {
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
    for (int wheel=0; wheel<_wheel_count; wheel++) {
        //printf("(%2d) ", (i+_positions[wheel])%_wires);
        i=(_wheels[wheel].get_wiring((i+_positions[wheel])%_wires)+_wires-_positions[wheel])%_wires;
        //printf("-> %2d ", i);
    }
    //reflector
    i=(_reflector->get_wiring((i+_reflector_position)%_wires)+_wires-_reflector_position)%_wires;
    //backward pass
    for (int wheel=_wheel_count-1; wheel>=0; wheel--) {
        //printf("(%2d) ", (i+_positions[wheel])%_wires);
        i=(_wheels[wheel].get_wiring((i+_positions[wheel])%_wires)+_wires-_positions[wheel])%_wires;
        //printf("-> %2d ", i);
    }
    //printf("\n");
    return i;
}
//print the cartridge
void   Cartridge::print() {
    printf("  ");
    for (int wheel=0; wheel<_wheel_count; wheel++) {
        printf("  W%d", wheel);
    }
    for (int wire=0; wire<_wires; wire++) {
        printf("\n%2d: ", wire);
        for (int wheel=0; wheel<_wheel_count; wheel++) {
            printf("%2d  ", _wheels[wheel].get_wiring(wire));
        }
    }
    printf("\n");
    return;
};
 //print positions of wheels in cartridge
void   Cartridge::print_positions() {
    for (int p=0; p<_wheel_count; p++) {
        printf("%2d ", _positions[p]);
    }
    printf("\n");}
void   Cartridge::randomize() {
    for (int w=0; w<_wheel_count; w++) {
        _wheels[w].randomize();
    }
}
Wheel* Cartridge::make_random_wheels(int n, int wires) {
    Wheel* wheels=(Wheel*) malloc(n*sizeof(Wheel));
    for(int w=0; w<n; w++) {
        wheels[w]=Wheel::make_random_wheel(wires);
    }
    return wheels;
}



//ENIGMA ENGINE
Enigma::Enigma(int wheels_number, int wires) {
    _cartridge=new Cartridge();
    *_cartridge=Cartridge::make_random_cartridge(wheels_number, wires);
    _wires=wires;
    _wheels_number=wheels_number;
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
    //Wheel wheel=Wheel::make_random_wheel(20);
    //wheel.print();
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

//wheel good, cart good, turn good, encrypt in cartridge good
