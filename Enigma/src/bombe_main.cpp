// main of enigma
//#include "enigma.hpp"
#include "bombe.hpp"
#include "rotors.cpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

vector<string> splitbywords (string in) {
    std::regex word ("[[:alpha:]]+");
    smatch match;
    regex_match (in, match, word);

    auto words_begin = std::sregex_iterator (in.begin (), in.end (), word);
    auto words_end = std::sregex_iterator ();

    std::vector<std::string> out;
    // Create vector to hold our words
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str ();
        out.push_back (match_str);
        // std::cout << match_str << '\n';
    }
    return out;
}

int main (int argc, char *argv[]) {
    bool quiet = false, verbose = false;

    // make description of all rotors
    string ALLROTORDESC = "NAME   DESCRIPTION \n";
    for (auto map : { COMMERCIALROTORMAP, ROCKETROTORMAP, SWISSKROTORMAP, KRIEGSMARINEROTORMAP, WEHRMACHTROTORMAP }) {
        for (auto it = map.begin (); it != map.end (); ++it) {
            int name_length = (it->first).length ();
            ALLROTORDESC += it->first + ":";
            ALLROTORDESC.append (max (6 - name_length, 0), ' ');
            ALLROTORDESC += it->second.second + "\n";
        }
    }
    // make descriptoin of all reflectors
    string ALLREFLECTORDESC = "NAME           DESCRIPTION \n";
    for (auto it = ALLREFLECTORMAP.begin (); it != ALLREFLECTORMAP.end (); ++it) {
        int name_length = (it->first).length ();
        ALLREFLECTORDESC += it->first + ":";
        ALLREFLECTORDESC.append (max (14 - name_length, 0), ' ');
        ALLREFLECTORDESC += it->second.second + "\n";
    }

    // Declare the supported options.
    po::options_description desc ("Allowed options");
    desc.add_options () ("help, h", "produce help message");
    desc.add_options () ("inputfile, i", po::value<string> (), "set file to run bombe on");
    desc.add_options () ("rotors", po::value<string> (), " set of rotors to choose from");
    desc.add_options () ("rotor_count", po::value<int> (), "suspected amount of rotors used ");
    desc.add_options () ("ciphertext, m", po::value<string> (), "string to run bombe on");
    desc.add_options () ("crib, c", po::value<string> (), "crib(s) to use, separated by commas and no spaces");
    desc.add_options () ("reflector, r", po::value<string> (), "suspected reflector(s)");
    desc.add_options () ("o", po::value<string> (), " file to write output to. If none provided displays solutions to terminal");
    desc.add_options () ("q", po::bool_switch (&quiet), " set quiet");
    desc.add_options () ("v", po::bool_switch (&verbose), " set verbose");

    po::variables_map vm;
    po::store (po::parse_command_line (argc, argv, desc), vm);
    po::notify (vm);

    int rotors_num;
    vector<Rotor> rotors;
    int reflectors_num;
    vector<Reflector> reflectors;

    // HELP
    if (vm.count ("help")) {
        cout << "The Bombe is used to find the configuration of an enigma that made a ciphertext. "
                "The whole procedure depends on having a crib - an unencrypted portion of the "
                "ciphertext.\n Given a ciphertext, a proper crib together with a set of wheels that "
                "you suspect was used in the encryption, the program will output a "
                "enigma-configuration that will encrypt the crib to the corresponding part of the "
                "ciphertext, hopefully returning a correct decryption of the ciphertext in the "
                "process.\n";
        cout << desc << "\n";
        return 1;
    }

    // VERBOSE AND QUIET?
    /*if (vm.count("v") && vm.count("q")) {
        cerr << "ERROR: cannot be both verbose and quiet simultanously\n";
        return 1;
    }*/

    // ROTORS
    if (vm.count ("rotors")) {
        vector<string> rotor_names = splitbywords (vm["rotors"].as<string> ());
        rotors_num = (int)rotor_names.size ();
        // rotor names to rotors
        for (int r = 0; r < rotors_num; ++r) {
            string rotor_name = rotor_names[r];
            if (ALLROTORMAP.count (rotor_name)) {
                rotors.push_back (ALLROTORMAP.at (rotor_name).first);
            } else { // wrong name
                cerr << "ERROR: Rotor name " << rotor_name
                     << " is wrong or not implemented yet. Use the following "
                        "rotor names: \n"
                     << ALLROTORDESC;
                return 1;
            }
        }
    } else { // no rotors
        cerr << "ERROR: No rotors provided. Use the following rotor names "
                "separated by comma(no spaces): \n"
             << ALLROTORDESC << "\n";
        return 1;
    }

    // REFLECTOR
    if (vm.count ("reflector")) {
        vector<string> reflector_names = splitbywords (vm["reflector"].as<string> ());
        reflectors_num = (int)reflector_names.size ();
        // reflector names to reflectors
        for (int r = 0; r < reflectors_num; ++r) {
            string reflector_name = reflector_names[r];
            if (ALLREFLECTORMAP.count (reflector_name)) {
                reflectors.push_back (ALLREFLECTORMAP.at (reflector_name).first);
            } else { // wrong name
                cerr << "ERROR: Reflector name " << reflector_name
                     << " is wrong or not implemented yet. Use the following "
                        "reflector names: \n"
                     << ALLREFLECTORDESC;
                return 1;
            }
        }
    } else { // no reflectors
        cerr << "ERROR: No reflectors provided. Use the following reflector names "
                "separated by comma(no spaces): \n"
             << ALLREFLECTORDESC << "\n";
        return 1;
    }

    // INPUT AND FILENAME?
    if (vm.count ("ciphertext") && vm.count ("inputfile")) {
        cerr << "ERROR: Only one of ciphertext or filename should be provided\n";
        return 1;
    }
    // NEITHER INPUT OR FILENAME?
    if (!vm.count ("ciphertext") && !vm.count ("inputfile")) {
        cerr << "ERROR: provde a ciphertext or filename\n";
        return 1;
    }
    // NO CRIB
    if (!vm.count ("crib")) {
        cerr << "ERROR: crib must be provided\n";
        return 1;
    }
    // NO CRIB
    if (vm.count ("rotor_count")) {
        rotors_num = vm["rotor_count"].as<int> ();
    } else {
        cout << "WARNING: suspected rotor count not provided, assuming all given rotors in use\n";
    }

    cout << "before making bombe\n";
    //
    string as = "";
    for (int i = 0; i < rotors_num; ++i) {
        as += "A";
    }

    // make the bombe
    Bombe bombe (rotors, reflectors, (rotors_num < 4));
    bombe.get_setting ().rotor_count = rotors_num;
    bombe.get_setting ().starting_ring_setting = as;
    bombe.get_setting ().starting_rotor_positions = as;
    cout << "made bombe\n";
    /*
    handle outstream
    ofstream   outfilestream;
    streambuf *streambuffer= cout.rdbuf();
    if (vm.count("o")) {
        outfilestream.open(vm["o"].as<string>());
        streambuffer= outfilestream.rdbuf();
    }
    ostream outstream(streambuffer);
    bombe.set_outstream(outstream);
    */

    // assemble ciphertext into a single string
    string ciphertext = "";

    if (vm.count ("ciphertext")) {
        ciphertext = vm["ciphertext"].as<string> ();
    }

    if (vm.count ("inputfile")) {
        string line;
        ifstream instream (vm["inputfile"].as<string> ());
        if (instream.fail ()) {
            cout << "ERROR: failed to open " << vm["inputfile"].as<string> () << "\n";
            return 1;
        }
        while (getline (instream, line)) {
            ciphertext += line;
        }
        instream.close ();
    }
    // analyze
    cout << "before  analyze\n";
    bombe.analyze (ciphertext, splitbywords (vm["crib"].as<string> ()));
    // outstream.close();
    return 0;
}

/*


*/
