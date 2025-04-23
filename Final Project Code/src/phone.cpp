#include "phone.hpp"
#include "terminal.hpp"
#include <cctype>

Phone::Phone(std::string&& value) : value(value) {
    if (
        value.size() != 12
        || !std::isdigit(value[0])
        || !std::isdigit(value[1])
        || !std::isdigit(value[2])
        ||  value[3] != '-'
        || !std::isdigit(value[4])
        || !std::isdigit(value[5])
        || !std::isdigit(value[6])
        ||  value[7] != '-'
        || !std::isdigit(value[8])
        || !std::isdigit(value[9])
        || !std::isdigit(value[10])
        || !std::isdigit(value[11])
    ) {
        throw Terminal::InvalidInput("Phone number does not have the format '###-###-####'.");
    }
}

Phone::operator std::string() {
    return value;
}
