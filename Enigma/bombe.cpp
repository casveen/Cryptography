//#include "enigma.h";
#include "bombe.h" //wire, diagonal board
#include "enigma.h"

void         Wire::flow() {
    m_live=-1;
    //activate connected dead wires
    for(int w=0; w<m_connections.size(); ++w) {
        Wire *wire=m_connections.at(w);
        if (wire->get_live()==0) {
            wire->flow();
        }
    }
    m_live=1;
}
int          Wire::get_live() {
    return m_live;
}
void         Wire::kill() {
    m_live=0;
}
void         Wire::set_live(int t_set) {
    m_live=t_set;
}
void         Wire::reset() {
    kill();
    m_connections.clear();
}
vector<Wire*> Wire::get_connections() {
    return m_connections;
}
void         Wire::connect(Wire* w) {
    m_connections.push_back(w);
}

DiagonalBoard::DiagonalBoard(int t_bundles) {
    //initialize, in triangle
    for(int b=0; b<t_bundles; b++) {
        //make wires for bundle
        vector<Wire*> wires;
        for(int w=0; w<=b; w++) {
            wires.push_back(new Wire());
        }
        //push onto bundle
        m_bundles.push_back(wires);
    }
    //fix symmetries
    for(int b=0; b<t_bundles; b++) {
        for(int w=b+1; w<t_bundles; w++) {
            m_bundles.at(b).push_back( m_bundles.at(w).at(b) );
        }
    }
}
void DiagonalBoard::activate(int t_bundle, int t_wire) {
    m_bundles.at(t_bundle).at(t_wire)->flow();
}
void DiagonalBoard::connect(int t_bundle_1, int t_wire_1, int t_bundle_2, int t_wire_2) {
    m_bundles.at(t_bundle_1).at(t_wire_1)->connect(m_bundles.at(t_bundle_2).at(t_wire_2));
    m_bundles.at(t_bundle_2).at(t_wire_2)->connect(m_bundles.at(t_bundle_1).at(t_wire_1));
}
void DiagonalBoard::connect_enigma(Enigma* t_enigma, int t_from, int t_to) {
    unique_ptr<int[]> encryption=t_enigma->get_encryption();
    for (int i=0; i<t_enigma->get_wires(); i++) {
        //cout<<i<<"/"<<t_enigma->get_wires()<<": "<<encryption[i]<<"\n";
        connect(t_from, i, t_to, encryption[i]);
    }
    //cout<<"finished connecting\n";
}
void DiagonalBoard::print() {
    int bundle_size=m_bundles.size();
    //cout<<"bundle size "<<bundle_size<<"\n";
    for(int b=0; b<bundle_size; b++) {
        //cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        for(int w=0; w<m_bundles.at(b).size(); w++) {
            Wire* wire=m_bundles.at(b).at(w);
            cout<<wire->get_live()<<" ";
        }
        cout<<"\n";
    }
    cout<<"sizes\n";
    for(int b=0; b<bundle_size; b++) {
        //cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        for(int w=0; w<m_bundles.at(b).size(); w++) {
            Wire* wire=m_bundles.at(b).at(w);
            cout<<wire->get_connections().size()<<" ";
        }
        cout<<"\n";
    }
}

Bombe::Bombe() {
    m_diagonal_board=new DiagonalBoard(26);
    m_enigma=        new Enigma(3, 26);
}
vector<int> Bombe::probable_search(string ciphertext, string crib) {
    vector<int> candidates;
    //find suitable pattern
    int ciphertext_length=ciphertext.length(), crib_length=crib.length();
    bool suitable;
    for(int i=0; i<ciphertext_length-crib_length; i++) {
        //compare, if same letter on same pos we have a contradiction
        suitable=true;
        for(int j=0; j<crib_length; j++) {
            if (ciphertext[i+j]==crib[j]) {
                suitable=false;
                break;
            }
        }
        if (suitable) {
            cout<<"suitable substring at "<<i<<"\n";
            cout<<ciphertext.substr(i, crib_length)<<"\n"<<crib<<"\n\n";
            candidates.push_back(i);
        }
    }
    return candidates;
}
void Bombe::setup_diagonal_board(string ciphertext, string crib) {
    cout<<"setting up DB\n";
    for(int j=0; j<crib.length(); j++) {
        m_diagonal_board->connect_enigma(m_enigma, (int)crib[j]-(int)'A', (int)ciphertext[j]-(int)'A');
        m_enigma->turn();
    }
    cout<<"set up DB\n";
}
void Bombe::analyze(string ciphertext, string crib) {
    cout<<"analyzing\n";
    //find candidates
    vector<int> candidates=probable_search(ciphertext, crib);
    //analyze each candidate
    for(int i=0; i<candidates.size(); i++) {
        //setup the wiring for the candidate
        cout<<"candidate "<<i<<"\n";
        setup_diagonal_board(ciphertext.substr(candidates[i], crib.length()), crib);
    }
    m_diagonal_board->print();
}






int main() {



    //board.connect(0,1,5,3); //connect Ab to Df
    //board.connect(0,1,2,3); //connect Ab to Df
    //board.connect_enigma(enigma, 0, 3); //A and D connected
    //board.activate(0,1); //b wire in A bundle
    //board.activate(2,4); //b wire in A bundle
    //board.activate(5,3); //f wire in D bundle
    Bombe* bombe=new Bombe();
    bombe->analyze("EEEEEEEEEEEEEEEEEEEDBGEAHDBDDDDDDDDDDDDD", "BEACHHEAD");
    delete bombe;
}
