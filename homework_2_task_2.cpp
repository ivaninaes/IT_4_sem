#include <iostream>
#include <map>
#include <string>

using namespace std;

class Phonebook {
private:
    std::map<std::string, std::string> contacts;

public:
    void addContact(const std::string& name, const std::string& phone) {
        contacts[name] = phone;
        std::cout << "We added contact: " << name << " with phone number " << phone << std::endl;
    }

    void removeContact(const std::string& name) {
        if (contacts.erase(name)) {
            std::cout << "We removed contact: " << name << std::endl;
        } else {
            std::cout << "There is no contact with name: " << name << std::endl;
        }
    }

    void findContacts(const std::string& name) {
        auto it = contacts.find(name);
        if (it != contacts.end()) {
            std::cout << name << " is " << it->second << std::endl;
        } else {
            std::cout << "There is no contact with name: " << name << std::endl;
        }
    }

    void displayContacts() {
        std::cout << "Contacts:" << std::endl;
        for (const auto& it : contacts) {
            std::cout << it.first << ": " << it.second << std::endl;
        }
    }
};

int main() {
    Phonebook p;
    p.addContact("John", "1234");
    p.addContact("Jojo", "5678");
    p.displayContacts();
    p.findContacts("Jojo");
    p.findContacts("Ojoj");
    p.removeContact("Jojo");
    p.removeContact("Ojoj");
    p.displayContacts();
    return 0;
}
