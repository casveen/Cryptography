//#include "enigma.h";
#include "bombe.hpp"   //wire, diagonal board

void update_performance(double &mean, double &var,
                        std::chrono::duration<double> measurement,
                        int &                         records) {
    auto   t     = measurement.count();
    double mean_p= mean;
    mean         = mean_p + (t - mean_p) / (records + 1);
    var          = (records * var + (t - mean_p) * (t - mean)) / (records + 1);
    records++;
}

Wire::~Wire() {
    // do not deallocate aythng, handled by diag board
    m_connections.clear();
    m_connections.shrink_to_fit();
}
void Wire::flow() {
    m_live= -1;
    // activate connected dead wires
    for (Wire *wire : m_connections) {
        if (wire->get_live() == 0) { wire->flow(); }
    }
    m_live= 1;
}

int  Wire::get_live() const { return m_live; }
void Wire::kill() { m_live= 0; }
void Wire::set_live(int t_set) { m_live= t_set; }
void Wire::reset() {
    kill();
    m_connections.clear();
}
vector<Wire *> *Wire::get_connections() { return &m_connections; }
void            Wire::connect(Wire *w) { m_connections.push_back(w); }

DiagonalBoard::DiagonalBoard(int t_bundles) {
    // initialize, in triangle
    for (int b= 0; b < t_bundles; b++) {
        // make wires for bundle
        vector<Wire *> wires;
        for (int w= 0; w <= b; w++) { wires.push_back(new Wire()); }
        // push onto bundle
        m_bundles.push_back(wires);
    }
    // fix symmetries
    for (int b= 0; b < t_bundles; b++) {
        for (int w= b + 1; w < t_bundles; w++) {
            m_bundles.at(b).push_back(m_bundles.at(w).at(b));
        }
    }
}
DiagonalBoard::~DiagonalBoard() {
    for (unsigned int b= 0; b < m_bundles.size(); ++b) {
        for (unsigned int w= 0; w <= b; ++w) { delete m_bundles.at(b).at(w); }
    }
    for (auto bundle : m_bundles) {
        bundle.clear();
        bundle.shrink_to_fit();
    }
    m_bundles.clear();
    m_bundles.shrink_to_fit();
}
Wire *DiagonalBoard::get_wire(int t_bundle, int t_wire) const {
    return m_bundles.at(t_bundle).at(t_wire);
}
void DiagonalBoard::activate(int t_bundle, int t_wire) {
    get_wire(t_bundle, t_wire)->flow();
}
void DiagonalBoard::wipe() {
    for (unsigned int bundle= 0; bundle < m_bundles.size(); ++bundle) {
        for (unsigned int wire= 0; wire <= bundle; ++wire) {
            get_wire(bundle, wire)->kill();
        }
    }
}
void DiagonalBoard::reset() {
    for (unsigned int bundle= 0; bundle < m_bundles.size(); ++bundle) {
        for (unsigned int wire= 0; wire <= bundle; ++wire) {
            get_wire(bundle, wire)->reset();
        }
    }
}
void DiagonalBoard::connect(int t_bundle_1, int t_wire_1, int t_bundle_2,
                            int t_wire_2) {
    get_wire(t_bundle_1, t_wire_1)->connect(get_wire(t_bundle_2, t_wire_2));
    // get_wire(t_bundle_1, t_wire_2)->connect(get_wire(t_bundle_2, t_wire_1));
    get_wire(t_bundle_2, t_wire_1)->connect(get_wire(t_bundle_1, t_wire_2));
    // XXX this is the correct secoind one!!!
    // get_wire(t_bundle_2,
    // t_wire_2)->connect(get_wire(t_bundle_1, t_wire_1));
}
void DiagonalBoard::connect_enigma(int *encryption, int t_from, int t_to) {
    int            m_letters= m_bundles.size();
    vector<Wire *> bundle_1 = m_bundles[t_from];
    vector<Wire *> bundle_2 = m_bundles[t_to];
    /*for (int i= 0; i < m_letters; i++) {
        bundle_1[i]->connect(bundle_2[encryption[i]]);
        bundle_1[encryption[i]]->connect(bundle_2[i]);
    }*/

    for (int i= 0; i < m_letters; i++) {
        connect(t_from, i, t_to, encryption[i]);
    }
}
int DiagonalBoard::bundle_sum(int bundle) const {
    int sum= 0;
    for (unsigned int i= 0; i < m_bundles.size(); ++i) {
        sum+= get_wire(bundle, i)->get_live();
    }
    return sum;
}
void DiagonalBoard::print() const {
    unsigned int bundle_size= m_bundles.size();
    // cout<<"bundle size "<<bundle_size<<"\n";
    cout << "  ";
    for (unsigned int b= 0; b < bundle_size; b++) {
        cout << (char)(b + (int)'a') << " ";
    }
    cout << "\n";
    for (unsigned int b= 0; b < bundle_size; b++) {
        // cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        cout << (char)(b + (int)'A') << " ";
        for (unsigned int w= 0; w < m_bundles.at(b).size(); w++) {
            Wire *wire= m_bundles.at(b).at(w);
            cout << ((wire->get_live()) ? '1' : ' ') << " ";
        }
        cout << "\n";
    }
}
void DiagonalBoard::print_connections() const {
    unsigned int bundle_size= m_bundles.size();
    cout << "   ";
    for (unsigned int b= 0; b < bundle_size; b++) {
        cout << (char)(b + (int)'a') << "  ";
    }
    cout << "\n";
    for (unsigned int b= 0; b < bundle_size; b++) {
        // cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        cout << (char)(b + (int)'A') << " ";
        for (unsigned int w= 0; w < m_bundles.at(b).size(); w++) {
            Wire *wire= m_bundles.at(b).at(w);
            printf("%2ld ", wire->get_connections()->size());
        }
        cout << "\n";
    }
}
void DiagonalBoard::print_live() const {
    int bundle_size= m_bundles.size();
    for (int b= 0; b < bundle_size; b++) { cout << " | "; }
    cout << "\n";
    for (int b= 0; b < bundle_size; b++) {
        cout << " " << (char)(b + (int)'A') << " ";
    }
    cout << "\n";
    // cout<<"bundle size "<<bundle_size<<"\n";
    for (int b= 0; b < bundle_size; b++) {
        // cout<<"wires size "<<m_bundles.at(b).size()<<"\n";
        printf("%2d ", bundle_sum(b));
    }
    cout << "\n";
    cout << "\n";
}
bool DiagonalBoard::bundle_contradiction(int bundle) const {
    int ones= 0, zeros= 0;
    // we have a contradiction if there is not exactly 1 or exactly 25 live
    // wires
    for (unsigned int i= 0; i < m_bundles.size(); ++i) {
        get_wire(bundle, i)->get_live() == 0 ? ++zeros : ++ones;
        //  1 impossible, 25 impossible
        if (ones >= 2 && zeros >= 2) return true;
    }
    // if only ones or only zeros, we have a contradiciton
    if (ones == 0 || zeros == 0) return true;
    // otherwise, the bundle wiring is valid(no contradiction)
    return false;
}

BombeUnit::BombeUnit(const std::initializer_list<Rotor> rotors,
                     const Reflector                    reflector) {
    m_letters       = (rotors.begin())->get_wires();
    m_rotor_count   = rotors.size();
    m_diagonal_board= new DiagonalBoard(m_letters);
    m_enigma        = new Enigma(rotors, reflector);
}
BombeUnit::~BombeUnit() {
    delete m_diagonal_board;
    delete m_enigma;
    for (auto encryption : m_enigma_encryptions) { delete[] encryption; }
    m_enigma_encryptions.clear();
    m_enigma_encryptions.shrink_to_fit();
}
// setters
void BombeUnit::set_ring_setting(const string setting) {
    m_enigma->set_ring_setting(setting);
}
void BombeUnit::set_rotor_position(const string setting) {
    m_enigma->set_rotor_position(setting);
}
// getters
struct BombeSetting &BombeUnit::get_setting() {
    return m_setting;
}
// other
vector<int> BombeUnit::probable_search(const string ciphertext,
                                       const string crib) {
    vector<int> candidates;
    // find suitable pattern
    int  ciphertext_length= ciphertext.length(), crib_length= crib.length();
    bool suitable;
    for (int i= 0; i <= ciphertext_length - crib_length; i++) {
        // compare, if same letter on same pos we have a contradiction
        suitable= true;
        for (int j= 0; j < crib_length; j++) {
            if (ciphertext[i + j] == crib[j]) {
                suitable= false;
                break;
            }
        }
        if (suitable) {
            // cout << "suitable substring at " << i << "\n";
            // cout << ciphertext.substr(i, crib_length) << "\n" << crib <<
            // "\n";
            candidates.push_back(i);
        }
    }
    if (m_setting.only_one_candidate) {
        candidates.erase(candidates.begin() + 1, candidates.end());
    }
    return candidates;
}
int BombeUnit::find_most_wired_letter(const string ciphertext,
                                      const string crib) {
    // make histogram, find most frequent element
    int *histogram= new int[m_letters];
    for (int i= 0; i < m_letters; ++i) { histogram[i]= 0; }
    for (unsigned int i= 0; i < min(ciphertext.length(), crib.length()); ++i) {
        histogram[(int)ciphertext[i] - (int)'A']++;
        histogram[(int)crib[i] - (int)'A']++;
    }
    int max_value= 0, max_index= -1;
    for (int i= 0; i < m_letters; ++i) {
        if (histogram[i] > max_value) {
            max_value= histogram[i];
            max_index= i;
        }
    }
    delete[] histogram;
    return max_index;
}
void BombeUnit::init_enigma_encryptions(int             encryptions,
                                        vector<string> &positions) {
    /*auto start_init_encryptions= std::chrono::system_clock::now();
    if (m_setting.time_performance) {
        start_init_encryptions= std::chrono::system_clock::now();
    }*/

    m_enigma_encryptions.clear();
    positions.clear();
    positions.push_back(m_enigma->get_rotor_position_as_string());
    for (int i= 0; i < encryptions; ++i) {

        m_enigma->turn();   // enigma turns before encryption
        m_enigma_encryptions.push_back(m_enigma->get_encryption());
        positions.push_back(m_enigma->get_rotor_position_as_string());
    }

    /*if (m_setting.time_performance) {
        auto stop_init_encryptions= std::chrono::system_clock::now();
        update_performance(m_setting.performance_init_encryptions_mean,
                           m_setting.performance_init_encryptions_var,
                           stop_init_encryptions - start_init_encryptions,
                           m_setting.records_init_encryptions);
    }*/
}
void BombeUnit::reset_diagonal_board() {
    /*auto start_reset_diagonal_board= std::chrono::system_clock::now();
    if (m_setting.time_performance) {
        start_reset_diagonal_board= std::chrono::system_clock::now();
    }*/

    m_diagonal_board->reset();

    /*if (m_setting.time_performance) {
        auto stop_reset_diagonal_board= std::chrono::system_clock::now();
        update_performance(m_setting.performance_reset_diagonal_board_mean,
                           m_setting.performance_reset_diagonal_board_var,
                           stop_reset_diagonal_board -
                               stop_reset_diagonal_board,
                           m_setting.records_reset_diagonal_board);
    }*/
}
void BombeUnit::setup_diagonal_board(const string ciphertext,
                                     const string crib) {
    /*auto start_setup_diagonal_board= std::chrono::system_clock::now();
    if (m_setting.time_performance) {
        start_setup_diagonal_board= std::chrono::system_clock::now();
    }*/

    for (unsigned int j= 0; j < crib.length(); j++) {
        int *encryption= m_enigma_encryptions.at(j);
        m_diagonal_board->connect_enigma(encryption, (int)crib[j] - (int)'A',
                                         (int)ciphertext[j] - (int)'A');
    }

    /*if (m_setting.time_performance) {
        auto stop_setup_diagonal_board= std::chrono::system_clock::now();
        update_performance(m_setting.performance_setup_diagonal_board_mean,
                           m_setting.performance_setup_diagonal_board_var,
                           stop_setup_diagonal_board -
                               stop_setup_diagonal_board,
                           m_setting.records_setup_diagonal_board);
    }*/
}
bool BombeUnit::bundle_contradiction(int bundle) {
    return m_diagonal_board->bundle_contradiction(bundle);
}
bool BombeUnit::check_one_wire(int most_wired_letter) {
    /*auto start_check_one_wire= std::chrono::system_clock::now();
    if (m_setting.time_performance) {
        start_check_one_wire= std::chrono::system_clock::now();
    }*/
    m_diagonal_board->activate(most_wired_letter, 4);
    bool contradiction= bundle_contradiction(most_wired_letter);
    /*if (m_setting.time_performance) {
        auto stop_check_one_wire= std::chrono::system_clock::now();
        update_performance(m_setting.performance_check_one_wire_mean,
                           m_setting.performance_check_one_wire_var,
                           stop_check_one_wire - stop_check_one_wire,
                           m_setting.records_check_one_wire);
    }*/
    return (!contradiction);
}
// vector<string> BombeUnit::unique_ring_settings() {}

vector<struct EnigmaSetting> BombeUnit::analyze(const string ciphertext,
                                                const string crib) {
    // find total amount of rotor positions
    vector<struct EnigmaSetting> solutions;
    int                          total_permutations= 1;
    int                          most_wired_letter;
    int                          ring_settings= 1;
    for (int j= 0; j < m_enigma->get_rotors(); j++) {
        total_permutations*= m_enigma->get_wires();
    }
    ring_settings= total_permutations / m_enigma->get_wires();
    ring_settings= min(ring_settings, m_setting.max_ring_settings);

    // timing variables
    auto start_ring_setting= std::chrono::system_clock::now();

    // find candidates

    vector<int>    candidates= probable_search(ciphertext, crib);
    vector<string> rotor_positions;   // used to reset

    // analyze each candidate
    for (unsigned int i= 0; i < candidates.size(); i++) {
        m_enigma->set_ring_setting(m_setting.starting_ring_setting);
        m_enigma->set_rotor_position(m_setting.starting_rotor_positions);
        // find most commonly connected letter
        most_wired_letter= find_most_wired_letter(
            ciphertext.substr(candidates[i], crib.length()), crib);
        // for each ring setting
        for (int rs= 0; rs < ring_settings; ++rs) {
            cout << "I: " << i
                 << ", Ring setting: " << m_enigma->get_ring_setting_as_string()
                 << "\n";
            if (m_setting.time_performance) {
                start_ring_setting= std::chrono::system_clock::now();
            }

            // setup the wiring for the candidate
            init_enigma_encryptions(crib.length(), rotor_positions);

            // for each rotor position
            for (int j= 0; j < total_permutations - 1; j++) {

                reset_diagonal_board();

                setup_diagonal_board(
                    ciphertext.substr(candidates[i], crib.length()), crib);

                if (check_one_wire(most_wired_letter)) {
                    // what happens here is not neccesary to optimize
                    if (doublecheck_and_get_plugboard()) {
                        // tripple chack:
                        m_enigma->set_rotor_position(
                            rotor_positions[rotor_positions.size() -
                                            crib.length() - 1]);
                        string recrypt= m_enigma->encrypt(
                            ciphertext.substr(candidates[i], crib.length()));
                        if (m_setting.interactive_wiring_mode) {
                            interactive_wirechecking();
                        }
                        if (recrypt == crib) {
                            m_enigma->set_rotor_position(rotor_positions[0]);
                            solutions.push_back(m_enigma->get_setting());
                            m_enigma->set_rotor_position(
                                rotor_positions[rotor_positions.size() - 1]);
                            if (m_setting.stop_on_first_valid == true) {
                                return solutions;
                            }
                        }
                        m_enigma->set_plugboard("");   // reset plugboard
                    }
                }
                // take the oldest encryption, give the newest encryption and
                // push to front
                m_enigma->turn();
                int *encryption= m_enigma_encryptions.at(0);
                m_enigma->get_encryption_inplace(encryption);
                m_enigma_encryptions.erase(m_enigma_encryptions.begin());
                m_enigma_encryptions.push_back(encryption);
                if (rotor_positions.size() > crib.length() + candidates[i]) {
                    rotor_positions.erase(rotor_positions.begin());
                }
                rotor_positions.push_back(
                    m_enigma->get_rotor_position_as_string());
            }   // for rotor position
            m_enigma->next_ring_setting();

            if (m_setting.time_performance == true) {
                auto stop_ring_setting= std::chrono::system_clock::now();
                update_performance(m_setting.performance_ring_setting_mean,
                                   m_setting.performance_ring_setting_var,
                                   stop_ring_setting - start_ring_setting,
                                   m_setting.records_ring_setting);
            }
            if (m_setting.time_performance) { print_performance(); }
        }   // for ring setting
    }
    return solutions;
}

void BombeUnit::print_encryptions() const {
    cout << "size of encryptions: " << m_enigma_encryptions.size() << "\n";
    for (int wire= 0; wire < m_letters; ++wire) {
        cout << (char)(wire + (int)'A') << ": ";
        for (unsigned int e= 0; e < m_enigma_encryptions.size(); ++e) {
            cout << (char)(m_enigma_encryptions.at(e)[wire] + (int)'A') << " ";
            // printf("%2d-", m_enigma_encryptions.at(e)[wire]);
        }
        cout << "\n";
    }
}
void BombeUnit::print_performance() const {
    // ring_setting
    printf("---------------------------------------------------\n");
    printf("|                   MEAN       VAR        RECORDS |\n");
    printf("| RING-SETTING      %6.2E   %6.2E   %7d |\n",
           m_setting.performance_ring_setting_mean,
           m_setting.performance_ring_setting_var,
           m_setting.records_ring_setting);
    /*printf("| INIT-ENCRYPTIONS  %6.2E   %6.2E   %7d |\n",
           m_setting.performance_init_encryptions_mean,
           m_setting.performance_init_encryptions_var,
           m_setting.records_init_encryptions);
    printf("| DB SETUP          %6.2E   %6.2E   %7d |\n",
           m_setting.performance_setup_diagonal_board_mean,
           m_setting.performance_setup_diagonal_board_var,
           m_setting.records_setup_diagonal_board);
    printf("| CHECK ONE WIRE    %6.2E   %6.2E   %7d |\n",
           m_setting.performance_check_one_wire_mean,
           m_setting.performance_check_one_wire_var,
           m_setting.records_check_one_wire);
    printf("| DB RESET          %6.2E   %6.2E   %7d |\n",
           m_setting.performance_reset_diagonal_board_mean,
           m_setting.performance_reset_diagonal_board_var,
           m_setting.records_reset_diagonal_board);*/
    printf("---------------------------------------------------\n");
}
/*
bool BombeUnit::doublecheck_and_get_plugboard() {
    //bool false_stop= false;
    // we have a valid configuration od the enigma, no we have to double check
    // that there are no other contradictions
    // there are a lot of double-checks here, but valid configurations are rare
    // and this double-check is insignificant compared to the analysis.
    Plugboard *plugboard= m_enigma->get_cartridge()->get_plugboard();
    for (int bundle= 0; bundle < m_letters; ++bundle) {
        for (int wire= 0; wire < m_letters; ++wire) {
            m_diagonal_board->wipe();
            m_diagonal_board->activate(bundle, wire);
            // cout << "sum at bundle=" << bundle << ":"
            //     << m_diagonal_board->bundle_sum(bundle) << "\n";
            if (m_diagonal_board->bundle_sum(bundle) == 1) {   // exact hit
                // cout << "EXACT HIT\n";
                // also if exact hit, there is only one live wire per bundle
                for (int bundle_2= 0; bundle_2 < m_letters; ++bundle_2) {
                     //cout << "sum at bunde_2=" << (char) (bundle_2+(int)'A')
<< ":"
                    //      << m_diagonal_board->bundle_sum(bundle_2) << "\n";
                    if (m_diagonal_board->bundle_sum(bundle_2) > 1) {
                        // cout<<"FCUK\n";
                        plugboard->reset();
                        return false;   // a contradiciton; this was a false
                                        // stop
                    }
                }

                cout << (char) (bundle +(int)'A') << " is steckered to " <<
(char) (wire +(int)'A') << "\n"; plugboard->set_wiring(bundle, wire);   // other
way by symmetry
            }
        }
    }
    return true;
}
*/

bool BombeUnit::doublecheck_and_get_plugboard() {
    // bool false_stop= false;
    // we have a valid configuration od the enigma, the most occurring letter is
    // activated in some wire.
    // for all bundles, the sum is either
    // 1 : the steckered letter is live
    // 25: all but the steckered letter is live
    // other: unable to find, probably self-steckered
    Plugboard *plugboard= m_enigma->get_cartridge()->get_plugboard();
    for (int bundle= 0; bundle < m_letters; ++bundle) {
        int sum= m_diagonal_board->bundle_sum(bundle);
        if (sum == 1) {   // steckered is live
            // all other bundles should have 1 or less live wires
            for (int bundle_2= 0; bundle_2 < m_letters; ++bundle_2) {
                if (m_diagonal_board->bundle_sum(bundle) > 1) {
                    plugboard->reset();
                    return false;
                }
            }
            // find live wire
            for (int wire= 0; wire < m_letters; ++wire) {
                if (m_diagonal_board->get_wire(bundle, wire)->get_live()) {
                    // cout << (char) (bundle +(int)'A') << " is steckered to "
                    // <<  (char) (wire +(int)'A') << "\n";
                    plugboard->set_wiring(bundle, wire);
                    plugboard->set_wiring(wire, bundle);
                    break;
                }
            }
        } else if (sum == m_letters - 1) {   // steckered is dead
            // find dead wire
            for (int wire= 0; wire < m_letters; ++wire) {
                if (!m_diagonal_board->get_wire(bundle, wire)->get_live()) {
                    plugboard->set_wiring(bundle, wire);
                    plugboard->set_wiring(wire, bundle);
                    break;
                }
            }
        } else {   // undeterminable
        }
    }
    return true;
}

void BombeUnit::interactive_wirechecking() {
    cout << "--------------------INTERACTIVE WIRING MODE------------------\n";
    string input;
    int    bundle, wire;
    while (true) {
        cout << "    INPUT A WIRE TO ACTIVATE(q to EXIT): ";
        cin >> input;
        if (input == "q") { break; }
        bundle= (int)input[0] - (int)'A';
        wire  = (int)input[1] - (int)'a';
        if (bundle > m_letters || bundle < 0 || wire > m_letters || wire < 0) {
            cout << "WRONG INPUT FORMAT. USAGE: Aa, Ab, Zk\n";
            continue;
        }
        cout << "activating " << (char)(bundle + (int)'A')
             << (char)(wire + (int)'a') << "\n";
        m_diagonal_board->wipe();
        m_diagonal_board->activate(bundle, wire);
        m_diagonal_board->print();
    }
}
vector<string> BombeUnit::get_special_rotor_positions() {
    /*the turning of the engma rotors is not a surjective function.
    this function returns the rotor positions not hit when turning from
    bombesetting::starting_rotor_position
    The problem stems from the double stepping mechanism, which occurs when a
    non fast rotor is turned and its left rotor is in notch. It then turs twice
    meaning that the 25 rotor posiitons where the given rotor was in its first
    stepped position is ignored*/
}
/*
vector<string> equivalent_ring_settings() {
    // when running the bombe with a small crib, if we have a solution, often
    // the same rotor position minus one turn is a solution when checking the
    // next ring setting. return all these settings.
}*/

/*
int main() {
    const Rotor I=                 Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q");
    const Rotor II=                Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E");
    const Rotor III=               Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V");
    const Reflector UKWR=      Reflector("QYHOGNECVPUZTFDJAXWMKISRBL"); //ref
    BombeUnit* bombe=new BombeUnit({I, II, III}, UKWR);
    //bombe->analyze("LCUCNXJXJCXZVWJBKULEERCLXZJZBSNNBFVRNBJDEBUUJGMRTZ",
"THISISAPLAINTEXTMESSAGETOBEENCIPHEREDWITHTHEENIGM"); OK?
    //bombe->analyze("LCUCNXJXJCXZVWJBAZVFKUXIJDYLRNUUIGVDZDSQJQIUUJDWIXIPAMZJJMZWXLGFCPLAF",
"THISISAPLAINTEXTMESSAGETOBEENCIPHERED");
    // THISISAPLAINTEXTMESSAGETOBEENCIPHEREDWITHTHEENIGMAANDITISVERYVERYLONG ok
benchmark! auto start = std::chrono::high_resolution_clock::now();
    bombe->analyze("LCUCNXJXJCXZVWJBAZVFKUXIJDYLRNUUIGVDZD",
"THISISAPLAINTEXTMESSAGETOBEENCIPHERED"); auto stop  =
std::chrono::high_resolution_clock::now(); auto duration =
std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout<<"Elapsed time "<< duration.count();




    //ok benchmark!
    auto start = std::chrono::high_resolution_clock::now();
    bombe->analyze("LCUCNXJXJCXZVWJBAZVFKUXIJDYLRNUUIGVDZDSQJQIUUJDWIXIPAMZJJMZWXLGFCPLAF",
"TOBEENCIPHEREDWITHTHEENIGMAANDITISVERYVERY"); auto stop  =
std::chrono::high_resolution_clock::now(); auto duration =
std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout<<"Elapsed time "<< duration.count();


    //bombe->analyze("GLKTSUAMKNQQYISUSNAEZLLBUN", "BCDEFGHIJKLM");
    //bombe->analyze("RTPTBZWSONHXBGYRSLVXZKKAUM", "DEFGH");
    //bombe->analyze("QFZWRWIVTYRESXBFOGKUHQBAISEZ", "WETTERVORHERSAGEBISKAYA");
    delete bombe;
}*/
