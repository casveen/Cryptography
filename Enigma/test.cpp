#include "enigma.h"


//make a large random wheel, test the wirings
const int wires =1000;
const int wheels=10;
bool test_wiring() {
    Wheel wheel=Wheel::make_random_wheel(wires);
    //test if wires cross correctly
    for (int w=0; w<wires; w++) {
        if wheel.get_wiring(wheel.get_wiring(w))!=w or wheel.get_wiring(w)==w
            return false;
    }
    return true;
}


int main() {
    cout<< test_wiring();
}
