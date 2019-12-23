#ifndef ENIGMA_VARIATIONS_HPP
#    def ENIGMA_VARIATIONS_HPP

class EnigmaA: public Enigma {
  private:
    string description=
        "Commercial Enigma type A. Lacks a reflector and plugboard.";

  public:
};

class EnigmaB: public Enigma {
  private:
    string description=
        "Commercial Enigma type B. Lacks a reflector and plugboard.";

  public:
};

class EnigmaC: public Enigma {
  private:
    string description=
        "Commercial Enigma type C. First type to have a "
        "reflector, lacks a plugboard. Also called the \"glowlamp enigma\".";

  public:
};

class EnigmaD: public EnigmaC {
  private:
    string description= "Commercial Enigma type D. The first type to be widely "
                        "used. Lacks a plugboard.";

  public:
};

class NavyCipherD: public EnigmaD {
  private:
    string description= "Used by the spanish navy. Lacks a plugboard.";

  public:
};

class EnigmaK: public EnigmaD {
  private:
    string description= "Swiss K, used by the Swiss";

  public:
};

class TypeX: public Enigma {
  private:
    string description= "TypeX, a vast improvement of the standard Enigma, "
                        "used by the british.";

  public:
};

class FunkschlusselC: public Enigma {
  private:
    string description= "Earliest model used by the military, uses 28 wires "
                        "and bypasses X.";

  public:
};

// Enigma G/abwehr/counter machine  ->four wheel unsteckered multiple notches
// Wehrmacht/enigma I -> plugboard, fixed reflector notches on letter ring
// M3/funkschlussel -> navy three from five, army three from three
// M4   -> four roto, reflector split into thin and thin reflector, neither
// which moved

#endif
