 // Final Project Code.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "terminal.hpp"

using namespace std;

class RegisterUser { //hacving a problem opening user.txt for writting but otherwise the code runs
public:
    string firstName, lastName, address, phone, email, password, userType;
    string schoolID; 
    int libraryID;
    bool isActive = true;  

    RegisterUser() {}

    void promptUserData(int nextID) {
        libraryID = nextID;

        cout << "Enter User Type (e.g. student, faculty): ";
        std::getline(cin >> std::ws, userType);

        cout << "Enter First Name: ";
        std::getline(cin, firstName);

        cout << "Enter Last Name: ";
        std::getline(cin, lastName);

        cout << "Enter Address: ";
        std::getline(cin, address);

        cout << "Enter Phone Number: ";
        std::getline(cin, phone);

        cout << "Enter Email: ";
        std::getline(cin, email);

        cout << "Enter Password: ";
        std::getline(cin, password);

        cout << "Enter School ID (e.g. campus ID): ";
        std::getline(cin, schoolID);
    }

    void printSummary() const {
        cout << "\nUser Registered Successfully:\n";
        cout << "ID: " << setw(10) << setfill('0') << libraryID << "\n";
        cout << "Name: " << firstName << " " << lastName << "\n";
        cout << "Address: " << address << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Email: " << email << "\n";
        cout << "User Type: " << userType << "\n";
        cout << "School ID: " << schoolID << "\n";
    }

    void saveToFile(const string& filename = "Final Project Code Take2/data/users.txt") const {
        ofstream outFile(filename, ios::app); // append mode
        if (!outFile) {
            cerr << "Error: Could not open " << filename << " for writing.\n";
            return;
        }

        // Format libraryID with leading zeroes (10 digits)
        outFile << setw(10) << setfill('0') << libraryID << ";"
            << userType << ";"
            << firstName << ";"
            << lastName << ";"
            << address << ";"
            << phone << ";"
            << email << ";"
            << password << ";"
            << schoolID << ";"
            << isActive << "\n";

        outFile.close();
    }
}; // end of class RegisterUser

class Admin {
private:
    string username, password;
public:
    Admin(string user, string pass) : username(user), password(pass) {}

    bool login(string inputUser, string inputPass) {
        return (inputUser == username && inputPass == password);
    }

    void showMenu() {
        Terminal t;
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
                cout << "";
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
            case 4:
                cout << "Borrowing books...\n";
                // Add function call here
                break;
            case 5: /* {// this sends them to the SearchFunction Class
                cout << "Searching...\n";
                SearchFunction sf;
                sf.searchBooks();
                break;
            }*/
           
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

class Borrowing {

}; // end of class Borrowing 


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
            case 2: /* {// This sends them to the Search Function Class
                cout << "Searching...\n";
                SearchFunction sf;
                sf.searchBooks();
                break;
            } */
            
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

class SearchFunction { /*We need to add a part where the user can use this class to search for other Users*/
public: /*
        void searchBooks() { //allows users and admin to search for books
            string keyword, filter;
            cout << "\nSearch by (type (book, magazine, journal) / title / author / publisher): ";
            cin >> filter;
            cout << "Enter keyword: ";
            cin.ignore(); // this will remove leftover newline
            getline(cin, keyword);

            ifstream file("books.txt");
            if (!file) {
                cout << "Failed to open books.txt\n";
                return;
            }

            string line;
            bool found = false;

            while (getline(file, line)) {
                stringstream ss(line);
                string type, title, author, publisher, isBorrowedStr;

                getline(ss, type, ';');
                getline(ss, title, ';');
                getline(ss, author, ';');
                getline(ss, publisher, ';');
                getline(ss, isBorrowedStr);

                bool match = false;
                if (filter == "type" && title.find(keyword) != string::npos)
                    match = true;
                else if (filter == "title" && title.find(keyword) != string::npos)
                    match = true;
                else if (filter == "author" && author.find(keyword) != string::npos)
                    match = true;
                else if (filter == "publisher" && publisher.find(keyword) != string::npos)
                    match = true;

                if (match) {
                    found = true;
                    cout << "\n--- Book Found ---\n";
                    cout << "Type: " << type
                        << "Title: " << title
                        << "\nAuthor: " << author
                        << "\nPublisher: " << publisher
                        << "\nBorrowed: " << (isBorrowedStr == "1" ? "Yes" : "No") << "\n";
                }
            }

            if (!found) {
                cout << "No book matched your search.\n";
            }

            file.close();
        }
        */
    }; // end of class SearchFunction


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
            newUser.saveToFile("Final Code Project Take2/data/users.txt");
            break;
        }
        case 2: { // Admin (Librarian) Login
            string user, pass;
            cout << "Enter admin username: ";
            cin >> user;
            cout << "Enter admin password: ";
            cin >> pass;

            Admin admin("admin", "admin123"); // Hardcoded admin credentials for now

            if (admin.login(user, pass)) {
                cout << "Login successful.\n";
                admin.showMenu();
            }
            else {
                cout << "Invalid login.\n";
            }
            break;
        }

        case 3: { // User Login
            string user, pass;
            cout << "Enter username: ";
            cin >> user;
            cout << "Enter password: ";
            cin >> pass;

            UserLogin u("user", "pass123"); // Example credentials

            if (u.login(user, pass)) {
                cout << "User login successful.\n";
                u.showMenu();
            }
            else {
                cout << "Invalid username or password.\n";
            }
            break;
        }
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
