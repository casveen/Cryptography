//#include "enigma.h";
#include "bombe.h" //wire, diagonal board

void           Wire::flow() {
    m_live=-1;
    //activate connected dead wires
    for(unsigned int w=0; w<m_connections.size(); ++w) {
        Wire *wire=m_connections.at(w);
        if (wire->get_live()==0) {
            wire->flow();
        }
    }
    m_live=1;
}
int            Wire::get_live() const {
    return m_live;
}
void           Wire::kill() {
    m_live=0;
}
void           Wire::set_live(int t_set) {
    m_live=t_set;
}
void           Wire::reset() {
    kill();
    m_connections.clear();
}
vector<Wire*>* Wire::get_connections() {
    return &m_connections;
}
void           Wire::connect(Wire* w) {
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
    for(unsigned int bundle=0; bundle<m_bundles.size(); ++bundle) {
        for(unsigned int wire=0; wire<=bundle; ++wire) {
            get_wire(bundle, wire)->kill();
        }
    }
}
void DiagonalBoard::reset() {
    for(unsigned int bundle=0; bundle<m_bundles.size(); ++bundle) {
        for(unsigned int wire=0; wire<=bundle; ++wire) {
            get_wire(bundle, wire)->reset();
        }
    }
}
void DiagonalBoard::connect(int t_bundle_1, int t_wire_1, int t_bundle_2, int t_wire_2) {
    get_wire(t_bundle_1, t_wire_1)->connect(get_wire(t_bundle_2, t_wire_2));
    get_wire(t_bundle_2, t_wire_2)->connect(get_wire(t_bundle_1, t_wire_1));
}
void DiagonalBoard::connect_enigma(vector<pair<int,int>>* encryption, int t_from, int t_to) {
    //cout<<"diag board wiring in an enigma\n";
    for (unsigned int i=0; i<encryption->size(); i++) {
        //cout<<"connecting...";
        //cout<<t_from<<":"<<encryption->at(i).first<<" <-> "<<t_to<<":"<<encryption->at(i).second<<"\n";
        connect(t_from, encryption->at(i).first, t_to, encryption->at(i).second);
        connect(t_from, encryption->at(i).second, t_to, encryption->at(i).first);
    }
    //cout<<"finished connecting\n";
}
int DiagonalBoard::bundle_sum(int bundle) const{
    int sum=0;
    for (unsigned int i=0; i<m_bundles.size(); ++i) {
        sum+=get_wire(bundle,i)->get_live();
    }
    return sum;
}
void DiagonalBoard::print() const{
    unsigned int bundle_size= m_bundles.size();
    //cout<<"bundle size "<<bundle_size<<"\n";
    for(unsigned int b=0; b<bundle_size; b++) {
        //cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        for(unsigned int w=0; w<m_bundles.at(b).size(); w++) {
            Wire* wire=m_bundles.at(b).at(w);
            cout<<wire->get_live()<<" ";
        }
        cout<<"\n";
    }
    cout<<"sizes\n";
    for(unsigned int b=0; b<bundle_size; b++) {
        //cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        for(unsigned int w=0; w<m_bundles.at(b).size(); w++) {
            Wire* wire=m_bundles.at(b).at(w);
            cout<<wire->get_connections()->size()<<" ";
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
    int ones=0, zeros=0;
    //we have a contradiction if there is not exactly 1 or exactly 25 live wires
    for (unsigned int i=0; i<m_bundles.size(); ++i) {
        get_wire(bundle,i)->get_live()==0?++zeros:++ones;
        // ones==1 && zeros=25 || ones=25 zeros=1
        //  1 impossible, 25 impossible
        if (ones>=2 && zeros>=2) return true;
    }
    //if only ones or only zeros, we have a contradiciton
    if (ones==0 || zeros==0) return true;
    //otherwise, the bundle wiring is valid
    return false;
}


Bombe::Bombe(const std::initializer_list<Rotor> rotors, const Reflector reflector) {
    m_letters=         ((Rotor*) rotors.begin())->get_wires();
    m_rotor_count=   rotors.size();
    m_diagonal_board=new DiagonalBoard(m_letters);
    m_enigma=        new Enigma(rotors, reflector);
}
vector<int> Bombe::probable_search(const string ciphertext, const string crib) {
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
            cout<<ciphertext.substr(i, crib_length)<<"\n"<<crib<<"\n";
            candidates.push_back(i);
        }
    }
    return candidates;
}
int  Bombe::find_most_wired_letter(const string ciphertext, const string crib) {
    //make histogram, find most frequent element
    int* histogram=new int[m_letters];
    for (int i=0; i<m_letters; ++i) { histogram[i]=0; }
    for (unsigned int i=0; i<min(ciphertext.length(), crib.length()); ++i) {
        histogram[(int) ciphertext[i]-(int) 'A']++;
        histogram[(int) crib[i]- (int) 'A']++;
    }
    int max_value=0, max_index=-1;
    for (int i=0; i<m_letters; ++i) {
        if (histogram[i]>max_value) {
            max_value=histogram[i];
            max_index=i;
        }
    }
    delete [] histogram;
    return max_index;
}
void Bombe::init_enigma_encryptions(int encryptions) {
    //cout<<"intitialising encryptions\n";
    m_enigma_encryptions.clear();
    for(int i=0; i<encryptions; ++i) {
        //cout<<m_enigma->get_encryption_as_string()<<"\n";
        //cout<<i<<"\n";
        m_enigma_encryptions.push_back(m_enigma->get_encryption_onesided());
        m_enigma->turn();
    }
    //cout<<"encryptions done\n";
}
void Bombe::setup_diagonal_board(const string ciphertext, const string crib) {
    //cout<<"setting up DB\n";
    for(unsigned int j=0; j<crib.length(); j++) {
        //cout<<"getting an encryption\n";
        vector<pair<int,int>> encryption=m_enigma_encryptions.at(j);
        //cout<<"connecting an enigma\n";
        m_diagonal_board->connect_enigma(&encryption, (int)crib[j]-(int)'A', (int)ciphertext[j]-(int)'A');
    }
}
void Bombe::analyze(const string ciphertext, const string crib) {
    //find total amount of rotor positions
    int total_permutations=1, most_wired_letter;
    for(int j=0; j<m_enigma->get_rotors(); j++) {
        total_permutations*=m_enigma->get_wires();
    }
    cout<<"analyzing\n";
    //find candidates
    vector<int> candidates=probable_search(ciphertext, crib);
    //analyze each candidate
    for(unsigned int i=0; i<candidates.size(); i++) {
        //find most commonly connected letter
        most_wired_letter=find_most_wired_letter(ciphertext.substr(candidates[i], crib.length()), crib);
        //setup the wiring for the can didate
        cout<<"candidate "<<i<<"\n";
        //setup crib.length() enigma encryptions
        init_enigma_encryptions(crib.length());
        //print_encryptions();
        //m_diagonal_board->print();
        //cout<<"HELL\n";
        for(int j=0; j<total_permutations-1; j++) {
            //cout<<"\n";
            //printf("\r%5d/%5d", j, total_permutations);
            //reset board
            m_diagonal_board->reset();
            //setup connections in board
            //cout<<"setup\n";
            setup_diagonal_board(ciphertext.substr(candidates[i], crib.length()), crib);
            //cout<<"checking\n";
            if (check_wiring(most_wired_letter)) {
                //m_diagonal_board->print_live();
                cout<<"VALID CONFIGURATION FOUND AT "<<i<<"\n";
                //cin.get();
            }
            //cout<<"checked\n";
            //take out oldest, put in
            //new encryption after enigma turns
            //print_encryptions();
            //cin.get();
            m_enigma->turn();
            m_enigma_encryptions.erase(m_enigma_encryptions.begin());; //erase first/oldest
            m_enigma_encryptions.push_back(m_enigma->get_encryption_onesided()); //add new
        }
        //cout<<"UUU\n";
    }

}
bool Bombe::bundle_contradiction(int bundle) {
       return m_diagonal_board->bundle_contradiction(bundle);
}
bool Bombe::check_wiring(int most_wired_letter) {
    //the diagonal board is properly wired, the rotors are in position
    //check if no contradictions

    //for each bundle, activate each wire, test each bundle for a contradiciton
    //(more than one live)
    for(int wire=0; wire<m_letters; ++wire) {
        //for(int wire=0; wire<m_letters; ++wire) {
            //if (wire!=bundle) {
                //cout<<"BUNDLE: "<<(char)(most_wired_letter+(int)'A');
                //cout<<"  WIRE: "<<(char)(wire+(int)'A');
                //cout<<"\n";
                m_diagonal_board->activate(most_wired_letter, wire);
                //m_diagonal_board->print_live();
                //cin.get();
                //m_diagonal_board->print();
                //sum=m_diagonal_board->bundle_sum();
                if (bundle_contradiction(most_wired_letter)) return false;
                m_diagonal_board->wipe(); //wipe current in wires
    }
    return true;
}
void Bombe::print_encryptions() const {
    for(int wire=0; wire<m_letters/2; ++wire) {
        for(unsigned int e=0; e<m_enigma_encryptions.size(); ++e) {
            printf("<%2d,%2d>   ",(m_enigma_encryptions[e])[wire].first,(m_enigma_encryptions[e])[wire].second);
        }
        cout<<"\n";
    }
}





/*
int main() {
    const Rotor I=                 Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
    const Rotor II=                Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
    const Rotor III=               Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
    const Reflector UKWR=      Reflector("QYHOGNECVPUZTFDJAXWMKISRBL"); //ref
    Bombe* bombe=new Bombe({I, II, III}, UKWR);
    //bombe->analyze("LCUCNXJXJCXZVWJBKULEERCLXZJZBSNNBFVRNBJDEBUUJGMRTZ", "THISISAPLAINTEXTMESSAGETOBEENCIPHEREDWITHTHEENIGM"); OK?
    //bombe->analyze("LCUCNXJXJCXZVWJBAZVFKUXIJDYLRNUUIGVDZDSQJQIUUJDWIXIPAMZJJMZWXLGFCPLAF", "THISISAPLAINTEXTMESSAGETOBEENCIPHERED");
    //                THISISAPLAINTEXTMESSAGETOBEENCIPHEREDWITHTHEENIGMAANDITISVERYVERYLONG
    /*ok benchmark!
    auto start = std::chrono::high_resolution_clock::now();
    bombe->analyze("LCUCNXJXJCXZVWJBAZVFKUXIJDYLRNUUIGVDZD", "THISISAPLAINTEXTMESSAGETOBEENCIPHERED");
    auto stop  = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout<<"Elapsed time "<< duration.count();
    



    //ok benchmark!
    auto start = std::chrono::high_resolution_clock::now();
    bombe->analyze("LCUCNXJXJCXZVWJBAZVFKUXIJDYLRNUUIGVDZDSQJQIUUJDWIXIPAMZJJMZWXLGFCPLAF", "TOBEENCIPHEREDWITHTHEENIGMAANDITISVERYVERY");
    auto stop  = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout<<"Elapsed time "<< duration.count();


    //bombe->analyze("GLKTSUAMKNQQYISUSNAEZLLBUN", "BCDEFGHIJKLM");
    //bombe->analyze("RTPTBZWSONHXBGYRSLVXZKKAUM", "DEFGH");
    //bombe->analyze("QFZWRWIVTYRESXBFOGKUHQBAISEZ", "WETTERVORHERSAGEBISKAYA");
    delete bombe;
}*/
