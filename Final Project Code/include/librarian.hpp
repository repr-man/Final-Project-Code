#pragma once

#include <filesystem>
#include <string>

#include "printable.hpp"

class Librarian : public Printable<3> {
public:
    std::string first, last, password;

    static constexpr int Offset = 2;
    inline static const std::filesystem::path SaveFileLocation
        = std::filesystem::current_path().append("Final Project Code/data/librarians.txt");

    enum class FieldTag {
        First,
        Last,
        Password
    };

    static constexpr auto First = FieldTag::First;
    static constexpr auto Last = FieldTag::Last;
    static constexpr auto Password = FieldTag::Password;

    Librarian(
        std::string&& first,
        std::string&& last,
        std::string&& password
    ) : first(first),
        last(last),
        password(password)
    {}

    bool matches(FieldTag field, const std::string& value) const {
        switch (field) {
            case First:
                return first == value;
            case Last:
                return last == value;
            case Password:
                return password == value;
        }
    }

    std::string serialize() const {
        return first + ';' +
            last + ';' +
            password;
    }

    std::array<std::string, 3> providePrintableData() const override {
        return {
            first,
            last,
            password
        };
    }
    
    friend class Library;
};
