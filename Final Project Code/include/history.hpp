#pragma once

#include <array>
#include <string>
#include <filesystem>
#include "printable.hpp"

class HistoryItem : public Printable<2> {
public:
    long userID;
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

    HistoryItem(long userID, std::string&& name);

    bool matches(FieldTag field, const std::string& value) const;

    std::string serialize() const;

    std::array<std::string, 2> providePrintableData() const override;

    friend class Library;
};
