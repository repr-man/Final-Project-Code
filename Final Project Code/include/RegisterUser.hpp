#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std; 

class RegisterUser {
public:
    string firstName, lastName, address, phone, email, password, userType;
    string schoolID;
    int libraryID;
    bool isActive = true;

    RegisterUser() {}

    void promptUserData(int nextID) {
        libraryID = nextID;
        cout << "\n--- Registering ---\n";
        cout << "Enter User Type (e.g. student, faculty): ";
        std::getline(cin >> std::ws, userType);

        cout << "Enter First Name: ";
        std::getline(cin, firstName);

        cout << "Enter Last Name: ";
        std::getline(cin, lastName);

        cout << "Enter Address: ";
        std::getline(cin, address);

        cout << "Enter Phone Number: ";
        std::getline(cin, phone);

        cout << "Enter Email: ";
        std::getline(cin, email);

        cout << "Enter Password: ";
        std::getline(cin, password);

        cout << "Enter School ID (e.g. campus ID): ";
        std::getline(cin, schoolID);
    }

    void printSummary() const {
        cout << "\nUser Registered Successfully:\n";
        cout << "ID: " << setw(10) << setfill('0') << libraryID << "\n";
        cout << "Name: " << firstName << " " << lastName << "\n";
        cout << "Address: " << address << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Email: " << email << "\n";
        cout << "User Type: " << userType << "\n";
        cout << "School ID: " << schoolID << "\n";
    }

    void saveToFile(const string& filename = "Final Project Code/data/users.txt") const {
        ofstream outFile(filename, ios::app); // append mode
        if (!outFile) {
            cerr << "Error: Could not open " << filename << " for writing.\n";
            return;
        }

        // Format libraryID with leading zeroes (10 digits) /*need to change this so that it's randomized for each new registered user*/
        outFile << setw(10) << setfill('0') << libraryID << ";"
            << userType << ";"
            << firstName << ";"
            << lastName << ";"
            << address << ";"
            << phone << ";"
            << email << ";"
            << password << ";"
            << schoolID << ";"
            << isActive << std::endl;

        outFile.close();
    }
}; // end of class RegisterUser