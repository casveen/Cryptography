#include "enigma.h"
using namespace std;

/*
An enigma contains a cartridge, which contains the wheels.
enigma passes data through cartridge, cartridge passes through wheels and returns, and
rotates the wheels. Cartridge contains positions, wheels do not know orientation
*/

//a wheel has wiring, ints to ints.
class Wheel {
    private:
        int* _wiring; //index i goes to value at index i
        int  _wires;

    public:
    //CONSTRUCTOR
    Wheel(int wires) {
        _wires=wires;
        //allocate to wiring array
        _wiring=(int*) malloc(_wires*sizeof*_wiring);
        //make a legal wiring, note that wires[wires[j]]=j
        for(int j=0; j<_wires; j++) {
            _wiring[j]=j+1-2*(j%2);
        }
    }

    //GETTERS
    int  get_wires()      { return _wires; }
    int* get_wiring()     { return _wiring; }
    int get_wiring(int i) { return _wiring[i]; }

    //FUNCTIONS
    void randomize() {
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

    void print() {
        for (int wire=0; wire<_wires; wire++) {
            printf("%2d: %2d\n", wire, _wiring[wire]);
        }
        return;
    }

    //STATICS
    static Wheel make_random_wheel(int wires) {
        Wheel wheel=Wheel(wires);
        wheel.randomize();
        return wheel;
    }
};



class Cartridge {
    private:
    Wheel* _wheels;
    int    _wheel_count, _wires; //wires?
    int*   _positions;

    public:
    //CONSTRUCTOR, random wheels
    Cartridge(int wheel_count, int wires) {
        _wheel_count=wheel_count;
        _wires=wires;
        _positions=(int*) malloc(_wheel_count*sizeof*_positions);
        //init positions
        reset_positions();
        //make wheels
        _wheels=make_random_wheels(wheel_count, wires);
    }

    void reset_positions() {
        for (int p=0; p<_wheel_count; p++) {
            _positions[p]=0;
        }
    }

    void turn(int t) {
        int carry=t, next;
        for (int p=0; p<_wheel_count && carry>0; p++) {
            next=_positions[p]+carry;
            _positions[p]=next%_wires;
            carry=(int) next/_wires;
        }
    }

    //overloaded, single turn
    void turn() { turn(1); }

    //pass integer through wires without turning
    int encrypt(int i) {
        //forward pass + reflector(last)
        for (int wheel=0; wheel<_wheel_count; wheel++) {
            i=_wheels[wheel].get_wiring((i+_positions[wheel])%_wheel_count);
        }
        //backward pass
        for (int wheel=_wheel_count-2; wheel>=0; wheel--) {
            i=_wheels[wheel].get_wiring((i+_positions[wheel])%_wheel_count);
        }
        return i;
    }

    //PRINT
    void print() {
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

    void print_positions() {
        for (int p=0; p<_wheel_count; p++) {
            printf("%2d ", _positions[p]);
        }
        printf("\n");
    }

    void randomize() { _wheels[0].randomize(); }

    //make array of n random wheels
    Wheel* make_random_wheels(int n, int wires) {
        Wheel* wheels=(Wheel*) malloc(n*sizeof(Wheel));
        for(int w=0; w<n; w++) {
            wheels[w]=Wheel::make_random_wheel(wires);
        }
        return wheels;
    }

    //STATICS
    static Cartridge make_random_cartridge(int wheels, int wires) {
        Cartridge cartridge=Cartridge(wheels, wires);
        return cartridge;
    }
};



class Enigma {
    private:
    Cartridge* _cartridge;
    int _wheels_number, _wires;

    public:
    //CONSTRUCTOR
    Enigma(int wheels_number, int wires) {
        *_cartridge=Cartridge::make_random_cartridge(wheels_number, wires);
        _wires=wires;
        _wheels_number=wheels_number;
    }

    void set_coder() {
        //set code for language
    }

    void reset() {
        _cartridge->reset_positions();
    }

    int encrypt(int m) {
        _cartridge->encrypt(m);
        _cartridge->turn();
    }

    int* encrypt(int* m, int n) {
        int* e=(int*) malloc(n*sizeof(e));
        for (int i=0; i<n; i++) {
            e[i]=encrypt(m[i]);
        }
        return e;
    }
};



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
    }*/
    for (int t=0; t<8; t++) {
        printf("%2d -> %2d\n", t, cartridge.encrypt(t));
    }
    //cartridge.randomize();
    //cartridge.print();
    return 0;
}

//wheel good, cart good, turn good, encrypt in cartridge good
