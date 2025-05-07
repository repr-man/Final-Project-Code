#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include "librarystoragetype.hpp"

class Librarian : public LibraryStorageType {
public:
    uint64_t id;

    static constexpr int Offset = 3;
    inline static const auto SaveFileLocation
        = std::filesystem::current_path().filename() == "Final-Project-Code"
        ? std::filesystem::current_path() / "Final Project Code" / "data" / "librarians.txt"
        : std::filesystem::current_path() / "data" / "librarians.txt";

    enum class FieldTag {
        Id
    };

    static constexpr auto Id = FieldTag::Id;

    static std::string to_string(const Librarian::FieldTag& item);
    friend std::ostream& operator<<(std::ostream& os, const Librarian::FieldTag& item);
    friend std::istream& operator>>(std::istream& is, Librarian::FieldTag& item);

    Librarian(
        uint64_t id
    );

    std::string serialize() const override;
    Row provideRow() const override;

    bool matches(FieldTag field, const std::string& value) const;
    std::string get(FieldTag field) const;
    
    friend class Library;
};
