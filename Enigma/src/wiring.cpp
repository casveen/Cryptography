#include "wiring.hpp"

Wiring::Wiring(int wires) : m_wires{wires} {
    // init attributes
    m_live= new bool[m_wires];
    for (int i= 0; i < m_wires; ++i) {
        // cout << "push\n";
        m_live[i]= false;
        m_all_connectors.push_back(new Connector(&m_live[i]));
        m_all_connectors[i]->set_id(i);
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
    Connector *connector1= m_connected_to[wire1];
    Connector *connector2= m_connected_to[wire2];

    // merge connectors, /merge smallest into
    if (connector1->get_connections() != connector2->get_connections()) {
        if (connector1->get_connections_count() <
            connector2->get_connections_count()) {
            Connector *temp= connector1;
            connector1     = connector2;
            connector2     = temp;
        }

        // int connection_sum= connector1->get_connections_count() +
        //                    connector2->get_connections_count();
        // connector1->get_connections().reserve(connection_sum);
        // connector1->set_connections_count(connection_sum);
        for (Connector *connector : *connector2->get_connections()) {

            connector1->get_connections()->push_back(connector);
            connector->get_connections()->clear();
            connector->set_connections(connector1->get_connections());
        }
        /*cout << wire1 << "<->" << wire2 << ": "
             << connector1->get_connections()->size() << "\n    ";
        for (Connector *connector : *connector1->get_connections()) {
            cout << connector->get_id() << " ";
        }
        cout << "\n    -------------------\n    ";
        for (Connector *connector : *connector2->get_connections()) {
            cout << connector->get_id() << " ";
        }
        cout << "\n";
        cin.get();
*/
        // connector2->get_connections()->clear();
        // connector2->set_connections(connector1->get_connections());
        // connector2->set_connections_count(0);
        //
    }
}

void Wiring::set(int wire, bool value) {
    // get connector of, then set all this one is connected tot to value
    Connector &connector= *m_connected_to[wire];
    for (Connector *connector : *connector.get_connections()) {
        connector->set_value(value);
    }
}
bool Wiring::get(int wire) const { return m_live[wire]; }
void Wiring::reset() {
    for (int i= 0; i < m_wires; ++i) {
        m_live[i]= false;
        m_connected_to[i]->reset();
        // m_all_connectors[i]->get_connections().push_back(i);
        // m_connected_to[i]= m_all_connectors[i];
    }
}
void Wiring::wipe() {
    for (int i= 0; i < m_wires; ++i) { m_live[i]= false; }
}

Wiring::Connector::Connector(bool *initial_wire) {
    // set connects to live of Wiring
    m_bool    = initial_wire;
    m_connects= new vector<Connector *>;
    m_connects->push_back(this);
    m_connects_original= m_connects;
    m_connections_count= 1;
}
Wiring::Connector::~Connector() {   // delete m_connects;
}
void Wiring::Connector::take_connections_of(Connector &other) {
    m_connections_count+= other.get_connections_count();
    // m_connects= 0;

    other.get_connections()->clear();
    // connects.reserve(m_connections_count); // preallocate memory
    // m_connects.splice(m_connects.end(), other.get_connections());   // moves
}
void Wiring::Connector::set_value(bool value) { *m_bool= value; }
int  Wiring::Connector::get_connections_count() const {
    return m_connects->size();
}
void Wiring::Connector::set_connections_count(int in) {
    m_connections_count= in;
}
// reference...
void Wiring::Connector::set_connections(vector<Connector *> *set) {
    m_connects= set;
}
vector<Wiring::Connector *> *Wiring::Connector::get_connections() {
    return m_connects;
}
void Wiring::Connector::reset() {
    m_connects= m_connects_original;
    if (m_connects->size() > 1) {
        m_connects->clear();
    } else if (m_connects->size() == 0) {
        m_connects->push_back(this);
    }
}
int Wiring::get_connections_count(int wire) const {
    return m_connected_to[wire]->get_connections_count();
}
int  Wiring::Connector::get_id() { return m_id; }
void Wiring::Connector::set_id(int id) { m_id= id; }
