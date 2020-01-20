//#include "enigma.h";
#include "bombe.hpp"   //wire, diagonal board
#include "statistics.hpp"
#include <cmath>
#include <omp.h>
const int PROGRESS_BAR_WIDTH= 20;

void update_performance(double &mean, double &var, std::chrono::duration<double> measurement,
                        int &records) {
    auto   t     = measurement.count();
    double mean_p= mean;
    mean         = mean_p + (t - mean_p) / (records + 1);
    var          = (records * var + (t - mean_p) * (t - mean)) / (records + 1);
    records++;
}
void vector_from_array_inplace(vector<shint> &vec, const int *array, int length) {
    for (int i= 0; i < length; ++i) { vec[i]= (shint)array[i]; }
}
vector<shint> vector_from_array(const int *array, int length) {
    vector<shint> vec;
    for (int i= 0; i < length; ++i) { vec.push_back((shint)array[i]); }
    return vec;
}
vector<shint> vector_from_string(const string &str) {
    vector<shint> vec;
    for (int i= 0; i < str.length(); ++i) { vec.push_back((shint)((shint)str[i] - (shint)'A')); }
    return vec;
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

BombeUnit::BombeUnit(const vector<Rotor> rotors, const Reflector reflector,
                     const bool use_configuration_grid /*true*/) :
    m_use_configuration_grid{use_configuration_grid} {
    // Top constructor
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
    // find all rotor positions, and make configuration grid
    if (m_use_configuration_grid) { m_configuration_grid= new ConfigurationGrid(*m_enigma); }
}
BombeUnit::BombeUnit(const std::initializer_list<Rotor> rotors, const Reflector reflector,
                     const bool use_configuration_grid /*true*/) :
    BombeUnit(vector<Rotor>(rotors), reflector, use_configuration_grid) {}
BombeUnit::BombeUnit(struct EnigmaSetting enigma_setting,
                     const bool           use_configuration_grid /*true*/) :
    BombeUnit(enigma_setting.rotors, enigma_setting.reflector, use_configuration_grid) {
    // plugboard, rotor position are ignored
    m_setting.starting_ring_setting   = enigma_setting.ring_setting;
    m_setting.starting_rotor_positions= enigma_setting.rotor_position;
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
void BombeUnit::set_identifier(string identifier) { m_identifier= identifier; }
// getters
struct BombeUnitSetting &BombeUnit::get_setting() {
    return m_setting;
}
string BombeUnit::get_identifier() const { return m_identifier; }
// other

void BombeUnit::init_enigma_encryptions(int encryptions, vector<string> &rotor_positions,
                                        vector<vector<shint>> &positions) {
    for (int *encryption : m_enigma_encryptions) { delete[] encryption; }
    m_enigma_encryptions.clear();
    rotor_positions.clear();
    rotor_positions.push_back(m_enigma->get_rotor_position_as_string());
    positions.clear();
    positions.push_back(vector_from_array(m_enigma->get_positions(), m_rotor_count));
    for (int i= 0; i < encryptions; ++i) {
        m_enigma->turn();   // enigma turns before encryption
        m_enigma_encryptions.push_back(m_enigma->get_encryption());
        rotor_positions.push_back(m_enigma->get_rotor_position_as_string());
        positions.push_back(vector_from_array(m_enigma->get_positions(), m_rotor_count));
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

vector<struct EnigmaSetting> BombeUnit::analyze(const string &ciphertext, const string &crib,
                                                int most_wired_letter, int position) {
    vector<struct EnigmaSetting> solutions;
    int total_permutations= m_enigma->compute_total_permutations_brute_force(),
        crib_n            = crib.length(),   // ciphertext_n= ciphertext.length();
        ring_settings     = min((int)pow(m_letters, m_rotor_count), m_setting.max_ring_settings);
    // cout << "Enigma starting from " << m_enigma->get_rotor_position_as_string() << "\n";

    vector<string>        rotor_positions;
    vector<vector<shint>> positions;
    if (m_use_configuration_grid) { m_configuration_grid->set_crib_length(crib_n); }
    m_enigma->set_ring_setting(m_setting.starting_ring_setting);
    m_enigma->set_rotor_position(m_setting.starting_rotor_positions);
    // for each ring setting
    auto start_ring_setting= std::chrono::system_clock::now();
    for (int rs= 0; rs < ring_settings; ++rs) {
        // cin.get();
        // print_progress(rs, ring_settings, (int)solutions.size());
        init_enigma_encryptions(crib_n, rotor_positions, positions);
        if (m_setting.time_performance) { start_ring_setting= std::chrono::system_clock::now(); }
        // for each rotor position
        for (int j= 0; j < total_permutations - 1; j++) {
            print_progress(rs, ring_settings, (int)solutions.size());
            cout << " RP:" << m_enigma->get_rotor_position_as_string();
            cin.get();
            // vector_from_array_inplace(rotor_position_at_start_of_crib, , m_rotor_count);
            // cout << "   RP:" << m_enigma->get_rotor_position_as_string() << "\n";
            //     << "  letters:" << m_letters << "   perms:" << total_permutations << "\n";
            // cout << flush;
            if (!m_use_configuration_grid ||
                (m_use_configuration_grid &&
                 !m_configuration_grid->get_checked(
                     m_enigma->get_ring_setting(),
                     vector_from_string(rotor_positions[positions.size() - crib_n - 1])))) {
                reset_diagonal_board();
                setup_diagonal_board(ciphertext, crib);
                // BEGIN TESTS
                if (check_one_wire(most_wired_letter)) {     // first test
                    if (doublecheck_and_get_plugboard()) {   // second test
                        if (tripplecheck(crib, ciphertext, position,
                                         rotor_positions)) {   // final test
                            /*cout << "\nRS:" << m_enigma->get_ring_setting_as_string() << "   RP:"
                                 << rotor_positions[rotor_positions.size() - crib.length() - 1]
                                 << "   P:" << m_enigma->get_cartridge()->get_positions_as_string()
                                 << "\n";*/
                            m_enigma->set_rotor_position(rotor_positions[0]);
                            solutions.push_back(m_enigma->get_setting());
                            m_enigma->set_rotor_position(rotor_positions.back());
                            if (m_setting.stop_on_first_valid) { return solutions; }
                        }
                        m_enigma->set_plugboard("");   // reset plugboard
                    }
                }
                if (m_use_configuration_grid) {
                    m_configuration_grid->set_checked(m_enigma->get_ring_setting(),
                                                      positions.begin() + positions.size() -
                                                          crib.length() - 1);
                }
            }

            // END TESTS
            // shuffle arrays TODO make own function
            m_enigma->turn();
            int *encryption= m_enigma_encryptions.front();
            m_enigma->get_encryption_inplace(encryption);
            m_enigma_encryptions.erase(m_enigma_encryptions.begin());
            m_enigma_encryptions.push_back(encryption);
            if ((int)rotor_positions.size() > crib_n + position) {
                rotor_positions.erase(rotor_positions.begin());
                positions.erase(positions.begin());
            }
            rotor_positions.push_back(m_enigma->get_rotor_position_as_string());
            positions.push_back(vector_from_array(m_enigma->get_positions(), m_rotor_count));
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
    cout << "                                                                                      "
            "      \r";
    if (m_setting.time_performance && m_verbose) { print_performance(); }
    // cout << "done";
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
    m_diagonal_board->activate(most_wired_letter, min(4, m_letters - 1));
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

    if (m_setting.debug_doublecheck_show_wiring) {
        cout << "DEBUG: RS" << m_enigma->get_ring_setting_as_string() << ", RP "
             << m_enigma->get_rotor_position_as_string();
        m_diagonal_board->print();
        m_diagonal_board->print_live();
        m_diagonal_board->print_connections();
        cin.get();
    }

    for (int bundle= 0; bundle < m_letters; ++bundle) {
        int sum= m_diagonal_board->bundle_sum(bundle);
        if (sum == 1) {   // steckered is live
            // all other bundles should have 1 or less live wires
            for (int bundle_2= 0; bundle_2 < m_letters; ++bundle_2) {
                if (m_diagonal_board->bundle_sum(bundle_2) > 1) {
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
        } else {   // undeterminable... try some more! TODO
            // cout << bundle << "OH NO!\n";
        }
    }
    return true;
}
bool BombeUnit::tripplecheck(const string &crib, const string &ciphertext, int candidate,
                             vector<string> &rotor_positions) {
    // test if the given configuration encrypts the crib to plaintext
    // turn back
    m_enigma->set_rotor_position(rotor_positions[rotor_positions.size() - crib.length() - 1]);
    /*cout << "enigma recrypting " << ciphertext << " with RS "
         << m_enigma->get_ring_setting_as_string() << ", RP "
         << m_enigma->get_rotor_position_as_string() << " PLUGBOARD";
    m_enigma->get_cartridge()->get_plugboard()->print();*/
    string recrypt= m_enigma->encrypt(ciphertext);
    // cout << "(" << recrypt << ")";
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
void BombeUnit::print_progress(int ring_setting, int max_ring_settings, int solutions_n) {
    cout << "\r";
    string progress_bar= "[";
    for (int i= 0; i < PROGRESS_BAR_WIDTH; ++i) {
        progress_bar+=
            i <= (PROGRESS_BAR_WIDTH * (ring_setting + 1)) / max_ring_settings ? "#" : " ";
    }
    progress_bar+= "]";
    cout << progress_bar;
    if (m_use_configuration_grid) {
        cout << " [exhausted "
             << (100 * (float)m_configuration_grid->get_checked_configurations()) /
                    (float)m_configuration_grid->get_total_configurations()
             << "]";
    }

    cout << " [" << m_identifier << "] ";
    cout << "RS: " << m_enigma->get_ring_setting_as_string() << ", solutions: " << solutions_n
         << flush;
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

Bombe::Bombe(const initializer_list<Rotor> rotors, const Reflector reflector,
             const bool use_configuration_grid) :
    m_rotors{vector<Rotor>(rotors)},
    m_reflector{vector<Reflector>({reflector})}, m_use_configuration_grid{use_configuration_grid} {
    m_letters= m_rotors[0].get_wires();
    // streambuf *streambuffer= cout.rdbuf();
    // m_outstream            = ostream(streambuffer);
}

Bombe::Bombe(vector<Rotor> rotors, vector<Reflector> reflector, const bool use_configuration_grid) :
    m_rotors(rotors), m_reflector(reflector), m_use_configuration_grid{use_configuration_grid} {
    m_letters= m_rotors[0].get_wires();
}

vector<int> Bombe::probable_search(const string &ciphertext, const string &crib) {
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
vector<struct EnigmaSetting> Bombe::analyze_unit(const string & ciphertext_substring,
                                                 const string & crib,
                                                 vector<Rotor> &rotor_configuration,
                                                 Reflector &reflector, int candidate,
                                                 int most_wired_letter) {
    BombeUnit unit(rotor_configuration, reflector, m_use_configuration_grid);
    auto      start_unit_run= std::chrono::system_clock::now();
    unit.set_identifier(unit.get_identifier() + " position " + to_string(candidate) + " ");
    // translate settings
    unit.get_setting().performance_ring_setting_mean= m_setting.performance_ring_setting_mean;
    unit.get_setting().performance_ring_setting_var = m_setting.performance_ring_setting_var;
    unit.get_setting().records_ring_setting         = m_setting.records_ring_setting;
    unit.get_setting().only_one_candidate           = m_setting.only_one_candidate;
    unit.get_setting().max_ring_settings            = m_setting.max_ring_settings;
    unit.get_setting().starting_ring_setting        = m_setting.starting_ring_setting;
    unit.get_setting().starting_rotor_positions     = m_setting.starting_rotor_positions;
    unit.get_setting().only_one_candidate           = m_setting.only_one_candidate;
    unit.get_setting().stop_on_first_valid          = m_setting.stop_on_first_valid;
    if (m_setting.time_performance) { start_unit_run= std::chrono::system_clock::now(); }
    vector<struct EnigmaSetting> solutions=
        unit.analyze(ciphertext_substring, crib, most_wired_letter, candidate);
    if (m_setting.time_performance) {
        auto stop_unit_run= std::chrono::system_clock::now();
        update_performance(m_setting.performance_unit_run_mean, m_setting.performance_unit_run_var,
                           stop_unit_run - start_unit_run, m_setting.records_unit_run);
        // std::chrono::duration<double> measurement= (stop_unit_run - start_unit_run);
        // cout << " unit run: " << measurement.count();
    }

    // solutions.insert(solutions.end(), solutions_unit.begin(), solutions_unit.end());
    // update perofrmance timing
    m_setting.performance_ring_setting_mean= unit.get_setting().performance_ring_setting_mean;
    m_setting.performance_ring_setting_var = unit.get_setting().performance_ring_setting_var;
    m_setting.records_ring_setting         = unit.get_setting().records_ring_setting;
    int tid                                = omp_get_thread_num();
    /*cout << "\n"
         << "thread " << tid << " finished " << unit.get_identifier() << " found "
         << solutions.size() << " solutions"
         << "\n";*/
    return solutions;
}
int Bombe::find_most_wired_letter(const string &ciphertext, const string &crib) {
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
/*Bombe::Bombe(struct EnigmaSetting enigma_setting) {
    m_unit_setting= enigma_setting;   // XXX shallow copy might not be appropriate
}*/

/*vector<vector<Rotor>> Bombe::get_rotor_configurations(vector<Rotor> in) {
    return choices_of<Rotor>(m_rotors);
}*/

vector<struct EnigmaSetting> Bombe::analyze(const string &ciphertext, const string &crib) {
    vector<struct EnigmaSetting> solutions;
    // find candidates
    vector<int> candidates= probable_search(ciphertext, crib);
    if (candidates.size() == 0) { return solutions; }   // no candidates
    // make units with different rotor orders, and make them analyze
    vector<vector<Rotor>> rotor_configurations= choices_of(m_rotors, m_setting.rotor_count);
    if (m_setting.only_one_configuration) {
        rotor_configurations.erase(rotor_configurations.begin() + 1, rotor_configurations.end());
    }

    // spawn units that analyze. For each candidate, for each configuration of rotors
    // unsaefe!!!
    vector<Rotor> rotor_configuration;
    unsigned int  i, j, k;
    //#pragma omp parallel for collapse(3) private(rotor_configuration, i, j, k)
    for (i= 0; i < candidates.size(); ++i) {
        for (k= 0; k < m_reflector.size(); ++k) {
            for (j= 0; j < rotor_configurations.size(); ++j) {
                // cout << i << j << k << "\n";
                int    candidate           = candidates[i];
                string ciphertext_substring= ciphertext.substr(candidate, crib.length());
                int    most_wired_letter   = find_most_wired_letter(ciphertext_substring, crib);
                // TODO common
                vector<Rotor> rotor_configuration= rotor_configurations[j];
                // TODO thread it
                vector<struct EnigmaSetting> solutions_unit=
                    analyze_unit(ciphertext_substring, crib, rotor_configuration, m_reflector[k],
                                 candidate, most_wired_letter);
                for (struct EnigmaSetting solution : solutions_unit) {
                    solutions.push_back(solution);
                }
                if (m_setting.stop_on_first_valid && solutions.size() > 0) { return solutions; }
            }
        }
    }
    return solutions;
}

struct BombeSetting &Bombe::get_setting() {
    return m_setting;
}

string Bombe::preprocess(string in) const {
    // remove everything that is not an alpha
    std::regex nonalpha("[^a-zA-Z]");
    in= regex_replace(in, nonalpha, "");
    // convert string to upper case
    std::for_each(in.begin(), in.end(), [](char &c) { c= ::toupper(c); });
    return in;
}

// void BombeUnit::get_equivalent_settings(vector<int> position) {
// posiiton: physical position of rotors, ie rotor_position-ring_setting

/*If a message of length m is encrypted with a particular ring setting and rotor posiiton,
then encrypting the same message with rotor position turned one step forward and ring setting
one step "backward"(compensating for turn of rotor) should give the exact same message unless
there was a notch turnover when encrypting. That is, there are a lot of settings of the enigma
that would encrypt to the exact same message.*/

/*
The algorithm;
initially all configurations where rotor and ring setting gives the same position are candidates
from here we have to eliminate the rest. This is done by turning the enigma, and checking if the
position is the same as for the original when turned
*/

// for each rotorp position, there is exactly one ring setting that gives the sa,,e position as
// the original posiiton
/*string ring_setting;
for (vector<shint> rotor_position : m_all_rotor_positions) {
    // there is exactly one ring setting so that position is the origianl position
    ring_setting= "";
    for (int i= 0; i < position.size(); ++i) {
        ring_setting+=
            (char)((rotor_position[i] - position[i] + m_letters) % m_letters + (int)'A');
    }
    // now test if it holds
}
}*/

ConfigurationGrid::ConfigurationGrid(Enigma &enigma) {
    m_letters            = enigma.get_wires();
    m_rotor_count        = enigma.get_rotors();
    m_all_rotor_positions= enigma.get_all_rotor_positions();
    try {
        cout << "\rAllocating to inverse ";
        // larger than it needs to be...only m_all_rotor_poositions.size()<<letter^rotors
        m_all_rotor_positions_inverse= vector<unsigned int>(pow(m_letters, m_rotor_count), 0);
    } catch (bad_alloc &ba) {
        cerr << "ERROR:Failed to allocate inverse of all rotor positions, disable configuration "
                "grid";
    }
    cout << "DONE\n";
    try {
        cout << "\rAllocating checked, "
             << m_all_rotor_positions.size() * pow(m_letters, m_rotor_count) << " bits="
             << ((m_all_rotor_positions.size() * pow(m_letters, m_rotor_count)) / (8 * 125000000))
             << " Gb\n";
        m_checked=
            vector<bool>(m_all_rotor_positions.size() * pow(m_letters, m_rotor_count), false);
        cout << "got " << m_checked.size() << " asked for "
             << m_all_rotor_positions.size() * pow(m_letters, m_rotor_count) << "\n";
    } catch (bad_alloc &ba) {
        cerr << "ERROR:Failed to allocate checklist in configuration grid, disable "
                "configuration "
                "grid";
        exit(EXIT_FAILURE);
    }
    cout << "DONE\n";
    for (unsigned int rp= 0; rp < m_all_rotor_positions.size(); ++rp) {
        cout << "\rSetting to inverse, " << rp << "/" << m_all_rotor_positions.size() << "     "
             << flush;
        vector<shint> rotor_position= m_all_rotor_positions[rp];
        m_all_rotor_positions_inverse[vector_to_int_hash(rotor_position)]= rp;
    }
    cout << "DONE\n";
    cout << "SUCCESFULLY MADE CONFIGURATION GRID\n";
    m_total_configurations= pow(m_letters, m_rotor_count) * m_all_rotor_positions.size();
}

void ConfigurationGrid::reset_checked() {
    for (int rs= 0; rs < pow(m_letters, m_rotor_count); ++rs) {
        for (unsigned int rp= 0; rp < m_all_rotor_positions.size(); ++rp) {
            m_checked[rs * m_all_rotor_positions.size() + rp]= false;
        }
    }
}

bool ConfigurationGrid::get_checked(const int *ring_setting, const vector<int> &rotor_position) {
    // translate to index
    // cout << "get" << flush;
    // int rs= ring_setting_string_to_int(ring_setting),
    //    rp= rotor_position_string_to_int(rotor_position);
    int rs= ring_setting_array_to_int(ring_setting),
        rp= rotor_position_vector_to_int(rotor_position);
    /*cout << "\ntrying to get [" << rs << "," << rp << "]=["
         << rs * m_all_rotor_positions.size() + rp << "], size=" << m_checked.size() << "\n"
         << flush;*/
    bool out= m_checked[rs * m_all_rotor_positions.size() + rp];
    // cout << "got" << flush;
    return out;
}

const std::string wc("\033[0;31m");
const std::string rc("\033[0;33m");
const std::string gc("\033[1;30m");
const std::string bgc("\033[0;40m");
const std::string bgr("\033[0;40m");

void ConfigurationGrid::set_checked(const int *                           ring_setting,
                                    vector<vector<shint>>::const_iterator positions_original) {
    // translate to index
    // cout << "CONF::set_checked:    get rs, rp ";
    // int rs= ring_setting_array_to_int(ring_setting);
    /*cout << "computing rotor pos ";
    for (int i= 0; i < m_rotor_count; ++i) {
        cout << (char)((positions_original[0][i] - ring_setting[i] + m_letters) % m_letters +
                       (int)'A');
    }
    cin.get();*/
    // int    rp= rotor_position_array_to_int(rotor_position);
    // get position and all subsequent
    /*for (int i= 0; i < m_rotor_count; ++i) {
        for (int p= 0; p < m_crib_length;
             ++p) {   // offset from start                           // index of rotor_position
            printf("%2d ", positions_original[p][i]);
        }
        cout << "\n";
    }
    cout << "\n";*/
    //        /*int position_p_i= (m_all_rotor_positions[(rp + p) %
    //        m_all_rotor_positions.size()][i]
    //        -
    //                           ring_setting[i] + m_letters) %
    //                          m_letters;*/
    //        int position_p_i= (m_all_rotor_positions[(rp + p) %
    //        m_all_rotor_positions.size()][i] -
    //                           ring_setting[i] + m_letters) %
    //                          m_letters;
    //        positions_original[p][i]= position_p_i;
    //    }
    //}
    // get equivalents, one candidae per rotor position.
    // cout << "-";
    int set_count= 0;
    int rss      = 0;
    for (unsigned int rpp= 0; rpp < m_all_rotor_positions.size(); ++rpp) {

        // vector<shint> position;
        // find the ring setting corresponding to this rotor position(rs so that
        // rp-rs=r_original[0])
        for (int i= 0; i < m_rotor_count; ++i) {
            current_ring_setting[i]= ((m_all_rotor_positions[rpp][i] -
                                       positions_original[0][m_rotor_count - i - 1] + m_letters) %
                                      m_letters);
        }
        rss= vector_to_int_hash(current_ring_setting);
        // cout << "rss: " << rss << "\n";
        //    cout << "rotor pos: ";
        /*for (int i= 0; i < m_rotor_count; ++i) {
            cout << (char)(m_all_rotor_positions[rpp][i] + (int)'A');
        }
        cout << "\n";
        cout << "ring set: ";
        for (int i= 0; i < m_rotor_count; ++i) {
            cout << (char)(current_ring_setting[i] + (int)'A');
        }
        cin.get();*/
        if (!m_checked[rss * m_all_rotor_positions.size() + rpp]) {   // if not checked
            // now we can search from rss, rpp, along rpp axis and compare to original
            // p=0 isalready good
            bool equal= true;
            for (int p= m_crib_length - 1; p > 0; --p) {
                for (int i= 0; i < m_rotor_count; ++i) {
                    // cout << wc;
                    int position_p_i=
                        (m_all_rotor_positions[(rpp + p) % m_all_rotor_positions.size()][i] -
                         current_ring_setting[i] + m_letters) %
                        m_letters;
                    // printf("%2d ", position_p_i);
                    if (position_p_i != positions_original[p][m_rotor_count - i - 1]) {
                        equal= false;
                        break;
                    }
                }
                // cout << "\n";

                if (!equal) break;
            }
            if (equal) {
                if (m_checked[rss * m_all_rotor_positions.size() + rpp]) {
                    cout << "WARNING: checked[" << rss << "," << rpp
                         << "] already set, this should not be possible";
                }
                m_checked[rss * m_all_rotor_positions.size() + rpp]= true;
                set_count++;
            }
            // cout << "\n" << rc;
            // cin.get();
        }
    }
    // cout << "-";
    m_checked_configurations+= set_count;
    cout << "set " << set_count << flush << "\n";
    return;
}

void ConfigurationGrid::set_crib_length(int crib_length) {
    m_crib_length= crib_length;
    // now we can properly allocate positions_original and current ring setting
    for (int p= 0; p < crib_length; ++p) {   // offset from start
        positions_original.push_back(vector<shint>(m_rotor_count, 0));
    }
    current_ring_setting= vector<shint>(m_rotor_count, 0);
}

long int ConfigurationGrid::get_total_configurations() const { return m_total_configurations; }
long int ConfigurationGrid::get_checked_configurations() const { return m_checked_configurations; }
unsigned int ConfigurationGrid::rotor_position_string_to_int(const string &rotor_position) {
    // a injective mapping from ring setting strings to int. Essentially a hash
    // must map correctly to m_all_rotor_positions
    // that is,
    // m_all_rotor_positions[rotor_position_string_to_int[rotor_position]]=rotor_position
    // here we use the array m_all_rotor_positions_inverse, but first we have make an index
    // from the string, to look up in the inverse array
    return m_all_rotor_positions_inverse[string_to_int_hash(rotor_position)];
}
unsigned int ConfigurationGrid::ring_setting_array_to_int(const int *ring_setting) {
    // a injective mapping from ring setting strings to int. Essentially a hash
    return array_to_int_hash(ring_setting);
}
unsigned int ConfigurationGrid::rotor_position_array_to_int(const int *rotor_position) {
    // a injective mapping from ring setting strings to int. Essentially a hash
    // must map correctly to m_all_rotor_positions
    // that is,
    // m_all_rotor_positions[rotor_position_string_to_int[rotor_position]]=rotor_position
    // here we use the array m_all_rotor_positions_inverse, but first we have make an index
    // from the string, to look up in the inverse array
    return m_all_rotor_positions_inverse[array_to_int_hash(rotor_position)];
}
unsigned int ConfigurationGrid::rotor_position_vector_to_int(const vector<int> &rotor_position) {
    // a injective mapping from ring setting strings to int. Essentially a hash
    // must map correctly to m_all_rotor_positions
    // that is,
    // m_all_rotor_positions[rotor_position_string_to_int[rotor_position]]=rotor_position
    // here we use the array m_all_rotor_positions_inverse, but first we have make an index
    // from the string, to look up in the inverse array
    return m_all_rotor_positions_inverse[vector_to_int_hash(rotor_position)];
}
unsigned int ConfigurationGrid::ring_setting_string_to_int(const string &ring_setting) {
    // a injective mapping from ring setting strings to int. Essentially a hash
    return string_to_int_hash(ring_setting);
}
unsigned int ConfigurationGrid::string_to_int_hash(const string &str) {
    unsigned int as_int= 0;
    for (int i= m_rotor_count - 1; i >= 0; --i) {   // TODO reverse iterator
        as_int*= m_letters;
        as_int+= (char)(str[i] - (int)'A');
    }
    return as_int;
}
unsigned int ConfigurationGrid::vector_to_int_hash(const vector<shint> &vec) {
    unsigned int as_int= 0;
    for (int i= m_rotor_count - 1; i >= 0; --i) {   // TODO reverse iterator
        as_int*= m_letters;
        as_int+= vec[i];
    }
    return as_int;
}
unsigned int ConfigurationGrid::array_to_int_hash(const int *array) {
    unsigned int as_int= 0;
    for (int i= m_rotor_count - 1; i >= 0; --i) {   // TODO reverse iterator
        as_int*= m_letters;
        as_int+= array[i];
    }
    return as_int;
}
