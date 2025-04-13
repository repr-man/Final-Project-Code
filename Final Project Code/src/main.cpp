 // Final Project Code.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class RegisterUser {
public: 
    string firstName, lastName, address, phone, email, password, userType;
    int libraryID;
    RegisterUser(string fn, string ln, string addr, string ph, string em, string pass, string type, int id)
        : firstName(fn), lastName(ln), address(addr), phone(ph), email(em), password(pass), userType(type), libraryID(id) {
    }
}; // end of class RegisterUser

class Admin {
private:
    string username, password;
public:
    Admin(string user, string pass) : username(user), password(pass) {}

}; // end of class Admin

class Borrowing {

}; // end of class Borrowing 


class UserLogin {

}; // end of class UserLogin

class SearchFunction {

}; // end of class SearchFunction


int main()
{
    int choice;
    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Register New User\n2. Admin Login\n3. User Login\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: // register new users
            
            break;
        case 2: // Admin (Librarian) Login
            
            break;
        case 3: // User Login
            break;
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
