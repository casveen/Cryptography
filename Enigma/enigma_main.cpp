// main of enigma
#include "enigma.h"
#include <boost/program_options.hpp>

namespace po= boost::program_options;

int main(int argc, char *argv[]) {
    /* TODO:
    flags for encoding a file, (make case -insesnsitive)

    ringsetting: -rs/ -ringsetting / -ringsthellung /-s: string, might be
    numbers spaced with " " or "." rotorposition: -rp/ -rotorposition /-p:

    string: might be numbers spaced with " " or "." input: -p/-plain/-plaintext
           -i/-input :string
    inputfile: -f/-file/-inputfile: string

    wheels: -w/-wheels/-r/-rotors: string of names

    plugboard: -plug/-plugging/-plugboard/ -pb: string of pairs isolated with .
    or " "

    reflector: -ref/-reflector: might be provided, same type as plugboard string

    WARNINGS: providing ref in -wheels and -ref.
             ref in wrong place but also one at correct place


    ERROR: invalid -ref, invalid -plug, invalid wheel in wheel, no ref at end



     */

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()("help, h", "produce help message");
    desc.add_options()("ringsetting, rs", po::value<string>(),
                       "set ring setting");
    desc.add_options()("rotorposition, rp", po::value<string>(),
                       "set rotor positions");
    desc.add_options()("inputfile, i", po::value<string>(),
                       "set file to encrypt");
    desc.add_options()("r", po::value<string>(), "set rotors that are used");
    desc.add_options()("plugboard, p, plug", po::value<string>(),
                       "set plugboard wiring");
    desc.add_options()("plaintext, m", po::value<string>(),
                       "string that is to be encrypted");
    desc.add_options()("reflector, r", po::value<string>(),
                       "set reflector wiring");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("rotors")) {
        cout << "Compression level was set to " << vm["compression"].as<int>()
             << ".\n";
    } else {
        cout << "Rotors not set, please provide rotors, or enter the number of "
                "randomly made rotors you want to use.\n";
        // TODO the above
    }
}
