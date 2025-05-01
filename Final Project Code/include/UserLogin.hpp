#pragma once

#include "SearchFunction.hpp"
#include "inventoryitem.hpp"
#include "library.hpp"
#include "terminal.hpp"
#include "user.hpp"
#include "util.hpp"
#include "validators.hpp"
#include <cstdio>
#include <iostream>
#include <string>


using namespace std;

class UserLogin {
private:
    Library& lib;
    Terminal& term;
    string userID, password;

public:
    UserLogin(Library& lib, Terminal& term, string id, string pass) : lib(lib), term(term), userID(id), password(pass) {}

    static void attemptLogin(Library& lib, Terminal& term) {
        string inputID, inputPassword;

        while (true) {
            cout << "\n--- User Login ---\n";
            inputID = to_string(term.promptForInput<long, validateLibraryID>(
                "Enter your Library ID (10 digits)"
            ));
            inputPassword = term.promptForInput<string>("Enter your password");
            auto res = lib.search({User::ID, User::Password}, {inputID, inputPassword});
            if (res.size() == 0) {
                cout << "Invalid Library ID or password.\n";
                continue;
            }
            break;
        }

        UserLogin u(lib, term, inputID, inputPassword);
        cout << "User login successful.\n";
        u.showMenu();
    }

    void printUserSummary() {
        const auto res = lib.search({User::ID}, {userID});
        if (res.size() == 0) {
            cout << "User not found in records.\n";
            return;
        }

        cout << "\n--- User Summary ---\n";
        cout << "User ID: " << res[0].id << "\n";
        cout << "Name: " << res[0].first << " " << res[0].last << "\n";
        cout << "User Type: " << res[0].role << "\n";
        cout << "Address: " << res[0].address << "\n";
        cout << "Phone: " << res[0].phone << "\n";
        cout << "Email: " << res[0].email << "\n";
        cout << "School ID: " << res[0].institutionId << "\n";
        cout << "Books Borrowed: " << res[0].numCheckedOut << "\n";

        const auto books = lib.search({InventoryItem::BorrowerID}, {userID});
        if (books.size() != 0) {
            term.printTable(books, "Type", "Name", "Author", "Publisher", "Borrower ID");
        }
    }

    void showMenu() {
        while (true) {
            cout << "\n--- User Menu ---\n";
            cout << "1. View Available Books\n";
            cout << "2. Search Function\n";
            cout << "3. Print User Summary\n";
            cout << "4. Logout\n";
            int choice = term.promptForInput<int, validateNumRange<1, 4>>("Enter your choice");

            switch (choice) {
            case 1:
                cout << "Displaying available books...\n";
                term.printTable(
                    lib.allInventory(),
                    "Type", "Name", "Author", "Publisher", "Borrower ID"
                );
                break;
            case 2: {
                const auto res = SearchFunction().searchInventory(lib, term);
                if (res.size() == 0) {
                    cout << "No results found.\n";
                    break;
                }
                term.printTable(res, "Type", "Name", "Author", "Publisher", "Borrower ID");
                break;
            }
            case 3:
                printUserSummary();
                break;
            case 4:
                cout << "Logging out...\n";
                return;
            default:
                UNREACHABLE;
            }
        }
    }
};
