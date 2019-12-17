// main of enigma
#include "enigma.hpp"
#include "rotors.cpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

/*split sentence of from <>. <>. <>. ... <>. into vector of <>, <>, <>, ...*/
/*ignores whitespace, needs . separation */
/*vector<string> splitbycomma(string in) {

    std::regex separator("(,|[[:space:]])");
    std::regex word("[[:alpha:]]+");

    std::cout << in << "\n";

    std::vector<std::string> out;   // Create vector to hold our words
    // replace separator with .
    cout << "made out\n";
    string str_dot_separated=
        regex_replace(in, separator, ".", regex_constants::format_default);
    cout << str_dot_separated;

    return out;
}*/

vector<string> splitbywords(string in) {
  std::regex word("[[:alpha:]]+");
  smatch match;
  regex_match(in, match, word);

  auto words_begin = std::sregex_iterator(in.begin(), in.end(), word);
  auto words_end = std::sregex_iterator();

  std::vector<std::string> out;
  // Create vector to hold our words
  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    std::string match_str = match.str();
    out.push_back(match_str);
    // std::cout << match_str << '\n';
  }
  return out;
}

int main(int argc, char *argv[]) {
  bool quiet = false, verbose = false;

  /* TODO:
  flags for encoding a file, (make case -insesnsitive)
  flags for what to do with spaces, comma and punctuation

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

  // make description of all rotors
  string ALLROTORDESC = "NAME   DESCRIPTION \n";
  for (auto map : {COMMERCIALROTORMAP, ROCKETROTORMAP, SWISSKROTORMAP,
                   KRIEGSMARINEROTORMAP, WEHRMACHTROTORMAP}) {
    for (auto it = map.begin(); it != map.end(); ++it) {
      int name_length = (it->first).length();
      ALLROTORDESC += it->first + ":";
      ALLROTORDESC.append(max(6 - name_length, 0), ' ');
      ALLROTORDESC += it->second.second + "\n";
    }
  }
  // make descriptoin of all reflectors
  string ALLREFLECTORDESC = "NAME           DESCRIPTION \n";
  for (auto it = ALLREFLECTORMAP.begin(); it != ALLREFLECTORMAP.end(); ++it) {
    int name_length = (it->first).length();
    ALLREFLECTORDESC += it->first + ":";
    ALLREFLECTORDESC.append(max(14 - name_length, 0), ' ');
    ALLREFLECTORDESC += it->second.second + "\n";
  }

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help, h", "produce help message");
  desc.add_options()(
      "ringsetting, rs", po::value<string>(),
      "set ring setting.\nUSAGE: string of #rotors capital letters");
  desc.add_options()("rotorposition, rp", po::value<string>(),
                     "set rotor positions");
  desc.add_options()("inputfile, i", po::value<string>(),
                     "set file to encrypt");
  desc.add_options()("rotors", po::value<string>(),
                     "set rotors that are used, from last to first");
  desc.add_options()("plugboard, p, plug", po::value<string>(),
                     "set plugboard wiring");
  desc.add_options()("plaintext, m", po::value<string>(),
                     "string that is to be encrypted");
  desc.add_options()("reflector, r", po::value<string>(),
                     "set reflector wiring");
  // TODO or wiring
  desc.add_options()("stator, s", po::value<string>(), "set stator");
  desc.add_options()("o", po::value<string>(),
                     " file to write output to. If none provided encrypts "
                     "into the terminal");
  desc.add_options()("q", po::bool_switch(&quiet), " set quiet");
  desc.add_options()("v", po::bool_switch(&verbose), " set verbose");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  int rotors_num;
  vector<Rotor> rotors;
  string rotor_position;
  string ring_setting;
  string plugging;
  Reflector *reflector;
  Rotor *stator;
  bool trivial_stator = true;

  // HELP
  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  // VERBOSE AND QUIET?
  /*if (vm.count("v") && vm.count("q")) {
      cerr << "ERROR: cannot be both verbose and quiet simultanously\n";
      return 1;
  }*/

  // ROTORS
  if (vm.count("rotors")) {
    vector<string> rotor_names = splitbywords(vm["rotors"].as<string>());
    rotors_num = (int)rotor_names.size();
    // rotor names to rotors
    for (int r = 0; r < rotors_num; ++r) {
      string rotor_name = rotor_names[r];
      if (ALLROTORMAP.count(rotor_name)) {
        rotors.push_back(ALLROTORMAP.at(rotor_name).first);
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
  if (vm.count("reflector")) {
    string reflector_name = vm["reflector"].as<string>();
    if (ALLREFLECTORMAP.count(reflector_name)) {
      reflector = new Reflector(ALLREFLECTORMAP.at(reflector_name).first);
    } else { // wrong name
      cerr << "ERROR: Reflector name " << reflector_name
           << " is wrong or not implemented yet. Use the following "
              "reflector names: \n"
           << ALLREFLECTORDESC;
      return 1;
    }
  } else { // no reflector
    cerr << "ERROR: No reflector provided. Use one of the following "
            "reflector names: \n"
         << ALLREFLECTORDESC << "\n ";
    return 1;
  }

  // STATOR
  if (vm.count("stator")) {
    string stator_name = vm["stator"].as<string>();
    trivial_stator = false;
    if (ALLROTORMAP.count(stator_name)) {
      stator = new Rotor(ALLROTORMAP.at(stator_name).first);
    } else { // wrong name
      cerr << "ERROR: stator name " << stator_name
           << " is wrong or not implemented yet. Use the following "
              "rotor names: \n"
           << ALLROTORDESC;
      return 1;
    }
  } else { // no reflector
    cout << "WARNING: No stator provided. Using identity stator\n";
  }

  // ROTOR POSITIONS
  if (vm.count("rotorposition")) {
    rotor_position = vm["rotorposition"].as<string>();
    cout << rotor_position;
  } else {
    rotor_position = "";
    rotor_position.append(rotors.size(), 'A');
    if (!quiet) {
      cout << "WARNING: rotor positions not provided, using " << rotor_position
           << "\n";
    }
  }

  // RING SETTING
  if (vm.count("ringsetting")) {
    ring_setting = vm["ringsetting"].as<string>();
  } else {
    ring_setting = "";
    ring_setting.append(rotors.size(), 'A');
    if (!quiet) {
      cout << "WARNING: ring setting not provided, using " << ring_setting
           << "\n";
    }
  }

  // PLUGBOARD
  if (vm.count("plugboard")) {
    plugging = vm["plugboard"].as<string>();
  } else {
    plugging = "";
    if (!quiet) {
      cout << "WARNING: plugboard setting not provided, not using "
              "plugboard \n";
    }
  }

  // INPUT AND FILENAME?
  if (vm.count("plaintext") && vm.count("inputfile")) {
    cerr << "ERROR: Only one of plaintext or filename should be provided\n";
    return 1;
  }
  // NEITHER INPUT OR FILENAME? TODO interactive mode
  if (!vm.count("plaintext") && !vm.count("inputfile")) {
    cerr << "ERROR: provde a plaintext or filename\n";
    return 1;
  }

  // MAKE THE ENIGMA AND CONFIGURE
  Plugboard *plugboard = new Plugboard(plugging, 26);
  struct EnigmaSetting setting;
  setting.rotors = rotors;
  setting.reflector = reflector;
  setting.plugboard = plugboard; // copied by enigma
  setting.ring_setting = ring_setting;
  setting.rotor_position = rotor_position;
  setting.stator = stator;
  setting.trivial_stator = trivial_stator;
  Enigma enigma(setting); // has random rotors and identity plugboard
  // enigma.set_setting(setting);

  // ENCRYPT
  ofstream outfilestream;
  streambuf *streambuffer = cout.rdbuf();
  if (vm.count("o")) {
    outfilestream.open(vm["o"].as<string>());
    streambuffer = outfilestream.rdbuf();
  }
  ostream outstream(streambuffer);

  if (vm.count("plaintext")) {
    outstream << enigma.encrypt(vm["plaintext"].as<string>()) << "\n";
  }

  if (vm.count("inputfile")) {
    ifstream instream(vm["inputfile"].as<string>());
    enigma.encrypt(instream, outstream);
  }
  outfilestream.close();
  delete reflector; // XXX double free of reflector?
  delete plugboard;
  return 0;
}

/*
EXAMPLES
WIKIPEDIA EXAMPLE
./enigma.exe --rotors VIII,VI,V,BETA --reflector THINREFLECTORC --plaintext
RBBFPMPHHGCZXTDYGAHGUFXGEWKBLKGJ --ringsetting LEPE --rotorposition JTDC
--plugboard AE.BF.CM.DQ.HU.JN.LX.PR.SZ.VW
works well

indicator of WIKIOEDIA EXAMPLE
./enigma.exe --rotors VIII,VI,V,BETA --reflector THINREFLECTORC --plaintext QEOB
--ringsetting LEPE --rotorposition MEAN --plugboard
AE.BF.CM.DQ.HU.JN.LX.PR.SZ.VW
              A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
              | | | | | | | | | | | | | | | | | | | | | | | | | |
m[  0] = Q -> J W Q H N O Y D K A I V P E F M[C]X U Z S L B R G T
m[  1] = E -> U M F E[D]C Z I H O W V B X J R T P Y Q A L K N S G
m[  2] = O -> G D Y B K X A R L N E I P J[S]M V H O W Z Q T F C U
m[  3] = B -> K[Z]G W L Y C S V R A E Q O N X M J H U T I D P F B
CDSZ
works well


*/
