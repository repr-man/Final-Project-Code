
#pragma once

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "terminal.hpp"

#include "library.hpp"

using namespace std;
/* When a user borrows a book add one to the borrowedbooks count */

class Borrowing {
public:
    void borrowBook(Terminal& term) {
        string username, bookTitle;
        cout << "Enter User Name (First and Last): ";
        username = term.promptForInput<string>();

        // Read users
        ifstream userFile("data/users.txt");
        if (!userFile) {
            cout << "Could not open users.txt\n";
            return;
        }

        vector<string> userLines;
        bool userFound = false;
        string userID;
        int userIndex = -1;
        int borrowedBooksCount = 0;

        string userLine;
        while (getline(userFile, userLine)) {
            userLines.push_back(userLine);
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

            string fullName = firstName + " " + lastName;
            if (fullName == username) {
                userFound = true;
                userID = id;
                userIndex = userLines.size() - 1;
                borrowedBooksCount = stoi(booksBorrowed);
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

        if (borrowedBooksCount >= 4) {
            cout << "This user has already borrowed 4 books.\n";
            return;
        }

        // Read books
        ifstream booksFile("data/book.txt");
        if (!booksFile) {
            cout << "Could not open book.txt\n";
            return;
        }

        vector<string> bookLines;
        string line;
        cout << "Enter the title of the book to borrow: ";
        bookTitle = term.promptForInput<string>();

        bool bookFound = false;
        for (; getline(booksFile, line); ) {
            stringstream ss(line);
            string type, title, author, publisher, borrowedBy;
            getline(ss, type, ';');
            getline(ss, title, ';');
            getline(ss, author, ';');
            getline(ss, publisher, ';');
            getline(ss, borrowedBy);

            if (title == bookTitle && borrowedBy == "-1") {
                // Mark as borrowed by user
                line = type + ";" + title + ";" + author + ";" + publisher + ";" + userID;
                bookFound = true;
            }
            else if (title == bookTitle && borrowedBy != "-1") {
                cout << "Book is already borrowed by another user.\n";
                booksFile.close();
                return;
            }

            bookLines.push_back(line);
        }
        booksFile.close();

        if (!bookFound) {
            cout << "Book not found.\n";
            return;
        }

        // Write updated books
        ofstream outBooks("data/book.txt");
        for (const string& updatedLine : bookLines) {
            outBooks << updatedLine << "\n";
        }
        outBooks.close();

        // Update borrowed book count in users.txt
        if (userIndex != -1) {
            stringstream ss(userLines[userIndex]);
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

            borrowedBooksCount += 1;
            userLines[userIndex] = id + ";" + userType + ";" + firstName + ";" + lastName + ";" + address + ";" +
                phone + ";" + email + ";" + password + ";" + schoolID + ";" + to_string(borrowedBooksCount);
        }

        // Write updated users
        ofstream outUsers("data/users.txt");
        for (const string& updatedUser : userLines) {
            outUsers << updatedUser << "\n";
        }
        outUsers.close();

        cout << "Book successfully borrowed by User " << userID << ". Total books borrowed: " << borrowedBooksCount << "\n";
    }
}; // end of class Borrowing
