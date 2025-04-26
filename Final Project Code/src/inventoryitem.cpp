#include "inventoryitem.hpp"

using namespace std;



InventoryItem::InventoryItem(
    std::string&& type,
    std::string&& name,
    std::string&& author,
    std::string&& publisher,
    int borrowerID
) : type(type),
    name(name),
    author(author),
    publisher(publisher),
    borrowerID(borrowerID)
{}

bool InventoryItem::matches(InventoryItem::FieldTag field, const std::string& value) const {
    switch (field) {
        case Type:
            return type == value;
        case Name:
            return name == value;
        case Author:
            return author == value;
        case Publisher:
            return publisher == value;
        case BorrowerID:
            return borrowerID == stoi(value);
    }
}

std::string InventoryItem::serialize() const {
    return type + ';' +
    name + ';' +
    author + ';' +
    publisher + ';' +
    std::to_string(borrowerID);
}

std::array<std::string, 5> InventoryItem::providePrintableData() const {
    return {
        type,
        name,
        author,
        publisher,
        std::to_string(borrowerID)
    };
}
