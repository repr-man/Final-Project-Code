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
#include "resultlist.hpp"
#include "terminal.hpp"
#include "util.hpp"
#include "validators.hpp"


using namespace std;

class Admin {
private:
    Library& lib;
    Terminal& term;
    string username, password;
public:
    Admin(Library& lib, Terminal& term, string user, string pass) : lib(lib), term(term), username(user), password(pass) {}

    bool login(string inputUser, string inputPass) {
        return (inputUser == username && inputPass == password);
    }

    void showMenu() {
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
            choice = term.promptForInput<int, validateNumRange<1, 7>>("Enter your choice");

            switch (choice) {
            case 1:
                cout << "Inventory Management...\n";
                editInventory();
                break;
            case 2:
                cout << "User Management...\n";
                editUserInfo();
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
                choice = term.promptForInput<int, validateNumRange<1, 3>>("Enter your choice");
                    
                SearchFunction s;
                switch (choice) {
                    case 1: {
                        auto res = s.searchUser(lib, term);
                        term.printTable(res, "ID", "Role", "First Name", "Last Name", "Address", "Phone Number", "Email", "Password", "Institution ID", "# Items Borrowed");
                        editUserInfoAfterSearch(res);
                        break;
                    }
                    case 2: {
                        auto res = s.searchInventory(lib, term);
                        term.printTable(res, "Type", "Name", "Author", "Publisher", "Borrower ID");
                        break;
                    }
                    case 3:
                        cout << "Returning to Admin Menu...\n";
                        return;
                    default:
                        UNREACHABLE;
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
                UNREACHABLE;
            }
        }
    }
private: 
    void registerNewUser() {
        int nextID = generateNextLibraryID(lib);  
        RegisterUser newUser;
        newUser.promptUserData(term, nextID); 
        newUser.printSummary();
        newUser.saveToFile(lib);
    }

    void editUserInfo() {
        cout << "\n--- User Management Menu ---\n";
        cout << "1. Register New User\n";
        cout << "2. Update User Information\n";
        cout << "3. Delete User\n";
        cout << "4. Back to Admin Menu\n";
        int choice = term.promptForInput<int, validateNumRange<1, 4>>("Enter your choice");

        switch (choice) {
        case 1: { // Register New User
            registerNewUser();
            cout << "New user registered successfully!\n";
            break;
        }
        case 2:{ // Update User
            auto users = lib.allUsers();
            if (users.size() == 0) {
                cout << "No users to update.\n";
                break;
            }

            cout << "\n--- Registered Users ---\n";
            for (size_t i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i].first << " " << users[i].last << "\n";
            }

            int userIndex = term.promptForInput<int>(
                "Enter the number of the user to update (0 to cancel)"
            );
            while (true) {
                if (userIndex == 0) {
                    cout << "Update cancelled.\n";
                    return;
                } else if (userIndex > 0 && userIndex <= users.size()) {
                    break;
                }
                userIndex = term.promptForInput<int>(
                    "Invalid choice. Enter a valid user number"
                );
            }

            updateUserInfo(users[userIndex - 1]);
            break;
        }

        case 3: { // Delete User
            auto users = lib.allUsers();
            if (users.size() == 0) {
                cout << "No users to delete.\n";
                break;
            }

            cout << "\n--- Registered Users ---\n";
            for (size_t i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i].first << " " << users[i].last << "\n";
            }

            int delUserIndex = term.promptForInput<int>(
                "Enter the number of the user to delete (0 to cancel)"
            );
            while (true) {
                if (delUserIndex == 0) {
                    cout << "Delete cancelled.\n";
                    return;
                } else if (delUserIndex > 0 && delUserIndex <= users.size()) {
                    break;
                }
                delUserIndex = term.promptForInput<int>(
                    "Invalid choice. Enter a valid user number"
                );
            }

            users.remove(delUserIndex - 1);
            cout << "User deleted successfully.\n";
            break;
        }

        case 4:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            UNREACHABLE;
        }
    }


    void editInventory() {
        cout << "\n--- Inventory Menu ---\n";
        cout << "1. Add New Item\n";
        cout << "2. Delete Item\n";
        cout << "3. Edit Item\n";
        cout << "4. Back to Admin Menu\n";
        int choice = term.promptForInput<int, validateNumRange<1, 4>>("Enter your choice");

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
            UNREACHABLE;
        }
    }

    void editUserInfoAfterSearch(ResultList<User>& res) {
        cout << "\n--- User Management Menu ---\n";
        cout << "1. Update User Information\n";
        cout << "2. Delete User\n";
        cout << "3. Back to Admin Menu\n";
        int choice = term.promptForInput<int, validateNumRange<1, 3>>("Enter your choice");

        int userIndex = term.promptForInput<int>(
            "Enter the number of the user to update (0 to cancel)"
        );
        while (true) {
            if (userIndex == 0) {
                return;
                cout << "Operation cancelled.\n";
            } else if (userIndex > 0 && userIndex <= res.size()) {
                break;
            }
            userIndex = term.promptForInput<int>(
                "Invalid choice. Enter a valid user number"
            );
        }

        switch (choice) {
        case 1:
            updateUserInfo(res[userIndex - 1]);
            break;
        case 2:
            res.remove(userIndex - 1);
            cout << "User deleted successfully.\n";
            break;
        case 3:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            UNREACHABLE;
        }
    }

    void updateUserInfo(User& user) {
        term.printOptions("--- Select Fields to Update ---", {
            "First Name",
            "Last Name",
            "Address",
            "Phone Number",
            "Email"
        });
        auto fields = term.promptForInput<
            vector<User::FieldTag>,
            validateNumRange<1, 5>
        >(
            "Enter space-separated list of fields to update"
        );

        for (auto& field : fields) {
            switch (field) {
                using enum User::FieldTag;
                case First:
                    user.first = term.promptForInput<string>("Enter new first name");
                    break;
                case Last:
                    user.last = term.promptForInput<string>("Enter new last name");
                    break;
                case Address:
                    user.address = term.promptForInput<string>("Enter new address");
                    break;
                case Phone:
                    user.phone = term.promptForInput<string, validatePhone>(
                        "Enter new phone number"
                    );
                    break;
                case Email:
                    user.email = term.promptForInput<string, validateEmail>("Enter new email");
                    break;
                default:
                    UNREACHABLE;
            }
        }

        cout << "User information updated successfully.\n";
    }


}; // end of admin class
