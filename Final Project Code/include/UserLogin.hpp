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
    string username, password;

public:
    UserLogin(string user, string pass) : username(user), password(pass) {}

    bool login(string inputUser, string inputPass) {
        return (inputUser == username && inputPass == password);
    }

    void showMenu() {
        int choice;
        while (true) {
            cout << "\n--- User Menu ---\n";
            cout << "1. View Available Books\n";
            cout << "2. Search Function\n";
            cout << "3. Print User Summary\n";
            cout << "4. Edit User Information\n";
            cout << "5. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Displaying available books...\n";

                break;
            case 2: // This sends them to the Search Function Class

                break;

            case 3:
                cout << "User Summary...\n";

                break;
            case 4:
                cout << "Edit User Information...\n";
                break;
            case 5:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
}; // end of class UserLogin