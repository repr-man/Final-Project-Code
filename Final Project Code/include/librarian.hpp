#pragma once

#include <filesystem>
#include <string>

#include "librarystoragetype.hpp"

class Librarian : public LibraryStorageType {
public:
    std::string first, last, password;

    static constexpr int Offset = 3;
    inline static const auto SaveFileLocation
        = std::filesystem::current_path().filename() == "Final-Project-Code"
        ? std::filesystem::current_path() / "Final Project Code" / "data" / "librarians.txt"
        : std::filesystem::current_path() / "data" / "librarians.txt";

    enum class FieldTag {
        First,
        Last,
        Password
    };

    static constexpr auto First = FieldTag::First;
    static constexpr auto Last = FieldTag::Last;
    static constexpr auto Password = FieldTag::Password;

    static std::string to_string(const Librarian::FieldTag& item);
    friend std::ostream& operator<<(std::ostream& os, const Librarian::FieldTag& item);
    friend std::istream& operator>>(std::istream& is, Librarian::FieldTag& item);

    Librarian(
        std::string&& first,
        std::string&& last,
        std::string&& password
    );

    bool matches(FieldTag field, const std::string& value) const;

    std::string serialize() const;

    Row provideRow() const override;

    std::string get(FieldTag field) const;
    
    friend class Library;
};
