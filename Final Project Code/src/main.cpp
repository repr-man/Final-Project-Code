#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "terminal.hpp"
#include "RegisterUser.hpp"
#include "SearchFunction.hpp"
#include "Borrowing.hpp"
#include "UserLogin.hpp"
#include "Admin.hpp"
#include "inventoryitem.hpp"
#include "library.hpp"
#include "user.hpp"

using namespace std;

int main()
{
    Terminal term;
    Library lib;

    {
        std::vector<string> queries;

        cout << "Enter a first name:" << endl;
        string firstName = term.promptForInput<string>();
        queries.push_back(firstName);

        cout << "Enter a last name:" << endl;
        string lastName = term.promptForInput<string>();
        queries.push_back(lastName);

        auto results = lib.search(
            {User::First, User::Last},
            queries
        );

        cout << results[0].email << endl;

        cout << "Enter a new email:" << endl;
        string newEmail = term.promptForInput<string>();

        results[0].email = newEmail;
    }

    const auto results = lib.search(
        {User::First, User::Last},
        {"John", "Mandler"}
    );
    cout << endl << results[0].email << endl;





    //while (true) {
    //    cout << "\nLibrary Management System\n";
    //    term.printOptions("Library Management System", {
    //        "Register New User",
    //        "Admin Login",
    //        "User Login",
    //        "Exit"
    //    });
    //    auto choice = term.promptForInput<int>();

    //    switch (choice) {
    //        case 1: {// register new users
    //            static int nextID = 1; // Replace with a function that finds next ID if needed
    //            RegisterUser newUser;
    //            newUser.promptUserData(nextID++);
    //            newUser.printSummary();
    //            newUser.saveToFile("Final Project Code/data/users.txt");
    //            break;
    //        }//end of case 1
    //        case 2: { // Admin login (librarian) 
    //            string inputFirst, inputLast, inputPass;

    //            //do {
    //            //    cout << "Enter admin first name: " << endl;
    //            //    inputFirst = term.promptForInput<string>();

    //            //    cout << "Enter admin last name: " << endl;
    //            //    inputLast = term.promptForInput<string>();

    //            //} while ();
    //            cout << "Enter admin password: " << endl;
    //            inputPass = term.promptForInput<string>();

    //            string line;
    //            bool authenticated = false;
    //            std::ifstream adminFile("Final Project Code/data/librarians.txt");
    //            while (getline(adminFile, line)) {
    //                stringstream ss(line);
    //                string first, last, storedPass;

    //                getline(ss, first, ';');
    //                getline(ss, last, ';');
    //                getline(ss, storedPass);

    //                if (inputFirst == first && inputLast == last && inputPass == storedPass) {
    //                    authenticated = true;
    //                    break;
    //                }
    //            }

    //            if (authenticated) {
    //                cout << "Admin login successful.\n";
    //                string fullName = inputFirst + " " + inputLast;
    //                Admin admin(fullName, inputPass);
    //                admin.showMenu();
    //            }
    //            else {
    //                cout << "Invalid admin credentials.\n";
    //            }

    //            break;
    //        }//end of case 2

    //        case 3: { // User Login using User ID
    //            string inputID, inputPassword;
    //            cout << "Enter your User ID (10 digits): ";
    //            cin >> inputID;
    //            cout << "Enter your password: ";
    //            cin >> inputPassword;

    //            ifstream userFile("Final Project Code/data/users.txt");
    //            if (!userFile) {
    //                cerr << "Error: Could not open users.txt.\n";
    //                break;
    //            }

    //            string line;
    //            bool authenticated = false;

    //            while (getline(userFile, line)) {
    //                stringstream ss(line);
    //                string libraryID, userType, firstName, lastName, address, phone, email, password, schoolID, isActive;

    //                getline(ss, libraryID, ';');
    //                getline(ss, userType, ';');
    //                getline(ss, firstName, ';');
    //                getline(ss, lastName, ';');
    //                getline(ss, address, ';');
    //                getline(ss, phone, ';');
    //                getline(ss, email, ';');
    //                getline(ss, password, ';');
    //                getline(ss, schoolID, ';');
    //                getline(ss, isActive);

    //                if (inputID == libraryID && inputPassword == password && isActive == "1") {
    //                    authenticated = true;
    //                    break;
    //                }
    //            }

    //            userFile.close();

    //            if (authenticated) {
    //                cout << "User login successful.\n";
    //                UserLogin u(inputID, inputPassword);
    //                u.showMenu();
    //            }
    //            else {
    //                cout << "Invalid User ID or password.\n";
    //            }

    //            break;
    //        }// end of case 3

    //        case 4: //Exit Program
    //            cout << "Exiting system.\n";
    //            return 0;
    //        default:
    //            cout << "Invalid choice. Try again.\n";
    //    }
    //}
}
