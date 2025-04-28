
#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "inventoryitem.hpp"
#include "terminal.hpp"

#include "library.hpp"
#include "user.hpp"

using namespace std;
/* When a user borrows a book add one to the borrowedbooks count */

class Borrowing {
public:
    void borrowBook(Library& lib, Terminal& term) {
        string firstName = term.promptForInput<string>("Enter First Name");
        string lastName = term.promptForInput<string>("Enter Last Name");
        auto user = lib.search({User::First, User::Last}, {firstName, lastName});
        if (user.size() == 0) {
            term.printError("User not found in system.");
            return;
        }

        cout << "\nUser Found:\n";
        cout << "ID: " << user[0].id << "\n";
        cout << "Name: " << user[0].first << " " << user[0].last << "\n";
        cout << "Email: " << user[0].email << "\n";
        cout << "Phone: " << user[0].phone << "\n";
        cout << "User Type: " << user[0].role << "\n";

        if (user[0].numCheckedOut >= 4) {
            cout << "This user has already borrowed 4 books.\n";
            return;
        }

        auto bookTitle = term.promptForInput<string>("Enter the title of the book to borrow");
        auto item = lib.search({InventoryItem::Name}, {bookTitle});
        if (item.size() == 0) {
            term.printError("Book not found.");
            return;
        } else if (item[0].borrowerID != -1) {
            term.printError("Book is already borrowed by another user.");
            return;
        }

        user[0].numCheckedOut += 1;
        item[0].borrowerID = user[0].id;
        lib.addHistory(user[0].id, std::move(bookTitle));

        cout << "Book successfully borrowed by User " << user[0].id << ". Total books borrowed: " << user[0].numCheckedOut << "\n";
    }
}; // end of class Borrowing
