#include "inventoryitem.hpp"
#include "util.hpp"
#include <vector>


using namespace std;

std::string InventoryItem::to_string(const InventoryItem::FieldTag& item) {
    switch (item) {
        case InventoryItem::Type:
            return "Type";
        case InventoryItem::Name:
            return "Name";
        case InventoryItem::Author:
            return "Author";
        case InventoryItem::Publisher:
            return "Publisher";
        case InventoryItem::BorrowerID:
            return "Borrower ID";
        default:
            UNREACHABLE;
    }
}

ostream& operator<<(ostream& os, const InventoryItem::FieldTag& item) {
    os << InventoryItem::to_string(item);
    return os;
}

std::istream& operator>>(std::istream& is, InventoryItem::FieldTag& item) {
    int val;
    is >> val;
    item = static_cast<InventoryItem::FieldTag>(val - 1);
    return is;
}

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
    UNREACHABLE;
}

std::string InventoryItem::serialize() const {
    return type + ';' +
    name + ';' +
    author + ';' +
    publisher + ';' +
    std::to_string(borrowerID);
}

Row InventoryItem::provideRow() const {
    return Row(std::vector{
        type,
        name,
        author,
        publisher,
        std::to_string(borrowerID)
    });
}

std::string InventoryItem::get(InventoryItem::FieldTag field) const {
    switch (field) {
        case Type:
            return type;
        case Name:
            return name;
        case Author:
            return author;
        case Publisher:
            return publisher;
        case BorrowerID:
            return std::to_string(borrowerID);
    }
    UNREACHABLE;
}
