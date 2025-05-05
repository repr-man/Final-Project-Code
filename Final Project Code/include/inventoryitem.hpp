#pragma once

#include <filesystem>
#include <string>

#include "librarystoragetype.hpp"

class InventoryItem : public LibraryStorageType {
public:
    std::string type, name, author, publisher;
    int borrowerID;
    
    static constexpr int Offset = 0;
    inline static const auto SaveFileLocation
        = std::filesystem::current_path().filename() == "Final-Project-Code"
        ? std::filesystem::current_path() / "Final Project Code" / "data" / "book.txt"
        : std::filesystem::current_path() / "data" / "book.txt";

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

    static std::string to_string(const InventoryItem::FieldTag& item);
    friend std::ostream& operator<<(std::ostream& os, const InventoryItem::FieldTag& item);
    friend std::istream& operator>>(std::istream& is, InventoryItem::FieldTag& item);

	InventoryItem(
        std::string&& type,
        std::string&& name,
        std::string&& author,
        std::string&& publisher,
        int borrowerID
    );
 
    bool matches(FieldTag field, const std::string& value) const;

    std::string serialize() const;
    
    Row provideRow() const override;
    
    std::string get(FieldTag field) const;
    
    friend class Library;
};
