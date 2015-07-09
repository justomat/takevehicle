#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include "Helper.h"

using namespace std;

class Displayable {
  public:
    virtual ~Displayable() {
    }

    virtual void display() = 0;
};

class Serializable {
  public:
    virtual ~Serializable() {
    }

    virtual string save() = 0;
    virtual void load(string) = 0;
};

class User
    : public Displayable
    , public Serializable {
  protected:
    string username, password;
  public:
    explicit User(string u = "", string p = "")
        : username(u)
        , password(p) {
    }

    virtual ~User() {
    }

    string getId() {
        return getUsername();
    }

    string getUsername() {
        return username;
    }

    string getPassword() {
        return password;
    }

    void setId(string i) {
        setUsername(i);
    }

    void setUsername(string u) {
        username = u;
    }

    void setPassword(string p) {
        password = p;
    }

    void changePassword() {
        cout << "Enter new password: ";
        cin >> password;
    }

    virtual void set() {
        cout << "Enter id: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
    }
};

class Driver
    : public User {
    string first;
    string last;
    string pnumber;
    string zcode;
    string vehicle;
  public:
    Driver()
        : vehicle("-") {
    }

    Driver(Driver*) {
    }

    ~Driver() {
    }

    void set() override {
        User::set();
        cout << "Enter first name: ";
        cin >> first;
        cout << "Enter last name: ";
        cin >> last;
        cout << "Enter phone number: ";
        cin >> pnumber;
        cout << "Enter zipcode: ";
        cin >> zcode;
    }

    void display() override {
        cout
                << "id: " << username << endl
                << "password: " << password << endl
                << "first: " << first << endl
                << "last: " << last << endl
                << "pnumber: " << pnumber << endl
                << "zcode: " << zcode << endl
                << "vehicle: " << vehicle << endl;
    }

    string save() override {
        stringstream ss;
        ss
                << username << ','
                << password << ','
                << first << ','
                << last << ','
                << pnumber << ','
                << zcode << ','
                << vehicle << '\n';
        return ss.str();
    }

    void load(string in) {
        auto data = Helper::split(in, ',');
        if (data.size() != 7) return;
        username = data[0];
        password = data[1];
        first = data[2];
        last = data[3];
        pnumber = data[4];
        zcode = data[5];
        vehicle = data[6];
    }

    void assign(string v) {
        vehicle = v;
    }

    void unassign() {
        assign("-");
    }
};

class Vehicle
    : public Displayable
    , public Serializable {
    string id;
    string capacity;
    string type;
  public:
    Vehicle() {
    }

    ~Vehicle() {
    }

    void setId(int x) {
        id = x;
    }

    string getId() {
        return id;
    }

    void setCapacity(int x) {
        capacity = x;
    }

    string getCapacity() {
        return capacity;
    }

    void setType(string x) {
        type = x;
    }

    string getType() {
        return type;
    }

    void set() {
        cout << "Enter id: ";
        cin >> id;
        cout << "Enter capacity: ";
        cin >> capacity;
        cout << "Enter type: ";
        cin >> type;
    }

    void display() {
        cout
                << "id: " << id << endl
                << "capacity: " << capacity << endl
                << "type: " << type << endl;
    }

    string save() {
        stringstream ss;
        ss
                << id << ','
                << capacity << ','
                << type << '\n';
        return ss.str();
    }

    void load(string in) {
        auto data = Helper::split(in, ',');
        if (data.size() != 3) return;
        id = data[0];
        capacity = data[1];
        type = data[2];
    }
};

class Staff
    : public User {
  public:
    Staff() {
    }

    ~Staff() {
    }

    void display() override {
    }

    string save() {
        stringstream ss;
        ss
                << username << ','
                << password << '\n';
        return ss.str();
    }

    void load(string in) {
        auto data = Helper::split(in, ',');
        if (data.size() != 2) return;
        username = data[0];
        password = data[1];
    }
};

template <class T>
class Database
    : public Displayable {
    string filename;
    vector<T> elems;
  public:
    Database(string);
    virtual ~Database();
    void load();
    void save();
    void display() override;
    void add(T e);
    void remove(string id);
    T& get(string id);
};

template <class T>
void Database<T>::remove(string id) {
    for (size_t it = 0; it < elems.size(); it++)
        if (elems[it].getId() == id)
            elems.erase(elems.begin() + it);
}

template <class T>
T& Database<T>::get(string id) {
    for (T& e : elems)
        if (e.getId() == id)
            return e;
    throw runtime_error("Not Found");
}

template <class T>
void Database<T>::add(T e) {
    elems.push_back(e);
}

template <class T>
void Database<T>::display() {
    for (T e : elems)
        e.display();
}

template <class T>
void Database<T>::save() {
    ofstream out(filename);
    for (T e : elems)
        out << e.save();
    out.close();
}

template <class T>
void Database<T>::load() {
    ifstream in(filename);
    T e;
    string buf;
    while (getline(in, buf)) {
        e.load(buf);
        add(e);
    }
    in.close();
}

template <class T>
Database<T>::~Database() {
}

template <class T>
Database<T>::Database(string filename)
    : filename(filename) {
    load();
}

class Node {
  public:
    virtual ~Node() {
    }

    virtual std::string getText() const = 0;
    virtual bool run() = 0;
};

class Item : public Node {
    std::string text;
    std::function<void()> function;
  public:
    Item(std::string s, std::function<void()> f)
        : text(s)
        , function(f) {
    }

    inline std::string getText() const override {
        return text;
    }

    inline bool run() override {
        function();
        return false;
    }
};

class Menu : public Node {
  private:
    std::vector<Node*> nodes;
    std::string text;
  public:
    Menu() {
    }

    ~Menu() {
        for (size_t i = 0; i < nodes.size(); ++i)
            delete nodes[i];
    }

    void add(std::vector<Node*> newNode) {
        nodes.insert(nodes.end(),
                     std::make_move_iterator(newNode.begin()),
                     std::make_move_iterator(newNode.end()));
    }

    std::string getText() const override {
        return text;
    }

    bool run() override {
        std::cout << "Choose:\n\n";
        for (size_t i = 0; i < nodes.size();)
            std::cout << "  " << ++i << ". " << nodes[i]->getText() << "\n";
        std::cout << "\n\nSelect: ";

        size_t choice;
        std::cin >> choice;
        cin.clear();
        cin.ignore(10000, '\n');
        if (nodes.size() == choice)
            return false;
        if (nodes.size() > --choice) {
            try {
                nodes[choice]->run();
                return true;
            } catch (...) {
                cerr << "Not Found\n";
            }
        }
        return true;
    }
};

class System {
    User* user;
    Menu loginMenu;
    Menu staffMenu;
    Menu driverMenu;
    Database<Staff>* staffs;
    Database<Driver>* drivers;
    Database<Vehicle>* vehicles;
  public:
    System() {
        staffs = new Database<Staff>("staff.txt");
        drivers = new Database<Driver>("driver.txt");
        vehicles = new Database<Vehicle>("vehicle.txt");
        loginMenu.add({
            new Item("staff login", [&]
            {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (staffs->get(username).getPassword() == password) {
                    user = &staffs->get(username);
                    while (staffMenu.run());
                }
            }),
            new Item("driver login", [&]
            {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (drivers->get(username).getPassword() == password) {
                    user = &drivers->get(username);
                    while (driverMenu.run());
                }
            }),
            new Item("logout", [&]
            {
            })
        });
        staffMenu.add({
            new Item("Create Driver", [&]
            {
                addDriver();
            }),
            new Item("Create Vehicle", [&]
            {
                addVehicle();
            }),
            new Item("Delete Driver", [&]
            {
                removeDriver();
            }),
            new Item("Delete Vehicle", [&]
            {
                removeVehicle();
            }),
            new Item("Assign Vehicle", [&]
            {
                assignVehicle();
            }),
            new Item("Unassign Vehicle", [&]
            {
                unassignVehicle();
            }),
            new Item("List All Drivers", [&]
            {
                listAllDriver();
            }),
            new Item("List All Vehicles", [&]
            {
                listAllVehicle();
            }),
            new Item("List Particular Driver", [&]
            {
                getDriver();
            }),
            new Item("List Particular Vehicle", [&]
            {
                getVehicle();
            }),
            new Item("Edit Driver Details", [&]
            {
                editDriver();
            }),
            new Item("Edit Vehicle Details", [&]
            {
                editVehicle();
            }),
            new Item("Change Password", [&]
            {
                user->changePassword();
            }),
            new Item("Logout", [&]
            {
            })
        });
        driverMenu.add({
            new Item("View Details", [&]
            {
                user->display();
            }),
            new Item("Update Details", [&]
            {
                cout << "Enter new information\n";
                user->set();
            }),
            new Item("Change Password", [&]
            {
                user->changePassword();
            }),
            new Item("Logout", [&]
            {
            })
        });
        while (loginMenu.run());
    }

    ~System() {
        staffs->save();
        drivers->save();
        vehicles->save();
        delete staffs;
        delete drivers;
        delete vehicles;
    }

    void listAllDriver() {
        drivers->display();
    }

    void listAllVehicle() {
        vehicles->display();
    }

    void addDriver() {
        Driver d;
        d.set();
        drivers->add(d);
    }

    void addVehicle() {
        Vehicle v;
        v.set();
        vehicles->add(v);
    }

    void assignVehicle() {
        getDriver().assign(getVehicle().getId());
    }

    void unassignVehicle() {
        getDriver().assign("-");
    }

    void removeDriver() {
        drivers->remove(getDriver().getId());
    }

    Driver& getDriver() {
        string username;
        cout << "Enter username: ";
        cin >> username;
        Driver& d = drivers->get(username);
        d.display();
        return d;
    }

    void editDriver() {
        auto d = getDriver();
        drivers->remove(d.getId());
        cout << "Enter new information\n";
        d.set();
        drivers->add(d);
    }

    void removeVehicle() {
        vehicles->remove(getVehicle().getId());
    }

    Vehicle& getVehicle() {
        string id;
        cout << "Enter id: ";
        cin >> id;
        Vehicle& v = vehicles->get(id);
        v.display();
        return v;
    }

    void editVehicle() {
        auto v = getVehicle();
        drivers->remove(v.getId());
        cout << "Enter new information\n";
        v.set();
        vehicles->add(v);
    }
};


int main() {
    System s;
    return 0;
}
