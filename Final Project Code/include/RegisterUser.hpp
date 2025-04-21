#pragma once
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

    static int generateNextLibraryID() {
        ifstream inFile("Final Project Code/data/users.txt");
        string line;
        int userCount = 0;

        while (getline(inFile, line)) {
            if (!line.empty()) {
                userCount++;
            }
        }

        return userCount + 1;
    }

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