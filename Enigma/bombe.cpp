#include "enigma.h";
#include "bombe.h"; //wire

void         Wire::flow() {
    m_live=-1;
    //activate connected dead wires
    for (auto w=m_connectios.begin(); w!=m_connections.end(); ++w) {
        Wire wire=m_connections.at(w);
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
void         Wire::reset() {
    kill();
    m_connections.clear();
}
vector<wire> Wire::get_connections() {
    return m_connections;
}
void         Wire::connect(Wire w) {
    m_connections.push_back(w);
    w.get_connections().push_back(this) //hmmmmmm
}




void bombe(string ciphertext, string crib) {
    //find suitable pattern
    int ciphertext_length=ciphertext.length();, crib_lenght=crib.length();
    bool sutiable;
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
            cout<<"suitable substring at "<<i;
        }
        //XXX run bombe on suitable stirng
    }
}

//setup wiring XXX Wire*?
vector<vector<Wire>> bombe_init_wires(int wires) {
    //declare
    vector<vector<Wire>> bundles=new vector<vector<Wire>>;
    //initialize, in triangle
    for(int b=0; b<wires; b++) {
        //make wires for bundle
        vector<Wire> wires=new vector<Wire>;
        for(int w=0; w<=b; w++) {
            wires.push_back(new Wire());
        }
        //push onto bundle
        bundles.push_back(wires);
    }
    //fix symmetries
    for(int b=0; b<wires; b++) {
        //make wires for bundle
        vector<Wire> bundle=bundles.at(b);
        for(int w=b+1; w<wires; w++) {
            wires.push_back( bundles.at(w).at(b) );
        }
        //push onto bundle
        bundles.push_back(wires);
    }




}
