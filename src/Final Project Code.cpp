// Final Project Code.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

#include "terminal.hpp"

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


int main() {
    Terminal terminal;

    int choice;
    while (true) {
        std::cout << "\n1. Thing 1\n2. Thing 2\n3. Thing 3\n4. Thing 4\n5. Thing 5" << std::endl;
        choice = terminal.promptForInput<int>();

        switch (choice) {
        case 1:
            std::cout << "Thing 1" << std::endl;
            break;
        case 2:
            std::cout << "Thing 2" << std::endl;
            break;
        case 3:
            std::cout << "Thing 3" << std::endl;
            break;
        case 4:
            std::cout << "Thing 4" << std::endl;
            break;
        case 5:
            std::cout << "Thing 5" << std::endl;
            break;
                    
        default:
            return 0;
        }
    }
}
