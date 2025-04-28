#include "history.hpp"
#include <string>

std::istream& operator>>(std::istream& is, HistoryItem::FieldTag& item) {
    int val;
    is >> val;
    item = static_cast<HistoryItem::FieldTag>(val);
    return is;
}

HistoryItem::HistoryItem(long userID, std::string&& name)
: userID(userID),
  name(name)
{}

bool HistoryItem::matches(HistoryItem::FieldTag field, const std::string& value) const {
    switch (field) {
        case UserID:
            return userID == std::stoll(value);
        case Name:
            return name == value;
    }
}

std::string HistoryItem::serialize() const {
    return std::to_string(userID) + ';' + name;
}

std::array<std::string, 2> HistoryItem::providePrintableData() const {
    return {
        std::to_string(userID),
        name
    };
}
