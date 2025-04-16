#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <filesystem>



using namespace std;

class Admin {
private:
    string username, password;
public:
    Admin(string user, string pass) : username(user), password(pass) {}

    bool login(string inputUser, string inputPass) {
        return (inputUser == username && inputPass == password);
    }

    void showMenu() {
        int choice;
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Register New User\n";
            cout << "2. Edit Inventory\n";
            cout << "3. Edit User Information\n";
            cout << "4. Help User Borrow a book\n";
            cout << "5. Search Function\n";
            cout << "6. Active Users\n";
            cout << "7. Return Book\n";
            cout << "8. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Registering new user...";
                // Add function call here
                break;
            case 2:
                cout << "Editing Inventory...\n";
                // Add function call here
                break;
            case 3:
                cout << "Editing User Information...\n";
                // Add function call here
                break;
            case 4: /* {
                cout << "Borrowing books...\n";
                Borrowing b;
                b.borrowBook();

                break;
            }// end of case 4
            */
                break;
            case 5: // this sends them to the SearchFunction Class

                break;

            case 6:
                cout << "Viewing all registered users...\n";
                // add function call here
                break;
            case 7:
                cout << "Returning books...\n";
                // add function call here
                break;
            case 8:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
}; // end of admin class

