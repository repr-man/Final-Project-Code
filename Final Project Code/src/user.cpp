#include "user.hpp"
#include "util.hpp"
#include <string>
#include <vector>

std::string User::to_string(const User::FieldTag& item) {
    switch (item) {
        case User::ID:
            return "ID";
        case User::Role:
            return "Role";
        case User::First:
            return "First Name";
        case User::Last:
            return "Last Name";
        case User::Address:
            return "Address";
        case User::Phone:
            return "Phone";
        case User::Email:
            return "Email";
        case User::Password:
            return "Password";
        case User::InstitutionID:
            return "Institution ID";
        case User::NumCheckedOut:
            return "# of Books Checked Out";
        default:
            UNREACHABLE;
    }
}

std::ostream& operator<<(ostream& os, const User::FieldTag& item) {
    os << User::to_string(item);
    return os;
}

std::istream& operator>>(std::istream& is, User::FieldTag& item) {
    int val;
    is >> val;
    item = static_cast<User::FieldTag>(val - 1);
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
            return id == std::stoll(value);
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
            return institutionId == std::stoll(value);
        case NumCheckedOut:
            return numCheckedOut == std::stoi(value);
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

Row User::provideRow() const {
    return Row(std::vector{
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
    });
}

std::string User::get(User::FieldTag field) const {
    switch (field) {
        case ID:
            return std::to_string(id);
        case Role:
            return role;
        case First:
            return first;
        case Last:
            return last;
        case Address:
            return address;
        case Phone:
            return phone;
        case Email:
            return email;
        case Password:
            return password;
        case InstitutionID:
            return std::to_string(institutionId);
        case NumCheckedOut:
            return std::to_string(numCheckedOut);
    }
    UNREACHABLE;
}
