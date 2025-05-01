#include <cstdio>
#include <iostream>
#include "main.hpp"
#include "terminal.hpp"
#include "util.hpp"
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
                int nextID = generateNextLibraryID(lib); 
                RegisterUser newUser;
                newUser.promptUserData(term, nextID);
                newUser.saveToFile(lib);
                newUser.printSummary();
                break;
            }//end of case 1

            case 2:  {  // Admin login (librarian) 
                cout << "\n--- Admin Login ---\n";
                string inputFirst = term.promptForInput<string>("Enter admin first name");
                string inputLast = term.promptForInput<string>("Enter admin last name");
                string inputPass = term.promptForInput<string>("Enter admin password");
                auto res = lib.search(
                    {User::First, User::Last, User::Password},
                    {inputFirst, inputLast, inputPass}
                );

                if (res.size() > 0) {
                    cout << "Admin login successful.\n";
                    string fullName = inputFirst + " " + inputLast;
                    Admin admin(lib, term, fullName, inputPass);
                    admin.showMenu();
                }
                else {
                    term.printError("Invalid admin credentials.");
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
        }
    }
} catch (const Main::SafeExit&) {}

int main() {
    Main main;
    main.main();
}
