#include "wiring.hpp"

Wiring::Wiring(int wires) : m_wires{wires} {
    // init attributes
    m_live= new bool[m_wires];
    for (int i= 0; i < m_wires; ++i) {
        // cout << "push\n";
        m_live[i]= false;
        m_all_connectors.push_back(new Connector(i));
        m_connected_to.push_back(m_all_connectors[i]);
        // cout << "end of loop\n";
    }
    // cout << "made\n";
}
Wiring::~Wiring() {
    // cout << "wiring destroyed\n";
    for (auto connection : m_all_connectors) { delete connection; }
    delete[] m_live;
}
void Wiring::connect(int wire1, int wire2) {
    // find connector of each
    Connector &connector1= *m_connected_to[wire1];
    Connector &connector2= *m_connected_to[wire2];
    // merge connectors, /merge smallest into
    if (&connector1 != &connector2) {
        if (connector1.get_connections_count() >
            connector2.get_connections_count()) {
            // connector 2 is smallest
            // update connected to
            for (int connection_index : connector2.get_connections()) {
                m_connected_to[connection_index]= &connector1;
            }
            // take connections
            connector1.take_connections_of(connector2);
        } else {
            // connector 1 is smallest
            // update connected to
            for (int connection_index : connector1.get_connections()) {
                m_connected_to[connection_index]= &connector2;
            }
            // take connections
            connector2.take_connections_of(connector1);
        }
    }
}
void Wiring::set(int wire, bool value) {
    // get connector of, then set all this one is connected tot to value
    Connector &connector= *m_connected_to[wire];   // XXX might die after
    for (int connection_index : connector.get_connections()) {
        m_live[connection_index]= value;
    }
}
bool Wiring::get(int wire) const { return m_live[wire]; }
void Wiring::reset() {
    for (int i= 0; i < m_wires; ++i) {
        m_live[i]= false;
        m_all_connectors[i]->reset(i);
        m_connected_to[i]= m_all_connectors[i];
    }
}
void Wiring::wipe() {
    for (int i= 0; i < m_wires; ++i) { m_live[i]= false; }
}

Wiring::Connector::Connector(int initial_wire) {
    // set connects to live of Wiring
    m_connects.push_back(initial_wire);
    m_connections_count= 1;
}
Wiring::Connector::~Connector() { cout << "connector destrpyed\n"; }
void Wiring::Connector::take_connections_of(Connector &other) {
    m_connections_count+= other.get_connections_count();
    // connects.reserve(m_connections_count); // preallocate memory
    m_connects.splice(m_connects.end(), other.get_connections());   // moves
}
int Wiring::Connector::get_connections_count() const {
    return m_connections_count;
}
// reference...
list<int> &Wiring::Connector::get_connections() { return m_connects; }
void       Wiring::Connector::reset(int initial_wire) {
    // cout << m_connects.size() << " - ";
    // if (m_connects.size() > 0) {
    m_connects.clear();
    m_connections_count= 1;
    //}
    m_connects.push_back(initial_wire);
}
int Wiring::get_connections_count(int wire) const {
    return m_connected_to[wire]->get_connections_count();
}
