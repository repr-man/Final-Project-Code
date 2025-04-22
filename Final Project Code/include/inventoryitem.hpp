#pragma once

#include <filesystem>
#include <string>

#include "printable.hpp"

class InventoryItem : public Printable<5> {
public:
    std::string type, name, author, publisher;
    int borrowerID;

    static constexpr int Offset = 0;
    inline static const auto SaveFileLocation
        = std::filesystem::path(Printable<5>::SaveFileLocation) / "book.txt";

    enum class FieldTag {
        Type,
        Name,
        Author,
        Publisher,
        BorrowerID
    };
    static constexpr auto Type = FieldTag::Type;
    static constexpr auto Name = FieldTag::Name;
    static constexpr auto Author = FieldTag::Author;
    static constexpr auto Publisher = FieldTag::Publisher;
    static constexpr auto BorrowerID = FieldTag::BorrowerID;

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

    bool matches(FieldTag field, const std::string& value) const {
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

    std::string serialize() const {
        return type + ';' +
            name + ';' +
            author + ';' +
            publisher + ';' +
            std::to_string(borrowerID);
    }
    
    std::array<std::string, 5> providePrintableData() const override {
        return {
            type,
            name,
            author,
            publisher,
            std::to_string(borrowerID)
        };
    }
    
    friend class Library;
};
