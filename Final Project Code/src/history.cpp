#include "history.hpp"
#include "util.hpp"
#include <cstdint>
#include <string>
#include <vector>

std::string HistoryItem::to_string(const HistoryItem::FieldTag& item) {
    switch (item) {
        case HistoryItem::UserID:
            return "User ID";
        case HistoryItem::Name:
            return "Name";
        default:
            UNREACHABLE;
    }
}

std::ostream& operator<<(std::ostream& os, const HistoryItem::FieldTag& item) {
    os << HistoryItem::to_string(item);
    return os;
}

std::istream& operator>>(std::istream& is, HistoryItem::FieldTag& item) {
    int val;
    is >> val;
    item = static_cast<HistoryItem::FieldTag>(val - 1);
    return is;
}

HistoryItem::HistoryItem(uint64_t userID, std::string&& name)
: userID(userID),
  name(name)
{}

bool HistoryItem::matches(HistoryItem::FieldTag field, const std::string& value) const {
    switch (field) {
        case UserID:
            return userID == std::stoull(value);
        case Name:
            return name == value;
    }
    UNREACHABLE;
}

std::string HistoryItem::serialize() const {
    return std::to_string(userID) + ';' + name;
}

Row HistoryItem::provideRow() const {
    return Row(std::vector{std::to_string(userID), name});
}

std::string HistoryItem::get(HistoryItem::FieldTag field) const {
    switch (field) {
        case UserID:
            return std::to_string(userID);
        case Name:
            return name;
    }
    UNREACHABLE;
}
