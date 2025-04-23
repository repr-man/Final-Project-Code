#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "Borrowing.hpp"
#include "RegisterUser.hpp"
#include "Return.hpp"
#include "SearchFunction.hpp"
#include "library.hpp"
#include "terminal.hpp"


using namespace std;

class Admin {
private:
    string username, password;
public:
    Admin(string user, string pass) : username(user), password(pass) {}

    bool login(string inputUser, string inputPass) {
        return (inputUser == username && inputPass == password);
    }

    void showMenu(Library& lib, Terminal& term) {
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
                cout << "\n--- Search Menu ---\n";
                cout << "1. User\n";
                cout << "2. Inventory\n";
                cout << "3. Exit\n";
                cout << "Enter your choice: " << endl;
                choice = term.promptForInput<int>();
                    
                SearchFunction s;
                switch (choice) {
                    case 1: {
                        auto res = s.searchUser(lib, term);
                        term.printTable(res, "ID", "Role", "First Name", "Last Name", "Address", "Phone Number", "Email", "Password", "Institution ID", "# Items Borrowed");
                        break;
                    }
                    case 2: {
                        auto res = s.searchInventory(lib, term);
                        term.printTable(res, "Type", "Name", "Author", "Publisher", "Borrower ID");
                        break;
                    }
                    case 3:
                        cout << "Exiting system.\n";
                        return;
                    default:
                        cout << "Invalid choice. Try again.\n";
                        throw string("Error: Invalid choice.");
                }
                break;

            case 6:
                cout << "Viewing all registered users...\n";
                // add function call here
                break;
            case 7:
                cout << "Returning books...\n";
                Return r;
                r.ReturnBook(term);
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

    void registerNewUser() {
        int nextID = generateNextLibraryID();  
        RegisterUser newUser;
        newUser.promptUserData(nextID); 
        newUser.printSummary();
        newUser.saveToFile();
    }


}; // end of admin class

