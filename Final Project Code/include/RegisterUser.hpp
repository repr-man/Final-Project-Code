#pragma once
#include "library.hpp"
#include "terminal.hpp"
#include "validators.hpp"
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

    void promptUserData(Terminal& term, int nextID) { 
        libraryID = nextID;

        cout << "\n--- Registering ---\n";
        userType = term.promptForInput<string, validateRole>(
            "Enter User Type (e.g. student, faculty)"
        );
        firstName = term.promptForInput<string>("Enter First Name");
        lastName = term.promptForInput<string>("Enter Last Name");
        address = term.promptForInput<string>("Enter Address");
        phone = term.promptForInput<string, validatePhone>("Enter Phone Number");
        email = term.promptForInput<string, validateEmail>("Enter Email");
        password = term.promptForInput<string>("Enter Password");
        schoolID = to_string(term.promptForInput<long, validateInstitutionID>(
            "Enter School ID (e.g. campus ID)"
        ));
    }

    void printSummary() const {
        cout << "\nUser Registered Successfully:\n";
        cout << "ID: " << setw(10) << setfill('0') << libraryID << setfill(' ') << "\n";
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
