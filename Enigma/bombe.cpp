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
void DiagonalBoard::wipe() {
    for(int bundle=0; bundle<m_bundles.size(); ++bundle) {
        for(int wire=0; wire<=bundle; ++wire) {
            m_bundles.at(bundle).at(wire)->kill();
        }
    }
}
void DiagonalBoard::reset() {
    for(int bundle=0; bundle<m_bundles.size(); ++bundle) {
        for(int wire=0; wire<=bundle; ++wire) {
            m_bundles.at(bundle).at(wire)->reset();
        }
    }
}
void DiagonalBoard::connect(int t_bundle_1, int t_wire_1, int t_bundle_2, int t_wire_2) {
    m_bundles.at(t_bundle_1).at(t_wire_1)->connect(m_bundles.at(t_bundle_2).at(t_wire_2));
    m_bundles.at(t_bundle_2).at(t_wire_2)->connect(m_bundles.at(t_bundle_1).at(t_wire_1));
}

void DiagonalBoard::connect_enigma(vector<int> encryption, int t_from, int t_to) {
    for (int i=0; i<m_bundles.size(); i++) {
        //cout<<i<<"/"<<t_enigma->get_wires()<<": "<<encryption[i]<<"\n";
        connect(t_from, i, t_to, encryption.at(i));
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
/*int Diagonal::bundle_sum(int bundle) {
    int sum=0;
    for (int i=0; i<m_letters; ++i) {
        sum+=m_bundles.at(bundle).at(i)->get_live();
    }
    return sum;
}*/
bool DiagonalBoard::bundle_contradiction(int bundle) {
    int sum=0;
    for (int i=0; i<m_bundles.size(); ++i) {
        sum+=m_bundles.at(bundle).at(i)->get_live();
        if (sum>1) return false;
    }
    return true;
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
void Bombe::init_enigma_encryptions(int encryptions) {
    cout<<"intitialising encryptions\n";
    m_enigma_encryptions.clear();
    for(int i=0; i<encryptions; ++i) {
        cout<<i<<"\n";
        m_enigma_encryptions.push_back(m_enigma->get_encryption());
        m_enigma->turn();
    }
    cout<<"encryptions done\n";
}
void Bombe::setup_diagonal_board(string ciphertext, string crib) {
    //cout<<"setting up DB\n";
    for(int j=0; j<crib.length(); j++) {
        vector<int> encryption=m_enigma_encryptions.at(j);
        m_diagonal_board->connect_enigma(encryption, (int)crib[j]-(int)'A', (int)ciphertext[j]-(int)'A');
    }
}
void Bombe::analyze(string ciphertext, string crib) {
    //find total amount of rotor positions
    int total_permutations=1;
    for(int j=0; j<m_enigma->get_rotors(); j++) {
        total_permutations*=m_enigma->get_wires();
    }
    cout<<"analyzing\n";
    //find candidates
    vector<int> candidates=probable_search(ciphertext, crib);
    //analyze each candidate
    for(int i=0; i<candidates.size(); i++) {
        //setup the wiring for the candidate
        cout<<"candidate "<<i<<"\n";
        //setup crib.length() enigma encryptions
        init_enigma_encryptions(crib.length());
        //m_diagonal_board->print();
        //cout<<"HELL\n";
        for(int j=0; j<total_permutations; j++) {
            //cout<<"\n";
            printf("\r%5d/%5d", j, total_permutations);
            //reset board
            m_diagonal_board->reset();
            //setup connections in board
            setup_diagonal_board(ciphertext.substr(candidates[i], crib.length()), crib);
            if (check_wiring()) {
                cout<<"VALID CONFIGURATION FOUND\n";
            }
            //take out oldest, put in new encryption after enigma turns
            m_enigma->turn();
            m_enigma_encryptions.erase(m_enigma_encryptions.begin());; //erase first/oldest
            m_enigma_encryptions.push_back(m_enigma->get_encryption()); //add new
        }
        cout<<"UUU\n";
    }

}
bool Bombe::bundle_contradiction(int bundle) {
       return m_diagonal_board->bundle_contradiction(bundle);
}
bool Bombe::check_wiring() {
    //the diagonal board is properly wired, the rotors are in position
    //check if no contradictions

    //for each bundle, activate each wire, test each bundle for a contradiciton
    //(more than one live)
    for(int bundle=0; bundle<m_letters; ++bundle) {
        for(int wire=0; wire<m_letters; ++wire) {
            m_diagonal_board->activate(bundle, wire);
            if (bundle_contradiction(bundle)) return false;
            m_diagonal_board->wipe();
        }
    }
    return true;
}







int main() {
    Bombe* bombe=new Bombe();
    bombe->analyze("EEEEEEEEEEEEEEEEEEEDBGEAHDBDDDDDDDDDDDDD", "BEACHHEAD");
    delete bombe;
}
