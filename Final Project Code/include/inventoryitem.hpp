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
    
    std::array<std::string, 5> providePrintableData() const override;
    
    friend class Library;
};
