#include <iostream>
#include <fstream> 
#include <string>

class library() {
public:
	library() {
		auto bookFile = std::fstream("Final Project Code/data/book.txt");
        if (!bookFile) {
            std::cerr << "Failed to open book.txt file." << std::endl;
            return;
        }

   

        bookFile.close();

        auto usersFile = std::fstream("Final Project Code/data/users.txt");
        if (!usersFile) {
            std::cerr << "Failed to open users.txt file." << std::endl;
            return;
    }

        usersFile.close();

        auto librariansFile = std::fstream("Final Project Code/data/librarians.txt");
        if (!librariansFile) {
            std::cerr << "Failed to open users.txt file." << std :: endl;
            return;
        }

        librariansFile.close();



	}


}; 