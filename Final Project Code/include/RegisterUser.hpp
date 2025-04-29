#pragma once
#include "library.hpp"
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
    int booksborrowed = 0;

    RegisterUser() {}

    static int generateNextLibraryID() {
        ifstream inFile("data/users.txt");
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

    void saveToFile(Library& lib) {
        lib.addUser(libraryID, std::move(userType), std::move(firstName), std::move(lastName), std::move(address), std::move(phone), std::move(email), std::move(password), std::move(schoolID));
    }
}; // end of class RegisterUser
