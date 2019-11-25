#ifndef BOMBE_H // include guard
#define BOMBE_H

using namespace std;
#include <iostream>
#include <vector>

//wiring
class Wire {
    private:
    vector<Wire*> m_connections;
    int m_live=0;   //0: dead, 1: live, -1: just activated

    public:
    void flow();    //make wire live, and also wires connected to it
    int get_live(); //0 dead, -1 just set to live, 1 live
    void set_live(int);
    void kill(); //set to 0
    void reset();
    void connect(Wire* w);
    vector<Wire*> get_connections();
};

class DiagonalBoard {
    private:
    vector<vector<Wire*>> m_bundles;
    public:
    DiagonalBoard(int);
    void activate(int, int);
    void connect(int, int, int, int);
    void print();






};

#endif
