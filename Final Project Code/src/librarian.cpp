#include "librarian.hpp"
#include "util.hpp"
#include <string>
#include <vector>

std::string Librarian::to_string(const Librarian::FieldTag& item) {
    switch (item) {
        case Librarian::Id:
            return "ID";
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
    uint64_t id
) : id(id) {}

bool Librarian::matches(Librarian::FieldTag field, const std::string& value) const {
    switch (field) {
        case Id:
            return id == std::stoull(value);
    }
    UNREACHABLE;
}

std::string Librarian::serialize() const {
    return std::to_string(id);
}

Row Librarian::provideRow() const {
    return Row(std::vector{std::to_string(id)});
}

std::string Librarian::get(Librarian::FieldTag field) const {
    switch (field) {
        case Id:
            return std::to_string(id);
    }
    UNREACHABLE;
}
