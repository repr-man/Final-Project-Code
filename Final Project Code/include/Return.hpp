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
/* When a user returns a book subtract one to the borrowedbooks count */
class Return {
public:
    void ReturnBook(Library& lib, Terminal& term) {
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

        string bookTitle = term.promptForInput<string>("Enter the title of the book to return");
        auto book = lib.search({InventoryItem::Name}, {bookTitle});
        if (book.size() == 0) {
            term.printError("Book not found or not borrowed by this user.");
            return;
        }

        if (book[0].borrowerID == -1) {
            term.printError("Book is not currently borrowed.");
            return;
        } else if (book[0].borrowerID != user[0].id) {
            term.printError("This book was borrowed by a different user.");
            return;
        }

        book[0].borrowerID = -1;
        user[0].numCheckedOut -= 1;

        cout << "Book successfully returned. Updated borrowed book count: " << user[0].numCheckedOut << "\n";
    }
};
