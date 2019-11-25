#ifndef BOMBE_H // include guard
#define BOMBE_H

using namespace std;
#include <iostream>
#include <vector>
#include <memory>
#include "enigma.h"

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
    void connect_enigma(unique_ptr<int[]> , int, int);
    void wipe();
    bool bundle_contradiction(int);
};

class Bombe {
    private:
        int                 m_letters=26;
        DiagonalBoard*      m_diagonal_board;
        Enigma*             m_enigma;
        vector < unique_ptr<int[]> > m_enigma_encryptions; //keep track of encryptions
        //could be vector vector but im trying to familiarize myelf with these
        //Enigma*        enigma;
    public:
        Bombe();
        vector<int> probable_search(string, string);
        void init_enigma_encryptions(int);
        void analyze(string, string);
        void setup_diagonal_board(string, string);
        bool check_wiring();
        bool bundle_contradiction(int);
};

#endif
