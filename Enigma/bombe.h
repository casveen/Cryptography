#ifndef BOMBE_H // include guard
#define BOMBE_H

using namespace std;
#include "enigma.h"

//wiring
class Wire {
    private:
    vector<Wire*> m_connections;
    int m_live=0;   //0: dead, 1: live, -1: just activated
    public:
    void flow();    //make wire live, and also wires connected to it
    int get_live() const; //0 dead, -1 just set to live, 1 live
    void set_live(int);
    void kill(); //set to 0
    void reset();
    void connect(Wire* w);
    vector<Wire*>* get_connections();
};

class DiagonalBoard {
    private:
    vector<vector<Wire*>> m_bundles;
    public:
    DiagonalBoard(int);
    Wire* get_wire(int bundle, int wire) const;
    void activate(int, int);
    void connect(int, int, int, int);
    void print() const;
    void print_live() const;
    void connect_enigma(vector<pair<int,int>>* , int, int);
    void wipe();
    void reset();
    bool bundle_contradiction(int) const;
    int  bundle_sum(int) const;
};

class Bombe {
    private:
        int                 m_letters=26, m_rotor_count=3;
        DiagonalBoard*      m_diagonal_board;
        Enigma*             m_enigma;
        vector<vector<pair<int,int>>> m_enigma_encryptions; //keep track of encryptions
        //could be vector vector but im trying to familiarize myelf with these
        //Enigma*        enigma;
    public:
        Bombe(const std::initializer_list<Rotor> rotors, const Reflector reflector);
        vector<int> probable_search(const string, const string);
        int  find_most_wired_letter(const string, const string);
        void init_enigma_encryptions(int);
        void analyze(const string, const string);
        void setup_diagonal_board(const string, const string);
        bool check_wiring(int);
        bool bundle_contradiction(int);
        void print_encryptions() const;
};

#endif
