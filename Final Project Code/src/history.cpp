#include "history.hpp"
#include <string>

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
