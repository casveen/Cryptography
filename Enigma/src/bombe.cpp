//#include "enigma.h";
#include "bombe.hpp"   //wire, diagonal board
#include "statistics.hpp"
const int PROGRESS_BAR_WIDTH= 20;

void update_performance(double &mean, double &var, std::chrono::duration<double> measurement,
                        int &records) {
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
    m_live= true;
    // activate connected dead wires
    for (Wire *wire : m_connections) {
        if (wire->get_live() == false) { wire->flow(); }
    }
}
bool Wire::get_live() const { return m_live; }
void Wire::kill() { m_live= false; }
void Wire::set_live(bool t_set) { m_live= t_set; }
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
        for (int w= b + 1; w < t_bundles; w++) { m_bundles.at(b).push_back(m_bundles.at(w).at(b)); }
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
void DiagonalBoard::activate(int t_bundle, int t_wire) { get_wire(t_bundle, t_wire)->flow(); }
void DiagonalBoard::wipe() {
    for (unsigned int bundle= 0; bundle < m_bundles.size(); ++bundle) {
        for (unsigned int wire= 0; wire <= bundle; ++wire) { get_wire(bundle, wire)->kill(); }
    }
}
void DiagonalBoard::reset() {
    for (unsigned int bundle= 0; bundle < m_bundles.size(); ++bundle) {
        for (unsigned int wire= 0; wire <= bundle; ++wire) { get_wire(bundle, wire)->reset(); }
    }
}
void DiagonalBoard::connect(int t_bundle_1, int t_wire_1, int t_bundle_2, int t_wire_2) {
    get_wire(t_bundle_1, t_wire_1)->connect(get_wire(t_bundle_2, t_wire_2));
    // get_wire(t_bundle_1, t_wire_2)->connect(get_wire(t_bundle_2, t_wire_1));
    get_wire(t_bundle_2, t_wire_2)->connect(get_wire(t_bundle_1, t_wire_1));
    // XXX this is the correct secoind one!!!
    // get_wire(t_bundle_2,
    // t_wire_2)->connect(get_wire(t_bundle_1, t_wire_1));
}
void DiagonalBoard::connect_enigma(int *encryption, int t_from, int t_to) {
    int m_letters= m_bundles.size();
    // vector<Wire *> bundle_1 = m_bundles[t_from];
    // vector<Wire *> bundle_2 = m_bundles[t_to];
    /*for (int i= 0; i < m_letters; i++) {
        bundle_1[i]->connect(bundle_2[encryption[i]]);
        bundle_1[encryption[i]]->connect(bundle_2[i]);
    }*/

    for (int i= 0; i < m_letters; i++) { connect(t_from, i, t_to, encryption[i]); }
}
int DiagonalBoard::bundle_sum(int bundle) const {
    int sum= 0;
    for (unsigned int i= 0; i < m_bundles.size(); ++i) { sum+= get_wire(bundle, i)->get_live(); }
    return sum;
}
void DiagonalBoard::print() const {
    unsigned int bundle_size= m_bundles.size();
    // cout<<"bundle size "<<bundle_size<<"\n";
    cout << "  ";
    for (unsigned int b= 0; b < bundle_size; b++) { cout << (char)(b + (int)'a') << " "; }
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
    for (unsigned int b= 0; b < bundle_size; b++) { cout << (char)(b + (int)'a') << "  "; }
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
    for (int b= 0; b < bundle_size; b++) { cout << " " << (char)(b + (int)'A') << " "; }
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

BombeUnit::BombeUnit(const std::initializer_list<Rotor> rotors, const Reflector reflector) :
    BombeUnit(vector<Rotor>(rotors), reflector) {}
BombeUnit::BombeUnit(const vector<Rotor> rotors, const Reflector reflector) {
    m_letters       = (rotors.begin())->get_wires();
    m_rotor_count   = rotors.size();
    m_diagonal_board= new DiagonalBoard(m_letters);
    m_enigma        = new Enigma(rotors, reflector);
    m_identifier    = "";
    for (unsigned int r= 0; r < rotors.size(); ++r) {
        m_identifier+= rotors[r].get_name();
        m_identifier+= "-";
    }
    m_identifier+= reflector.get_name();
}
BombeUnit::BombeUnit(struct EnigmaSetting enigma_setting) {
    // plugboard, rotor position are ignored
    // TODO delegate to top constructor
    m_letters                         = (enigma_setting.rotors.begin())->get_wires();
    m_rotor_count                     = enigma_setting.rotors.size();
    m_setting.starting_ring_setting   = enigma_setting.ring_setting;
    m_setting.starting_rotor_positions= enigma_setting.rotor_position;
    m_diagonal_board                  = new DiagonalBoard(m_letters);
    m_enigma                          = new Enigma(enigma_setting);
    m_identifier                      = "";
    for (unsigned int r= 0; r < enigma_setting.rotors.size(); ++r) {
        m_identifier+= enigma_setting.rotors[r].get_name();
        m_identifier+= "-";
    }
    m_identifier+= enigma_setting.reflector.get_name();
}
BombeUnit::~BombeUnit() {
    delete m_diagonal_board;
    delete m_enigma;
    for (int *encryption : m_enigma_encryptions) { delete[] encryption; }
    m_enigma_encryptions.clear();
    m_enigma_encryptions.shrink_to_fit();
}
// setters
void BombeUnit::set_ring_setting(const string setting) { m_enigma->set_ring_setting(setting); }
void BombeUnit::set_rotor_position(const string setting) { m_enigma->set_rotor_position(setting); }
// getters
struct BombeUnitSetting &BombeUnit::get_setting() {
    return m_setting;
}
// other
vector<int> BombeUnit::probable_search(const string &ciphertext, const string &crib) {
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
            if (m_verbose) {
                cout << "suitable substring at " << i << ": ";
                cout << ciphertext.substr(i, crib_length) << " <-> " << crib << "\n";
            }
            candidates.push_back(i);
        }
    }
    if (candidates.size() == 0) {
        cout << "WARNING: no candidates found, exiting\n";
        return candidates;
    }
    if (m_setting.only_one_candidate) {
        candidates.erase(candidates.begin() + 1, candidates.end());
    }
    return candidates;
}
int BombeUnit::find_most_wired_letter(const string &ciphertext, const string &crib) {
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
void BombeUnit::init_enigma_encryptions(int encryptions, vector<string> &positions) {
    for (int *encryption : m_enigma_encryptions) { delete[] encryption; }
    m_enigma_encryptions.clear();
    positions.clear();
    positions.push_back(m_enigma->get_rotor_position_as_string());
    for (int i= 0; i < encryptions; ++i) {

        m_enigma->turn();   // enigma turns before encryption
        m_enigma_encryptions.push_back(m_enigma->get_encryption());
        positions.push_back(m_enigma->get_rotor_position_as_string());
    }
}
void BombeUnit::reset_diagonal_board() { m_diagonal_board->reset(); }
void BombeUnit::setup_diagonal_board(const string &ciphertext, const string &crib) {
    for (unsigned int j= 0; j < crib.length(); j++) {
        int *encryption= m_enigma_encryptions.at(j);
        m_diagonal_board->connect_enigma(encryption, (int)crib[j] - (int)'A',
                                         (int)ciphertext[j] - (int)'A');
    }
}

vector<struct EnigmaSetting> BombeUnit::analyze(const string &ciphertext, const string &crib) {
    // find total amount of rotor positions
    vector<struct EnigmaSetting> solutions;
    int                          total_permutations= m_enigma->get_wires();
    int            most_wired_letter, crib_n= crib.length();   // ciphertext_n= ciphertext.length();
    int            ring_settings= 1;
    vector<string> rotor_positions;   // used to reset when valid found
    /*for (int j= 1; j < m_enigma->get_rotors() - 1; j++) {
        total_permutations*=
            m_enigma->get_wires() - m_enigma.get_rotors()[j].get_notches();
    }
    total_permutations*= m_enigma->get_wires();*/
    total_permutations= m_enigma->compute_total_permutations_brute_force();   // TODO
    ring_settings     = total_permutations / m_enigma->get_wires();           // TODO
    ring_settings     = min(ring_settings, m_setting.max_ring_settings);

    // find candidates
    vector<int> candidates  = probable_search(ciphertext, crib);
    int         candidates_n= (int)candidates.size();
    if (candidates.size() == 0) { return solutions; }   // no candidates

    // analyze each candidate
    auto start_ring_setting= std::chrono::system_clock::now();
    for (int i= 0; i < candidates_n; i++) {
        int candidate= candidates[i];
        m_enigma->set_ring_setting(m_setting.starting_ring_setting);
        m_enigma->set_rotor_position(m_setting.starting_rotor_positions);
        most_wired_letter= find_most_wired_letter(ciphertext.substr(candidate, crib_n), crib);
        // for each ring setting
        for (int rs= 0; rs < ring_settings; ++rs) {
            init_enigma_encryptions(crib_n, rotor_positions);
            print_progress(rs, ring_settings, i, candidates_n);
            if (m_setting.time_performance) {
                start_ring_setting= std::chrono::system_clock::now();
            }
            // for each rotor position
            for (int j= 0; j < total_permutations - 1; j++) {
                // check_position() TODO
                reset_diagonal_board();
                setup_diagonal_board(ciphertext.substr(candidate, crib_n), crib);
                // BEGIN TESTS
                if (check_one_wire(most_wired_letter)) {     // first test
                    if (doublecheck_and_get_plugboard()) {   // second test
                        if (tripplecheck(crib, ciphertext, candidate,
                                         rotor_positions)) {   // final test
                            m_enigma->set_rotor_position(rotor_positions[0]);
                            // m_enigma->get_setting().reflector.print();
                            solutions.push_back(m_enigma->get_setting());
                            m_enigma->set_rotor_position(rotor_positions.back());
                            if (m_setting.stop_on_first_valid) { return solutions; }
                        }
                        m_enigma->set_plugboard("");   // reset plugboard
                    }
                }
                // END TESTS
                // shuffle arrays TODO make own function
                m_enigma->turn();
                int *encryption= m_enigma_encryptions.front();
                m_enigma->get_encryption_inplace(encryption);
                m_enigma_encryptions.erase(m_enigma_encryptions.begin());
                m_enigma_encryptions.push_back(encryption);
                if ((int)rotor_positions.size() > crib_n + candidate) {
                    rotor_positions.erase(rotor_positions.begin());
                }
                rotor_positions.push_back(m_enigma->get_rotor_position_as_string());
            }   // for rotor position

            // do the same for special rotor positions
            // init_enigma_encryptions(crib_n, rotor_positions);

            m_enigma->next_ring_setting();

            if (m_setting.time_performance) {
                auto stop_ring_setting= std::chrono::system_clock::now();
                update_performance(
                    m_setting.performance_ring_setting_mean, m_setting.performance_ring_setting_var,
                    stop_ring_setting - start_ring_setting, m_setting.records_ring_setting);
            }

        }   // for ring setting
        cout << "\r";
        if (m_setting.time_performance && m_verbose) { print_performance(); }
    }
    return solutions;
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
bool BombeUnit::bundle_contradiction(int bundle) {
    return m_diagonal_board->bundle_contradiction(bundle);
}
bool BombeUnit::check_one_wire(int most_wired_letter) {
    m_diagonal_board->activate(most_wired_letter, 4);
    return (!bundle_contradiction(most_wired_letter));
}
bool BombeUnit::doublecheck_and_get_plugboard() {
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
bool BombeUnit::tripplecheck(const string &crib, const string &ciphertext, int candidate,
                             vector<string> &rotor_positions) {
    // test if the given configuration encrypts the crib to plaintext
    // turn back
    m_enigma->set_rotor_position(rotor_positions[rotor_positions.size() - crib.length() - 1]);
    string recrypt= m_enigma->encrypt(ciphertext.substr(candidate, crib.length()));
    if (m_setting.interactive_wiring_mode) { interactive_wirechecking(); }
    return (recrypt == crib);
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
        cout << "activating " << (char)(bundle + (int)'A') << (char)(wire + (int)'a') << "\n";
        m_diagonal_board->wipe();
        m_diagonal_board->activate(bundle, wire);
        m_diagonal_board->print();
    }
}
void BombeUnit::print_progress(int ring_setting, int max_ring_settings, int candidate,
                               int max_candidates) {
    cout << "\r";
    string progress_bar= "[";
    for (int i= 0; i < PROGRESS_BAR_WIDTH; ++i) {
        progress_bar+=
            i <= (PROGRESS_BAR_WIDTH * (ring_setting + 1)) / max_ring_settings ? "#" : " ";
    }
    progress_bar+= "]";
    cout << progress_bar;
    cout << " [" << m_identifier << "]";
    printf(" (candidate %2d/%2d) ", candidate, max_candidates);
    cout << "RS: " << m_enigma->get_ring_setting_as_string() << flush;
}
void BombeUnit::print_encryptions() const {
    cout << "size of encryptions: " << m_enigma_encryptions.size() << "\n";
    for (int wire= 0; wire < m_letters; ++wire) {
        cout << (char)(wire + (int)'A') << ": ";
        for (unsigned int e= 0; e < m_enigma_encryptions.size(); ++e) {
            cout << (char)(m_enigma_encryptions.at(e)[wire] + (int)'A') << " ";
        }
        cout << "\n";
    }
}
void BombeUnit::print_performance() const {
    // ring_setting
    printf("---------------------------------------------------\n");
    printf("|                   MEAN       VAR        RECORDS |\n");
    printf("| RING-SETTING      %6.2E   %6.2E   %7d |\n", m_setting.performance_ring_setting_mean,
           m_setting.performance_ring_setting_var, m_setting.records_ring_setting);
    printf("---------------------------------------------------\n");
}

// bool compare_enigmaSetting(struct EnigmaSetting setting1, struct EnigmaSetting setting2, int
// length) TODO find if equivalent over a given length

Bombe::Bombe(const initializer_list<Rotor> rotors, const Reflector reflector) :
    m_reflector{Reflector(reflector)} {
    m_rotors= vector<Rotor>(rotors);
}
/*Bombe::Bombe(struct EnigmaSetting enigma_setting) {
    m_unit_setting= enigma_setting;   // XXX shallow copy might not be appropriate
}*/

/*vector<vector<Rotor>> Bombe::get_rotor_configurations(vector<Rotor> in) {
    return choices_of<Rotor>(m_rotors);
}*/

vector<struct EnigmaSetting> Bombe::analyze(const string &ciphertext, const string &crib) {
    vector<struct EnigmaSetting> solutions;
    // make units with different rotor orders, and make them analyze
    cout << "analyzing in bombe...\n";
    vector<vector<Rotor>> rotor_configurations= choices_of(m_rotors, m_setting.rotor_count);
    if (m_setting.only_one_configuration) {
        rotor_configurations.erase(rotor_configurations.begin() + 1, rotor_configurations.end());
    }
    /*for (vector<Rotor> rotor_configuration : rotor_configurations) {
        for (Rotor r : rotor_configuration) { r.print(); cout << r.get_name() << "-"; }
        m_reflector.print();
        cout << "\n\n\n";
    }*/
    // spawn units that analyze
    for (vector<Rotor> rotor_configuration : rotor_configurations) {
        // TODO thread it
        BombeUnit unit(rotor_configuration, m_reflector);
        // translate settings
        unit.get_setting().only_one_candidate      = m_setting.only_one_candidate;
        unit.get_setting().max_ring_settings       = m_setting.max_ring_settings;
        unit.get_setting().starting_ring_setting   = m_setting.starting_ring_setting;
        unit.get_setting().starting_rotor_positions= m_setting.starting_rotor_positions;
        unit.get_setting().only_one_candidate      = m_setting.only_one_candidate;
        unit.get_setting().stop_on_first_valid     = m_setting.stop_on_first_valid;
        vector<struct EnigmaSetting> solutions_unit= unit.analyze(ciphertext, crib);
        // solutions.insert(solutions.end(), solutions_unit.begin(), solutions_unit.end());
        for (struct EnigmaSetting solution : solutions_unit) { solutions.push_back(solution); }

        if (m_setting.stop_on_first_valid && solutions.size() > 0) { return solutions; }
    }
    return solutions;
}
struct BombeSetting &Bombe::get_setting() {
    return m_setting;
}
