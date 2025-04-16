#pragma once

#include <string>

class Librarian {
    std::string first, last, password;

    enum class FieldTag {
        First,
        Last,
        Password
    };

public:
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

    friend class Library;
};
