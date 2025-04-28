#include "user.hpp"
#include "util.hpp"

std::istream& operator>>(std::istream& is, User::FieldTag& item) {
    int val;
    is >> val;
    item = static_cast<User::FieldTag>(val);
    return is;
}

User::User(
    long id,
    std::string&& role,
    std::string&& first,
    std::string&& last,
    std::string&& address,
    std::string&& phone,
    std::string&& email,
    std::string&& password,
    long institutionId,
    int numCheckedOut
) : id(id),
    role(role),
    first(first),
    last(last),
    address(address),
    phone(phone),
    email(email),
    password(password),
    institutionId(institutionId),
    numCheckedOut(numCheckedOut)
{}

bool User::matches(User::FieldTag field, const std::string& value) const noexcept {
    switch (field) {
        case ID:
            return id == stol(value);
        case Role:
            return role == value;
        case First:
            return first == value;
        case Last:
            return last == value;
        case Address:
            return address == value;
        case Phone:
            return phone == value;
        case Email:
            return email == value;
        case Password:
            return password == value;
        case InstitutionID:
            return institutionId == stol(value);
        case NumCheckedOut:
            return numCheckedOut == stoi(value);
    }
    UNREACHABLE;
}

std::string User::serialize() const {
    return std::to_string(id) + ';' +
    role + ';' +
    first + ';' +
    last + ';' +
    address + ';' +
    phone + ';' +
    email + ';' +
    password + ';' +
    std::to_string(institutionId) + ';' +
    std::to_string(numCheckedOut);
}

std::array<std::string, 10> User::providePrintableData() const {
    return {
        std::to_string(id),
        role,
        first,
        last,
        address,
        phone,
        email,
        password,
        std::to_string(institutionId),
        std::to_string(numCheckedOut)
    };
}

