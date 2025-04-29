#include <cstdio>
#include <iostream>
#include "main.hpp"
#include "terminal.hpp"
#include "RegisterUser.hpp"
#include "SearchFunction.hpp"
#include "Borrowing.hpp"
#include "UserLogin.hpp"
#include "Admin.hpp"

using namespace std;

void Main::safeExit() {
    std::cout << std::endl;
    throw SafeExit();
}

void Main::main() try {
    Library lib;
    Terminal term = Terminal();
    int choice;
    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Register New User\n2. Admin Login\n3. User Login\n4. Exit\n";
        choice = term.promptForInput<int, validateNumRange<1, 4>>("Enter your choice");

        switch (choice) {
            case 1: {// register new users
                int nextID = RegisterUser::generateNextLibraryID(); 
                RegisterUser newUser;
                newUser.promptUserData(term, nextID);
                newUser.saveToFile(lib);
                newUser.printSummary();
                break;
            }//end of case 1

            case 2:  {  // Admin login (librarian) 
                cout << "\n--- Admin Login ---\n";
                string inputFirst, inputLast, inputPass;
                cout << "Enter admin first name: ";
                getline(cin >> ws, inputFirst);

                cout << "Enter admin last name: ";
                getline(cin >> ws, inputLast);

                cout << "Enter admin password: ";
                getline(cin >> ws, inputPass);

                ifstream adminFile(Librarian::SaveFileLocation);
                if (!adminFile) {
                    cerr << "Failed to open librarian file. Check the path.\n";
                    break;
                }

                string line;
                bool authenticated = false;

                while (getline(adminFile, line)) {
                    stringstream ss(line);
                    string first, last, storedPass;

                    getline(ss, first, ';');
                    getline(ss, last, ';');
                    getline(ss, storedPass);

                    if (inputFirst == first && inputLast == last && inputPass == storedPass) {
                        authenticated = true;
                        break;
                    }
                }

                if (authenticated) {
                    cout << "Admin login successful.\n";
                    string fullName = inputFirst + " " + inputLast;
                    Admin admin(fullName, inputPass);
                    admin.showMenu(lib, term);
                }
                else {
                    cout << "Invalid admin credentials.\n";
                }

                break;
            }//end of case 2

            case 3: { // User Login using User ID
                UserLogin::attemptLogin(lib, term);
                break;
            }// end of case 3

            case 4: //Exit Program
                cout << "Exiting system.\n";
                Main::safeExit();
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
} catch (const Main::SafeExit&) {}

int main() {
    Main main;
    main.main();
}
