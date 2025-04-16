#pragma once

#include <string>

class InventoryItem {
    std::string type, name, author, publisher;
    int borrowerID;
public:
	InventoryItem(
        std::string type,
        std::string name,
        std::string author,
        std::string publisher,
        int borrowerID
    ) : type(type),
        name(name),
        author(author),
        publisher(publisher),
        borrowerID(borrowerID)
    {}

    std::string getType() const {
        return type;
    }
};
