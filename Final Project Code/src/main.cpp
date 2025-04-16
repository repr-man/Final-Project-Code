 // Final Project Code.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <filesystem>
#include "terminal.hpp"
#include "RegisterUser.hpp"
#include "SearchFunction.hpp"
#include "UserLogin.hpp"
#include "Admin.hpp"

using namespace std;

int main()
{
    Terminal term = Terminal();
    int choice;
    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Register New User\n2. Admin Login\n3. User Login\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {// register new users
            static int nextID = 1; // Replace with a function that finds next ID if needed
            RegisterUser newUser;
            newUser.promptUserData(nextID++);
            newUser.printSummary();
            newUser.saveToFile("Final Project Code/data/users.txt");
            break;
        }//end of case 1
        case 2: { // Admin login (librarian) 

            string inputFirst, inputLast, inputPass;
            cout << "Enter admin first name: ";
            getline(cin >> ws, inputFirst);

            cout << "Enter admin last name: ";
            getline(cin >> ws, inputLast);

            cout << "Enter admin password: ";
            getline(cin >> ws, inputPass);

            ifstream adminFile("Final Project Code/data/librarians.txt"); 
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
                admin.showMenu();
            }
            else {
                cout << "Invalid admin credentials.\n";
            }

            break;
        }//end of case 2

        case 3: { // User Login using User ID
            string inputID, inputPassword;
            cout << "Enter your User ID (10 digits): ";
            cin >> inputID;
            cout << "Enter your password: ";
            cin >> inputPassword;

            ifstream userFile("Final Project Code/data/users.txt");
            if (!userFile) {
                cerr << "Error: Could not open users.txt.\n";
                break;
            }

            string line;
            bool authenticated = false;

            while (getline(userFile, line)) {
                stringstream ss(line);
                string libraryID, userType, firstName, lastName, address, phone, email, password, schoolID, isActive;

                getline(ss, libraryID, ';');
                getline(ss, userType, ';');
                getline(ss, firstName, ';');
                getline(ss, lastName, ';');
                getline(ss, address, ';');
                getline(ss, phone, ';');
                getline(ss, email, ';');
                getline(ss, password, ';');
                getline(ss, schoolID, ';');
                getline(ss, isActive);

                if (inputID == libraryID && inputPassword == password && isActive == "1") {
                    authenticated = true;
                    break;
                }
            }

            userFile.close();

            if (authenticated) {
                cout << "User login successful.\n";
                UserLogin u(inputID, inputPassword);
                u.showMenu();
            }
            else {
                cout << "Invalid User ID or password.\n";
            }

            break;
        }// end of case 3

        case 4: //Exit Program
            cout << "Exiting system.\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;

} // end of main

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
