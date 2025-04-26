#pragma once

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include "inventoryitem.hpp"
#include "terminal.hpp"

#include "library.hpp"
#include "user.hpp"

using namespace std;
/* When a user returns a book subtract one to the borrowedbooks count */
class Return {
public:
    void ReturnBook(Terminal& term) {
        string username, bookTitle;
        cout << "Enter User Name (First and Last): ";
        username = term.promptForInput<string>();

        ifstream userFile(User::SaveFileLocation);
        if (!userFile) {
            cout << "Could not open users.txt\n";
            return;
        }

        vector<string> userLines;
        bool userFound = false;
        string userID;
        int userIndex = -1;
        int booksBorrowed = 0;

        string userLine;
        while (getline(userFile, userLine)) {
            userLines.push_back(userLine);
            stringstream ss(userLine);
            string id, userType, firstName, lastName, address, phone, email, password, schoolID, borrowedCount;

            getline(ss, id, ';');
            getline(ss, userType, ';');
            getline(ss, firstName, ';');
            getline(ss, lastName, ';');
            getline(ss, address, ';');
            getline(ss, phone, ';');
            getline(ss, email, ';');
            getline(ss, password, ';');
            getline(ss, schoolID, ';');
            getline(ss, borrowedCount, ';');

            string fullName = firstName + " " + lastName;
            if (fullName == username) {
                userFound = true;
                userID = id;
                userIndex = userLines.size() - 1;
                booksBorrowed = stoi(borrowedCount);
                cout << "\nUser Found:\n";
                cout << "ID: " << id << "\n";
                cout << "Name: " << firstName << " " << lastName << "\n";
                cout << "Email: " << email << "\n";
                cout << "Phone: " << phone << "\n";
                cout << "User Type: " << userType << "\n";
                break;
            }
        }
        userFile.close();

        if (!userFound) {
            cout << "User not found in system.\n";
            return;
        }

        cout << "Enter the title of the book to return: ";
        bookTitle = term.promptForInput<string>();

        ifstream booksFile(InventoryItem::SaveFileLocation);
        if (!booksFile) {
            cout << "Could not open book.txt\n";
            return;
        }

        vector<string> bookLines;
        string bookLine;
        bool returned = false;

        while (getline(booksFile, bookLine)) {
            stringstream ss(bookLine);
            string type, title, author, publisher, borrowedBy;

            getline(ss, type, ';');
            getline(ss, title, ';');
            getline(ss, author, ';');
            getline(ss, publisher, ';');
            getline(ss, borrowedBy);

            if (title == bookTitle) {
                if (borrowedBy == "-1") {
                    cout << "Book is not currently borrowed.\n";
                }
                else if (borrowedBy != userID) {
                    cout << "This book was borrowed by a different user.\n";
                }
                else {
                    // Mark the book as returned
                    bookLine = type + ";" + title + ";" + author + ";" + publisher + ";-1";
                    returned = true;
                }
            }

            bookLines.push_back(bookLine);
        }
        booksFile.close();

        if (!returned) {
            cout << "Book not found or not borrowed by this user.\n";
            return;
        }

        // Rewrite book file
        ofstream outFile(InventoryItem::SaveFileLocation);
        for (const string& line : bookLines) {
            outFile << line << "\n";
        }
        outFile.close();

        // Subtract one from booksBorrowed and update users file
        if (userIndex != -1 && booksBorrowed > 0) {
            stringstream ss(userLines[userIndex]);
            string id, userType, firstName, lastName, address, phone, email, password, schoolID, borrowedCount;
            getline(ss, id, ';');
            getline(ss, userType, ';');
            getline(ss, firstName, ';');
            getline(ss, lastName, ';');
            getline(ss, address, ';');
            getline(ss, phone, ';');
            getline(ss, email, ';');
            getline(ss, password, ';');
            getline(ss, schoolID, ';');
            getline(ss, borrowedCount, ';');

            booksBorrowed -= 1;

            userLines[userIndex] = id + ";" + userType + ";" + firstName + ";" + lastName + ";" + address + ";" +
                phone + ";" + email + ";" + password + ";" + schoolID + ";" + to_string(booksBorrowed);
        }

        ofstream outUsers(User::SaveFileLocation);
        for (const string& updatedUser : userLines) {
            outUsers << updatedUser << "\n";
        }
        outUsers.close();

        cout << "Book successfully returned. Updated borrowed book count: " << booksBorrowed << "\n";
    }
};
