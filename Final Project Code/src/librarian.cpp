#include "librarian.hpp"

std::istream& operator>>(std::istream& is, Librarian::FieldTag& item) {
    int val;
    is >> val;
    item = static_cast<Librarian::FieldTag>(val);
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
