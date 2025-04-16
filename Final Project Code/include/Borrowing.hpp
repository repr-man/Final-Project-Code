
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <filesystem>

using namespace std;

// Giving errors so it isn't included 
class Borrowing {
public:
    void borrowBook() {
        string userID, bookTitle;
        cout << "Enter User ID: ";
        cin >> userID;
        cin.ignore();

        // Count how many books this user has already borrowed
        ifstream booksFile("books.txt");
        if (!booksFile) {
            cout << "Could not open books.txt\n";
            return;
        }

        string line;
        int borrowedCount = 0;
        std::vector<string> bookLines;

        while (getline(booksFile, line)) {
            stringstream ss(line);
            string type, title, author, publisher, borrowedBy;

            getline(ss, type, ';');
            getline(ss, title, ';');
            getline(ss, author, ';');
            getline(ss, publisher, ';');
            getline(ss, borrowedBy);

            if (borrowedBy == userID) {
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
        getline(cin, bookTitle);

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
                    bookLine = type + ";" + title + ";" + author + ";" + publisher + ";" + userID;
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
        ofstream outFile("books.txt");
        for (const string& updatedLine : bookLines) {
            outFile << updatedLine << "\n";
        }
        outFile.close();

        cout << "Book successfully borrowed by User " << userID << ".\n";
    }
}; // end of class Borrowing 