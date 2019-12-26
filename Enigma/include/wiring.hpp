#ifndef WIRING_H
#define WIRING_H
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
typedef int wint;
using namespace std;

class Wiring {
  private:
    class Connector {
      private:
        int          m_connections_count;
        vector<wint> m_connects;   // points to live of Wiring
      public:
        Connector(wint);
        ~Connector();
        void          take_connections_of(Connector &);
        int           get_connections_count() const;
        void          set_connections_count(int);
        vector<wint> &get_connections();
        void          reset(wint);
    };

  protected:
    int                 m_wires;
    bool *              m_live;
    vector<Connector *> m_connected_to;
    vector<Connector *> m_all_connectors;
    // vector<Connector *> m_unique_connectors;

  public:
    Wiring(int wires);
    ~Wiring();
    void connect(int, int);
    void connect(vector<int>, vector<int>);
    bool get(int) const;
    void set(int, bool);
    void reset();
    void wipe();
    int  get_connections_count(int) const;
};
#endif
