//#include "enigma.h";
#include "bombe.h" //wire, diagonal board

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
Wire* DiagonalBoard::get_wire(int t_bundle, int t_wire) const{
        return m_bundles.at(t_bundle).at(t_wire);
}
void DiagonalBoard::activate(int t_bundle, int t_wire) {
    get_wire(t_bundle, t_wire)->flow();
}
void DiagonalBoard::wipe() {
    for(int bundle=0; bundle<m_bundles.size(); ++bundle) {
        for(int wire=0; wire<=bundle; ++wire) {
            get_wire(bundle, wire)->kill();
        }
    }
}
void DiagonalBoard::reset() {
    for(int bundle=0; bundle<m_bundles.size(); ++bundle) {
        for(int wire=0; wire<=bundle; ++wire) {
            get_wire(bundle, wire)->reset();
        }
    }
}
void DiagonalBoard::connect(int t_bundle_1, int t_wire_1, int t_bundle_2, int t_wire_2) {
    get_wire(t_bundle_1, t_wire_1)->connect(get_wire(t_bundle_2, t_wire_2));
    get_wire(t_bundle_2, t_wire_2)->connect(get_wire(t_bundle_1, t_wire_1));
}
void DiagonalBoard::connect_enigma(vector<pair<int,int>> encryption, int t_from, int t_to) {
    //cout<<"diag board wiring in an enigma\n";
    for (int i=0; i<encryption.size(); i++) {
        //cout<<"connecting...";
        //cout<<t_from<<":"<<encryption[i].first<<" <-> "<<t_to<<":"<<encryption[i].second<<"\n";
        connect(t_from, encryption.at(i).first, t_to, encryption.at(i).second);
    }
    //cout<<"finished connecting\n";
}
int DiagonalBoard::bundle_sum(int bundle) const{
    int sum=0;
    for (int i=0; i<m_bundles.size(); ++i) {
        sum+=get_wire(bundle,i)->get_live();
    }
    return sum;
}
void DiagonalBoard::print() const{
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
void DiagonalBoard::print_live() const{
    int bundle_size=m_bundles.size();
    for(int b=0; b<bundle_size; b++) {
        cout<<" | ";
    }
    cout<<"\n";
    for(int b=0; b<bundle_size; b++) {
        cout<<" "<<(char)(b+(int)'A')<<" ";
    }
    cout<<"\n";
    //cout<<"bundle size "<<bundle_size<<"\n";
    for(int b=0; b<bundle_size; b++) {
        //cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        printf("%2d ", bundle_sum(b));
    }
    cout<<"\n";
    cout<<"\n";
}
/*int Diagonal::bundle_sum(int bundle) {
    int sum=0;
    for (int i=0; i<m_letters; ++i) {
        sum+=m_bundles.at(bundle).at(i)->get_live();
    }
    return sum;
}*/
bool DiagonalBoard::bundle_contradiction(int bundle) const{
    int sum=0;
    for (int i=0; i<m_bundles.size(); ++i) {
        sum+=get_wire(bundle,i)->get_live();
        if (sum>1) return false;
    }
    return true;
}


Bombe::Bombe(const std::initializer_list<Rotor> rotors, const Reflector reflector) {
    m_letters=         ((Rotor*) rotors.begin())->get_wires();
    m_rotor_count=   rotors.size();
    m_diagonal_board=new DiagonalBoard(m_letters);
    m_enigma=        new Enigma(m_rotor_count, m_letters);
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
    //cout<<"intitialising encryptions\n";
    m_enigma_encryptions.clear();
    for(int i=0; i<encryptions; ++i) {
        //cout<<i<<"\n";
        m_enigma_encryptions.push_back(m_enigma->get_encryption_onesided());
        m_enigma->turn();
    }
    //cout<<"encryptions done\n";
}
void Bombe::setup_diagonal_board(string ciphertext, string crib) {
    //cout<<"setting up DB\n";
    for(int j=0; j<crib.length(); j++) {
        //cout<<"getting an encryption\n";
        vector<pair<int,int>> encryption=m_enigma_encryptions.at(j);
        //cout<<"connecting an enigma\n";
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
    int dummy;
    //find candidates
    vector<int> candidates=probable_search(ciphertext, crib);
    //analyze each candidate
    for(int i=0; i<candidates.size(); i++) {
        //setup the wiring for the candidate
        cout<<"candidate "<<i<<"\n";
        //setup crib.length() enigma encryptions
        init_enigma_encryptions(crib.length());
        //print_encryptions();
        //m_diagonal_board->print();
        //cout<<"HELL\n";
        for(int j=0; j<total_permutations; j++) {
            //cout<<"\n";
            printf("%5d/%5d\n", j, total_permutations);
            //reset board
            m_diagonal_board->reset();
            //setup connections in board
            //cout<<"setup\n";
            setup_diagonal_board(ciphertext.substr(candidates[i], crib.length()), crib);
            //cout<<"checking\n";
            if (check_wiring()) {
                cout<<"VALID CONFIGURATION FOUND\n";
            }
            //cout<<"checked\n";
            //take out oldest, put in
            //new encryption after enigma turns
            //print_encryptions();
            cin.get();
            m_enigma->turn();
            m_enigma_encryptions.erase(m_enigma_encryptions.begin());; //erase first/oldest
            m_enigma_encryptions.push_back(m_enigma->get_encryption_onesided()); //add new
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
    int sum, wire;
    for(int bundle=0; bundle<m_letters; ++bundle) {
        //for(int wire=0; wire<m_letters; ++wire) {
            //if (wire!=bundle) {
                wire=0;
                cout<<"BUNDLE: "<<(char)(bundle+(int)'A');
                cout<<"  WIRE: "<<(char)(wire+(int)'A');
                cout<<"\n";
                m_diagonal_board->activate(bundle, wire);
                m_diagonal_board->print_live();
                //m_diagonal_board->print();
                sum=m_diagonal_board->bundle_sum(bundle);
                if (!(sum==1 || sum==m_letters-1)) return false;
                m_diagonal_board->wipe(); //wipe current in wires

        //}
    }
    return true;
}

void Bombe::print_encryptions() const {
    for(int wire=0; wire<m_letters/2; ++wire) {
        for(int e=0; e<m_enigma_encryptions.size(); ++e) {
            printf("<%2d,%2d>   ",(m_enigma_encryptions[e])[wire].first,(m_enigma_encryptions[e])[wire].second);
        }
        cout<<"\n";
    }
}






int main() {
    static Rotor I=                 Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
    static Rotor II=                Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
    static Rotor III=               Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
    static Reflector UKWR=      Reflector("QYHOGNECVPUZTFDJAXWMKISRBL"); //ref

    Bombe* bombe=new Bombe({I, II, III}, UKWR);

    bombe->analyze("QFZWRWIVTYRESXBFOGKUHQBAISEZ", "WETTERVORHERSAGEBISKAYA");
    delete bombe;
}
