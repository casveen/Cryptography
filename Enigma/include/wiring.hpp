#ifndef WIRING_H
#define WIRING_H
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

class Wiring {
  private:
    class Connector {
      private:
        int       m_connections_count;
        list<int> m_connects;   // points to live of Wiring
      public:
        Connector(int);
        ~Connector();
        void      take_connections_of(Connector &);
        int       get_connections_count() const;
        list<int> get_connections() const;
        void      reset(int);
    };

  protected:
    int                 m_wires;
    vector<bool *>      m_live;
    vector<Connector *> m_connected_to;
    vector<Connector *> m_all_connectors;

  public:
    Wiring(int wires);
    ~Wiring();
    void connect(int, int);
    bool get(int) const;
    void set(int, bool);
    void reset();
    void wipe();
    int  get_connections_count(int) const;
};
#endif
