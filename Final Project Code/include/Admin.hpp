#pragma once
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <filesystem>
#include "Borrowing.hpp"
#include "RegisterUser.hpp"


using namespace std;

class Admin {
private:
    string username, password;
public:
    Admin(string user, string pass) : username(user), password(pass) {}

    bool login(string inputUser, string inputPass) {
        return (inputUser == username && inputPass == password);
    }

    void showMenu(Terminal &term) {
        int choice;
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Register New User\n";
            cout << "2. Edit Inventory\n";
            cout << "3. Edit User Information\n";
            cout << "4. Borrow Book\n";
            cout << "5. Search Function\n";
            cout << "6. Active Users\n";
            cout << "7. Return Book\n";
            cout << "8. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: 
                cout << "Registering new user...";
                registerNewUser();
                break;
            case 2:
                cout << "Editing Inventory...\n";
                // Add function call here
                break;
            case 3:
                cout << "Editing User Information...\n";
                // Add function call here
                break;
            case 4: {
                cout << "Borrowing books...\n";
                Borrowing b;
                b.borrowBook(term);

                break;
            }// end of case 4
            
            case 5: // this sends them to the SearchFunction Class

                break;

            case 6:
                cout << "Viewing all registered users...\n";
                // add function call here
                break;
            case 7:
                cout << "Returning books...\n";
                // add function call here
                break;
            case 8:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
private: 
    int generateNextLibraryID() {
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

    void registerNewUser() {
        int nextID = generateNextLibraryID();  
        RegisterUser newUser;
        newUser.promptUserData(nextID); 
        newUser.printSummary();
        newUser.saveToFile();
    }


}; // end of admin class

