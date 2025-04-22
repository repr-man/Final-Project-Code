
#pragma once

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include "library.hpp"

using namespace std;

class Borrowing {
public:

    void borrowBook(Terminal &term) {
        string username, bookTitle;
        cout << "Enter User Name (First and Last): ";
        username = term.promptForInput<string>();

        // ensure that the user.txt files opens to check if the user exits
        ifstream userFile("Final Project Code/data/users.txt");
        if (!userFile) {
            cout << "Could not open users.txt\n";
            return;
        }
        bool userFound = false;
        string userLine;
        while (getline(userFile, userLine)) {
            stringstream ss(userLine);
            string id, userType, firstName, lastName, address, phone, email, password, schoolID, booksBorrowed;

            getline(ss, id, ';');
            getline(ss, userType, ';');
            getline(ss, firstName, ';');
            getline(ss, lastName, ';');
            getline(ss, address, ';');
            getline(ss, phone, ';');
            getline(ss, email, ';');
            getline(ss, password, ';');
            getline(ss, schoolID, ';');
            getline(ss, booksBorrowed, ';');

            string fullName = firstName + lastName;
            if (fullName == username) {
                userFound = true;
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

        // make sure that book.txt opens
        ifstream booksFile("Final Project/data/book.txt");
        if (!booksFile) {
            cout << "Could not open book.txt\n";
            return;
        }

        string line;
        int borrowedCount = 0;
        vector<string> bookLines;

        while (getline(booksFile, line)) {
            stringstream ss(line);
            string type, title, author, publisher, borrowedBy;

            getline(ss, type, ';');
            getline(ss, title, ';');
            getline(ss, author, ';');
            getline(ss, publisher, ';');
            getline(ss, borrowedBy);

            if (borrowedBy == username) {
                borrowedCount++;
            }

            bookLines.push_back(line);
        }

        booksFile.close();

        if (borrowedCount >= 4) {
            cout << "This user has already borrowed 4 books.\n";
            return;
        }

        cout << "Enter the title of the book to borrow: ";
        bookTitle = term.promptForInput<string>();

        // Try to find and update the book in memory
        bool found = false;
        for (string& bookLine : bookLines) {
            stringstream ss(bookLine);
            string type, title, author, publisher, borrowedBy;

            getline(ss, type, ';');
            getline(ss, title, ';');
            getline(ss, author, ';');
            getline(ss, publisher, ';');
            getline(ss, borrowedBy);

            if (title == bookTitle) {
                if (borrowedBy != "-1") {
                    cout << "Book is already borrowed by another user.\n";
                    return;
                }
                else {
                    // Update the line with the new userID
                    bookLine = type + ";" + title + ";" + author + ";" + publisher + ";" + username;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            cout << "Book not found.\n";
            return;
        }

        // Rewrite books.txt with updated data
        ofstream outFile("Final Project Code/data/book.txt");
        for (const string& updatedLine : bookLines) {
            outFile << updatedLine << "\n";
        }
        outFile.close();

        cout << "Book successfully borrowed by User " << username << ".\n";
    }
}; // end of class Borrowing 