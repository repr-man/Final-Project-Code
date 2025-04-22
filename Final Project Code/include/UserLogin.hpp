#pragma once
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <filesystem>


using namespace std;

class UserLogin {
private:
    string userID, password;

public:
    UserLogin(string id, string pass) : userID(id), password(pass) {}

    static void attemptLogin() {
        string inputID, inputPassword;

        cout << "\n--- User Login ---\n";
        cout << "Enter your Library ID (10 digits): ";
        getline(cin >> ws, inputID);

        cout << "Enter your password: ";
        getline(cin >> ws, inputPassword);

        ifstream userFile("data/users.txt");
        if (!userFile) {
            cerr << "Error: Could not open users.txt.\n";
            return;
        }

        string line;
        bool authenticated = false;

        while (getline(userFile, line)) {
            stringstream ss(line);
            string libraryID, userType, firstName, lastName, address, phone, email, storedPassword, schoolID, booksBorrowed;

            getline(ss, libraryID, ';');
            getline(ss, userType, ';');
            getline(ss, firstName, ';');
            getline(ss, lastName, ';');
            getline(ss, address, ';');
            getline(ss, phone, ';');
            getline(ss, email, ';');
            getline(ss, storedPassword, ';');
            getline(ss, schoolID, ';');
            getline(ss, booksBorrowed); 

            if (inputID == libraryID && inputPassword == storedPassword) {
                authenticated = true;
                UserLogin u(inputID, inputPassword);
                cout << "User login successful.\n";
                u.showMenu();
                break;
            }
        }

        userFile.close();

        if (!authenticated) {
            cout << "Invalid Library ID or password.\n";
        }
    }

    void printUserSummary() {
        ifstream userFile("data/users.txt");
        if (!userFile) {
            cout << "Could not open users.txt\n";
            return;
        }

        string line;
        while (getline(userFile, line)) {
            stringstream ss(line);
            string id, userType, firstName, lastName, address, phone, email, storedPassword, schoolID, booksBorrowed;

            getline(ss, id, ';');
            getline(ss, userType, ';');
            getline(ss, firstName, ';');
            getline(ss, lastName, ';');
            getline(ss, address, ';');
            getline(ss, phone, ';');
            getline(ss, email, ';');
            getline(ss, storedPassword, ';');
            getline(ss, schoolID, ';');
            getline(ss, booksBorrowed); 

            if (id == userID) {
                cout << "\n--- User Summary ---\n";
                cout << "User ID: " << id << "\n";
                cout << "Name: " << firstName << " " << lastName << "\n";
                cout << "User Type: " << userType << "\n";
                cout << "Address: " << address << "\n";
                cout << "Phone: " << phone << "\n";
                cout << "Email: " << email << "\n";
                cout << "School ID: " << schoolID << "\n";
                cout << "Books Borrowed: " << booksBorrowed << "\n";
                return;
            }
        }

        cout << "User not found in records.\n";
    }

    void showMenu() {
        int choice;
        while (true) {
            cout << "\n--- User Menu ---\n";
            cout << "1. View Available Books\n";
            cout << "2. Search Function\n";
            cout << "3. Print User Summary\n";
            cout << "4. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Displaying available books...\n";
                break;
            case 2:
                cout << "Search function not yet implemented.\n";
                break;
            case 3:
                printUserSummary();
                break;
            case 4:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
}; // end of class UserLogin