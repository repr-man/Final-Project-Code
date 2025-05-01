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
            cout << "1. Inventory Managment\n";
            cout << "2. User Managment\n";
            cout << "3. Borrow Book\n";
            cout << "4. Search Function\n";
            cout << "5. Active Users\n";
            cout << "6. Return Book\n";
            cout << "7. Logout\n";
            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cin.clear(); // Clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush buffer
                cout << "Invalid input. Please enter a number.\n";
                continue; // Prompt again
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline after valid input

            switch (choice) {
            case 1:
                cout << "Inventory Management...\n";
                editInventory(lib, term);
                break;
            case 2:
                cout << "User Management...\n";
                editUserInfo(lib, term);
                break;
            case 3: {
                cout << "Borrowing books...\n";
                Borrowing b;
                b.borrowBook(lib, term);
                break;
            }// end of case 4
            
            case 4: // this sends them to the SearchFunction Class
                cout << "\n--- Search Menu ---\n";
                cout << "1. User\n";
                cout << "2. Inventory\n";
                cout << "3. Exit\n";
                choice = term.promptForInput<int>("Enter your choice");
                    
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

            case 5:
                cout << "Viewing all registered users...\n";
                // add function call here
                break;
            case 6:
                cout << "Returning books...\n";
                Return r;
                r.ReturnBook(lib, term);
                break;
            case 7:
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

    void registerNewUser(Library& lib, Terminal& term) {
        int nextID = generateNextLibraryID();  
        RegisterUser newUser;
        newUser.promptUserData(term, nextID); 
        newUser.printSummary();
        newUser.saveToFile(lib);
    }

    void editUserInfo(Library& lib, Terminal& term) {
        int choice;
        cout << "\n--- User Management Menu ---\n";
        cout << "1. Register New User\n";
        cout << "2. Update User Information\n";
        cout << "3. Delete User\n";
        cout << "4. Back to Admin Menu\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

        switch (choice) {
        case 1: { // Register New User
            int nextID = generateNextLibraryID();
            RegisterUser newUser;
            newUser.promptUserData(term, nextID);
            newUser.printSummary();
            newUser.saveToFile(lib);
            cout << "New user registered successfully!\n";
            break;
        }
        case 2:{ // Update User
            const auto& users = lib.getUsers();
            if (users.empty()) {
                cout << "No users to update.\n";
                break;
            }

            cout << "\n--- Registered Users ---\n";
            for (size_t i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i].first << " " << users[i].last << "\n";
            }

            int userIndex;
            cout << "Enter the number of the user to update (0 to cancel): ";
            while (!(cin >> userIndex) || userIndex < 0 || userIndex > static_cast<int>(users.size())) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid user number: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (userIndex == 0) {
                cout << "Update cancelled.\n";
                break;
            }

            string newFirstName, newLastName, newAddress, newPhone, newEmail;

            cout << "Enter new first name: ";
            getline(cin, newFirstName);
            cout << "Enter new last name: ";
            getline(cin, newLastName);
            cout << "Enter new address: ";
            getline(cin, newAddress);
            cout << "Enter new phone number: ";
            getline(cin, newPhone);
            cout << "Enter new email: ";
            getline(cin, newEmail);

            lib.updateUser(userIndex - 1, newFirstName, newLastName, newAddress, newPhone, newEmail);
            cout << "User information updated successfully.\n";
            break;
        }

        case 3: { // Delete User
            const auto& users = lib.getUsers();
            if (users.empty()) {
                cout << "No users to delete.\n";
                break;
            }

            cout << "\n--- Registered Users ---\n";
            for (size_t i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i].first << " " << users[i].last << "\n";
            }

            int delUserIndex;
            cout << "Enter the number of the user to delete (0 to cancel): ";
            while (!(cin >> delUserIndex) || delUserIndex < 0 || delUserIndex > static_cast<int>(users.size())) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid user number: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (delUserIndex == 0) {
                cout << "Delete cancelled.\n";
                break;
            }

            lib.removeUser(delUserIndex - 1);
            cout << "User deleted successfully.\n";
            break;
        }

        case 4:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    }


    void editInventory(Library& lib, Terminal& term) {
        cout << "\n--- Inventory Menu ---\n";
        cout << "1. Add New Item\n";
        cout << "2. Delete Item\n";
        cout << "3. Edit Item\n";
        cout << "4. Back to Admin Menu\n";
        cout << "Enter your choice: ";
        int choice = term.promptForInput<int>("Enter your choice");

        switch (choice) {
        case 1: {
            string type = term.promptForInput<string>("Enter type");
            string name = term.promptForInput<string>("Enter name/title");
            string author = term.promptForInput<string>("Enter author");
            string publisher = term.promptForInput<string>("Enter publisher");
            string borrowed = "-1"; 

            lib.addInventory(std::move(type), std::move(name), std::move(author), std::move(publisher), std::move(borrowed));
            cout << "Item added successfully!\n";
            break;
        }
        case 2: {
            auto items = lib.allInventory();

            if (items.size() == 0) {
                cout << "No items in inventory to delete.\n";
                break;
            }

            cout << "\n--- Current Inventory ---\n";
            for (size_t i = 0; i < items.size(); ++i) {
                cout << i + 1 << ". " << items[i].name << " by " << items[i].author << "\n";
            }

            int delIndex = term.promptForInput<int>(
                "Enter the number of the item to delete (0 to cancel)"
            );
            if (delIndex == 0) {
                cout << "Delete cancelled.\n";
                break;
            }

            items.remove(delIndex - 1);
            cout << "Item deleted successfully.\n";
            break;
        }
        case 3: {
            // Edit Item
            auto items = lib.allInventory();

            if (items.size() == 0) {
                cout << "No items in inventory to edit.\n";
                break;
            }

            cout << "\n--- Current Inventory ---\n";
            for (size_t i = 0; i < items.size(); ++i) {
                cout << i + 1 << ". " << items[i].name << " by " << items[i].author << "\n";
            }

            int editIndex = term.promptForInput<int>(
                "Enter the number of the item to edit (0 to cancel)"
            );
            while (true) {
                if (editIndex == 0) {
                    cout << "Edit cancelled.\n";
                    return;
                } else if (editIndex > 0 && editIndex <= items.size()) {
                    break;
                }
                editIndex = term.promptForInput<int>(
                    "Invalid choice. Enter a valid item number"
                );
            }

            // Prompt for new values
            string newType = term.promptForInput<string>("Enter new type");
            string newName = term.promptForInput<string>("Enter new name/title");
            string newAuthor = term.promptForInput<string>("Enter new author");
            string newPublisher = term.promptForInput<string>("Enter new publisher");

            // Apply updates
            items[editIndex - 1].type = std::move(newType);
            items[editIndex - 1].name = std::move(newName);
            items[editIndex - 1].author = std::move(newAuthor);
            items[editIndex - 1].publisher = std::move(newPublisher);
            cout << "Item updated successfully.\n";
            break;
        }
        case 4:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    }


}; // end of admin class
