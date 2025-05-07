#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include "librarystoragetype.hpp"

class HistoryItem : public LibraryStorageType {
public:
    uint64_t userID;
    std::string name;

    static constexpr int Offset = 2;
    inline static const auto SaveFileLocation
        = std::filesystem::current_path().filename() == "Final-Project-Code"
        ? std::filesystem::current_path() / "Final Project Code" / "data" / "history.txt"
        : std::filesystem::current_path() / "data" / "history.txt";

    enum class FieldTag {
        UserID,
        Name
    };

    static constexpr auto UserID = FieldTag::UserID;
    static constexpr auto Name = FieldTag::Name;

    static std::string to_string(const HistoryItem::FieldTag& item);
    friend std::ostream& operator<<(std::ostream& os, const HistoryItem::FieldTag& item);
    friend std::istream& operator>>(std::istream& is, HistoryItem::FieldTag& item);

    HistoryItem(uint64_t userID, std::string&& name);

    std::string serialize() const override;
    Row provideRow() const override;

    bool matches(FieldTag field, const std::string& value) const;
    std::string get(FieldTag field) const;
    
    friend class Library;
};
