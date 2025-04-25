#include <cstdio>
#include "main.hpp"
#include "terminal.hpp"
#include "RegisterUser.hpp"
#include "SearchFunction.hpp"
#include "Borrowing.hpp"
#include "UserLogin.hpp"
#include "Admin.hpp"

using namespace std;

void Main::safeExit() {
    throw SafeExit();
}

void Main::main() try {
    Library lib;
    Terminal term = Terminal();
    int choice;
    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Register New User\n2. Admin Login\n3. User Login\n4. Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush buffer
            cout << "Invalid input. Please enter a number.\n";
            continue; // Prompt again
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline after valid input

        switch (choice) {
            case 1: {// register new users
                int nextID = RegisterUser::generateNextLibraryID(); 
                RegisterUser newUser;
                newUser.promptUserData(nextID);
                newUser.saveToFile();
                newUser.printSummary();
                break;
            }//end of case 1

            case 2:  {  // Admin login (librarian) 
                cout << "\n--- Admin Login ---\n";
                string inputFirst = term.promptForInput<string>("Enter admin first name");
                string inputLast = term.promptForInput<string>("Enter admin last name");
                string inputPass = term.promptForInput<string>("Enter admin password");

                auto res = lib.search(
                    {Librarian::First, Librarian::Last, Librarian::Password},
                    {inputFirst, inputLast, inputPass}
                );
                if (res.size() == 0) {
                    cout << "Invalid admin credentials.\n";
                    continue;
                }

                cout << "Admin login successful.\n";
                Admin admin(inputFirst + " " + inputLast, inputPass);
                admin.showMenu(lib, term);
                break;
            }//end of case 2

            case 3: { // User Login using User ID
                UserLogin::attemptLogin();
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
