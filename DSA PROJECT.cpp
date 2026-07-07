#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

// ================= UTIL =================
int convertDate(string d) {
    if (d.size() != 10) return 0;

    return stoi(d.substr(6,4)) * 10000 +
           stoi(d.substr(3,2)) * 100 +
           stoi(d.substr(0,2));
}

// ================= CUSTOMER =================
class Customer {
public:
    int id;
    string name;
    string phone;
};

class CustomerManager {
private:
    vector<Customer> customers;
    int nextId = 1;

public:
    int addCustomer(string name, string phone) {
        customers.push_back({nextId, name, phone});
        cout << "? Customer ID: " << nextId << endl;
        return nextId++;
    }

    void show() {
        cout << "\n--- CUSTOMERS ---\n";
        for (auto &c : customers)
            cout << c.id << " | " << c.name << " | " << c.phone << endl;
    }
};

// ================= SERVICE =================
struct Service {
    string name;
    int price;
};

vector<Service> services = {
    {"Bridal Makeup", 10000},
    {"Party Makeup", 5000},
    {"Engagement Makeup", 8000},
    {"HD Makeup", 12000},
    {"Photoshoot Makeup", 7000},
    {"Airbrush Makeup", 15000},

    {"Facial Basic", 2000},
    {"Facial Gold", 3500},
    {"Facial Whitening", 4000},
    {"Clean Up", 1500},

    {"Hair Styling", 3000},
    {"Hair Cutting", 1200},
    {"Hair Coloring", 5000},
    {"Hair Rebonding", 8000},

    {"Manicure", 1800},
    {"Pedicure", 2000},
    {"Nail Art", 2500},

    {"Eyebrow Shaping", 500},
    {"Threading", 300},
    {"Waxing Full Body", 4500}
};

// ================= INVENTORY =================
struct Item {
    string name;
    int qty;
};

vector<Item> inventory = {
    // Makeup
    {"Lipstick", 50},
    {"Liquid Foundation", 30},
    {"Compact Powder", 40},
    {"Concealer", 25},
    {"Primer", 20},
    {"Blush", 35},
    {"Highlighter", 15},
    {"Contour Kit", 10},
    {"Eyeshadow Palette", 20},
    {"Eyeliner", 45},
    {"Mascara", 30},
    {"Makeup Fixer Spray", 18},

    // Skincare
    {"Face Cleanser", 25},
    {"Face Wash", 40},
    {"Toner", 22},
    {"Moisturizer", 30},
    {"Sunscreen", 28},
    {"Face Scrub", 18},

    //  Hair Care
    {"Hair Spray", 15},
    {"Hair Gel", 12},
    {"Hair Serum", 25},
    {"Hair Oil", 35},
    {"Hair Dye", 20},
    {"Hair Conditioner", 30},

    //  Nails
    {"Nail Polish", 60},
    {"Nail Remover", 25},
    {"Nail Art Kit", 10},

    // Tools
    {"Makeup Brushes Set", 20},
    {"Beauty Blender", 40},
    {"Eyelash Curler", 15},
    {"Tweezers", 30},
    {"Mirror Kit", 10}
};

// ================= APPOINTMENT =================
class Appointment {
public:
    int id;
    int customerId;
    string customerName;
    string phone;
    string serviceName;
    int price;
    string date;
    string time;
};

// ================= ACTION =================
struct Action {
    string type;
    Appointment appt;
};

// ================= SYSTEM =================
class SalonSystem {
private:
    vector<Appointment> appointments;
    stack<Action> history;

    int totalBooked = 0;
    int totalCancelled = 0;
    int totalUndo = 0;

    int nextId = 1;

public:

    void book(int cid, string cname, string phone,
              string service, int price,
              string date, string time) {

        Appointment a;
        a.id = nextId++;
        a.customerId = cid;
        a.customerName = cname;
        a.phone = phone;
        a.serviceName = service;
        a.price = price;
        a.date = date;
        a.time = time;

        appointments.push_back(a);
        history.push({"BOOK", a});

        totalBooked++;

        cout << "? Booked ID: " << a.id << endl;
    }

    void show() {

        sort(appointments.begin(), appointments.end(),
        [](Appointment a, Appointment b) {
            if (convertDate(a.date) == convertDate(b.date))
                return a.time < b.time;

            return convertDate(a.date) < convertDate(b.date);
        });

        cout << "\n--- APPOINTMENTS ---\n";

        for (auto &a : appointments) {
            cout << a.id << " | "
                 << a.customerName << " | "
                 << a.phone << " | "
                 << a.serviceName << " | "
                 << a.date << " " << a.time << endl;
        }
    }

    void cancel(int id) {
        for (int i = 0; i < appointments.size(); i++) {
            if (appointments[i].id == id) {

                history.push({"CANCEL", appointments[i]});
                appointments.erase(appointments.begin() + i);

                totalCancelled++;

                cout << "? Cancelled ID: " << id << endl;
                return;
            }
        }

        cout << "? Invalid ID!\n";
    }

    void undo() {
        if (history.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }

        Action a = history.top();
        history.pop();

        if (a.type == "BOOK") {
            if (!appointments.empty())
                appointments.pop_back();
        } else {
            appointments.push_back(a.appt);
        }

        totalUndo++;

        cout << "? Undo completed.\n";
    }

    void inventoryMenu() {

        while (true) {

            cout << "\n===== INVENTORY =====\n";

            for (int i = 0; i < inventory.size(); i++) {
                cout << i + 1 << ". " << inventory[i].name
                     << " (Stock: " << inventory[i].qty << ")\n";
            }

            cout << "0. Exit\nSelect: ";

            int ch;
            cin >> ch;

            if (ch == 0) break;

            if (ch < 1 || ch > inventory.size()) {
                cout << "? Invalid!\n";
                continue;
            }

            int qty;
            cout << "Qty: ";
            cin >> qty;

            if (qty <= 0 || inventory[ch-1].qty < qty) {
                cout << "? Not enough stock!\n";
                continue;
            }

            inventory[ch-1].qty -= qty;

            cout << "? Used " << qty << " of "
                 << inventory[ch-1].name << endl;
        }
    }

    void reports() {

        cout << "\n========== REPORT ==========\n";
        cout << "Total Booked     : " << totalBooked << endl;
        cout << "Total Cancelled  : " << totalCancelled << endl;
        cout << "Total Undo       : " << totalUndo << endl;

        cout << "\n--- BOOKINGS ---\n";

        for (auto &a : appointments) {
            cout << a.date << " | "
                 << a.time << " | "
                 << a.serviceName << " | "
                 << a.customerName << endl;
        }

        cout << "===========================\n";
    }
};

// ================= MAIN =================
int main() {

    CustomerManager cm;
    SalonSystem system;

    int choice;

    do {
        cout << "\n===== SYSTEM =====\n";
        cout << "1. Add Customer\n";
        cout << "2. Show Customers\n";
        cout << "3. Book Appointment\n";
        cout << "4. Show Appointments\n";
        cout << "5. Cancel\n";
        cout << "6. Undo\n";
        cout << "7. Inventory\n";
        cout << "8. Reports\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        cin >> choice;

        if (choice == 1) {
            string n, p;
            cout << "Name: "; cin >> n;
            cout << "Phone: "; cin >> p;

            cm.addCustomer(n, p);
        }

        else if (choice == 2) {
            cm.show();
        }

        else if (choice == 3) {

            string name, phone, date, time;
            int cid, s;

            cout << "Customer Name: ";
            cin >> name;

            cout << "Phone: ";
            cin >> phone;

            cid = cm.addCustomer(name, phone);

            cout << "\nServices:\n";
            for (int i = 0; i < services.size(); i++)
                cout << i+1 << ". " << services[i].name << endl;

            cin >> s;

            if (s < 1 || s > services.size()) {
                cout << "? Invalid service!\n";
                continue;
            }

            cout << "Date (DD-MM-YYYY): ";
            cin >> date;

            cout << "Time (HH:MM): ";
            cin >> time;

            system.book(
                cid,
                name,
                phone,
                services[s-1].name,
                services[s-1].price,
                date,
                time
            );
        }

        else if (choice == 4)
            system.show();

        else if (choice == 5) {
            int id;
            cin >> id;
            system.cancel(id);
        }

        else if (choice == 6)
            system.undo();

        else if (choice == 7)
            system.inventoryMenu();

        else if (choice == 8)
            system.reports();

    } while (choice != 0);

    cout << "System Exit.\n";
}
public:

    void dashboard() {
        cout << "\n===== DASHBOARD =====\n";
        cout << "Total Bookings   : " << totalBooked << endl;
        cout << "Cancelled        : " << totalCancelled << endl;
        cout << "Undo Actions     : " << totalUndo << endl;
        cout << "Active Bookings  : " << appointments.size() << endl;
    }

    void book(int cid, string cname, string phone,
              string service, int price,
              string date, string time) {

        Appointment a;
        a.id = nextId++;
        a.customerId = cid;
        a.customerName = cname;
        a.phone = phone;
        a.serviceName = service;
        a.price = price;
        a.date = date;
        a.time = time;

        appointments.push_back(a);
        history.push({"BOOK", a});

        totalBooked++;

        cout << "? Booked ID: " << a.id << endl;
    }

    void show() {
        sort(appointments.begin(), appointments.end(),
        [](Appointment a, Appointment b) {
            if (convertDate(a.date) == convertDate(b.date))
                return a.time < b.time;

            return convertDate(a.date) < convertDate(b.date);
        });

        cout << "\n--- APPOINTMENTS ---\n";

        for (auto &a : appointments) {
            cout << a.id << " | "
                 << a.customerName << " | "
                 << a.serviceName << " | "
                 << a.date << " " << a.time << endl;
        }
    }

    void cancel(int id) {
        for (int i = 0; i < appointments.size(); i++) {
            if (appointments[i].id == id) {

                history.push({"CANCEL", appointments[i]});
                appointments.erase(appointments.begin() + i);

                totalCancelled++;

                cout << "? Cancelled ID: " << id << endl;
                return;
            }
        }
        cout << "? Invalid ID!\n";
    }

    void undo() {
        if (history.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }

        Action a = history.top();
        history.pop();

        if (a.type == "BOOK") {
            if (!appointments.empty())
                appointments.pop_back();
        } else {
            appointments.push_back(a.appt);
        }

        totalUndo++;

        cout << "? Undo completed.\n";
    }

    void reports() {
        cout << "\n===== REPORT =====\n";
        cout << "Booked: " << totalBooked << endl;
        cout << "Cancelled: " << totalCancelled << endl;
        cout << "Undo: " << totalUndo << endl;
    }
};