#pragma once

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include<terminal.hpp>

#include "library.hpp"

using namespace std; 

class Return {
public:
    void ReturnBook(Terminal& term) {
        string username, bookTitle;
        cout << "Enter User Name (First and Last): ";
        username = term.promptForInput<string>();

        ifstream userFile("Final Project Code/data/users.txt");
        if (!userFile) {
            cout << "Could not open users.txt\n";
            return;
        }

        bool userFound = false;
        string userLine;
        string userID; 
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

            string fullName = firstName + " " + lastName;
            if (fullName == username) {
                userFound = true;
                userID = id;
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

        ifstream booksFile("Final Project Code/data/book.txt");
        if (!booksFile) {
            cout << "Could not open book.txt\n";
            return;
        }

        vector<string> bookLines;
        string bookLine;
        bool found = false;

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
                    // Update the book as returned
                    bookLine = type + ";" + title + ";" + author + ";" + publisher + ";-1";
                    found = true;
                }
            }
            bookLines.push_back(bookLine);
        }
        booksFile.close();

        if (!found) {
            cout << "Book not found or not borrowed by this user.\n";
            return;
        }

        ofstream outFile("Final Project Code/data/book.txt");
        for (const string& line : bookLines) {
            outFile << line << "\n";
        }
        outFile.close();

        cout << "Book successfully returned.\n";
    } // end of Class Return

