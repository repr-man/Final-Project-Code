#pragma once

#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>
#include "inventoryitem.hpp"

class Library {
    std::vector<InventoryItem> inventory;
    std::vector<User> users;
    std::vector<Librarian> librarians;

public:
	Library() {
        std::string buffer;
        buffer.reserve(1024);

		auto bookFile = std::fstream("Final Project Code/data/book.txt");
        if (!bookFile) {
            throw "Failed to open book.txt file.";
        }
        while(bookFile.getline(buffer.data(), 1024, '\n').good()) {
            std::string type, name, author, publisher, borrower;
            auto line = std::stringstream(buffer);
            std::getline(line, type, ';');
            std::getline(line, name, ';');
            std::getline(line, author, ';');
            std::getline(line, publisher, ';');
            std::getline(line, borrower);
            inventory.push_back(InventoryItem(type, name, author, publisher, borrower));
        }
        bookFile.close();

		auto userFile = std::fstream("Final Project Code/data/users.txt");
        if (!userFile) {
            throw "Failed to open book.txt file.";
        }
        while(userFile.getline(buffer.data(), 1024, '\n').good()) {
            std::string buf;
            int id, institutionId;
            std::string role, first, last, address, phone, email, password, numCheckedOut;
            auto line = std::stringstream(buffer);
            line >> id;
            line.ignore(1);
            std::getline(line, role, ';');
            std::getline(line, first, ';');
            std::getline(line, last, ';');
            std::getline(line, address, ';');
            std::getline(line, phone, ';');
            std::getline(line, email, ';');
            std::getline(line, password, ';');
            line >> institutionId;
            line.ignore(1);
            line >> numCheckedOut;
            inventory.push_back(User(id, role, first, last, address, phone, email, password, numCheckedOut));
        }
        userFile.close();

		auto librariansFile = std::fstream("Final Project Code/data/librarians.txt");
        if (!librariansFile) {
            throw "Failed to open librarians.txt file.";
        }
        while(librariansFile.getline(buffer.data(), 1024, '\n').good()) {
            std::string first, last, password;
            auto line = std::stringstream(buffer);
            std::getline(line, first, ';');
            std::getline(line, last, ';');
            std::getline(line, password);
            inventory.push_back(Librarian(first, last, password));
        }
        librariansFile.close();
    }
}; 
