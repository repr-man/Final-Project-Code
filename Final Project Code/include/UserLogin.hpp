#pragma once

#include "SearchFunction.hpp"
#include "inventoryitem.hpp"
#include "library.hpp"
#include "terminal.hpp"
#include "user.hpp"
#include "util.hpp"
#include "validators.hpp"
#include "history.hpp"

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
            using enum InventoryItem::FieldTag;
            term.printTable(books, Type, Name, Author, Publisher);
        }
    }

    void showMenu() {
        while (true) {

            cout << "\n--- User Menu ---\n";
            cout << "1. View Available Books\n";
            cout << "2. View Borrowed Books\n";
            cout << "3. Search Function\n";
            cout << "4. Print User Summary\n";
            cout << "5. Logout\n";
            int choice = term.promptForInput<int, validateNumRange<1, 5>> ("Enter your choice");

            switch (choice) {
                case 1: {
                    using enum InventoryItem::FieldTag;
                    cout << "Displaying available books...\n";
                    term.printTable(
                        lib.allInventory(),
                        Type, Name, Author, Publisher
                    );
                    break;
                }
                case 2: {
                    using enum InventoryItem::FieldTag;

                    cout << "\n--- Currently Borrowed Books ---\n";
                    auto borrowed = lib.search({InventoryItem::BorrowerID}, {userID});
                    if (borrowed.size() == 0) {
                        cout << "No books currently borrowed.\n";
                    } else {
                        term.printTable(borrowed, Type, Name, Author, Publisher);
                    }

                    cout << "\n--- Borrowing History ---\n";
                    auto history = lib.search({HistoryItem::UserID}, {userID});
                    auto inventory = lib.allInventory();
                    if (history.size() == 0 || inventory.size() == 0) {
                        cout << "No borrowing history found.\n";
                    } else {
                        const auto res = inventory.join(Name, history, HistoryItem::Name);
                        term.printTable(res, Type, Name, Author, Publisher);
                    }
                    break;
                }


                case 3: {
                    using enum InventoryItem::FieldTag;
                    const auto res = SearchFunction().searchInventory(lib, term);
                    if (res.size() == 0) {
                        cout << "No results found.\n";
                        break;
                    }
                    term.printTable(res, Type, Name, Author, Publisher);
                    break;
                }
                case 4:
                    printUserSummary();
                    break;
                case 5:
                    cout << "Logging out...\n";
                    return;
                default:
                    UNREACHABLE;
            }
        }
    }
};
