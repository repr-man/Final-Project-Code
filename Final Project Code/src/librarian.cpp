#include "librarian.hpp"
#include "util.hpp"
#include <vector>

std::string Librarian::to_string(const Librarian::FieldTag& item) {
    switch (item) {
        case Librarian::First:
            return "First Name";
        case Librarian::Last:
            return "Last Name";
        case Librarian::Password:
            return "Password";
        default:
            UNREACHABLE;
    }
}

std::ostream& operator<<(std::ostream& os, const Librarian::FieldTag& item) {
    os << Librarian::to_string(item);
    return os;
}

std::istream& operator>>(std::istream& is, Librarian::FieldTag& item) {
    int val;
    is >> val;
    item = static_cast<Librarian::FieldTag>(val - 1);
    return is;
}

Librarian::Librarian(
    std::string&& first,
    std::string&& last,
    std::string&& password
) : first(first),
    last(last),
    password(password)
{}

bool Librarian::matches(Librarian::FieldTag field, const std::string& value) const {
    switch (field) {
        case First:
            return first == value;
        case Last:
            return last == value;
        case Password:
            return password == value;
    }
    UNREACHABLE;
}

std::string Librarian::serialize() const {
    return first + ';' +
    last + ';' +
    password;
}

std::array<std::string, 3> Librarian::providePrintableData() const {
    return {
        first,
        last,
        password
    };
}
