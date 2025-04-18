#pragma once

#include <filesystem>
#include <string>

class User {
public:
    std::string role, first, last, address, phone, email, password;
    long id, institutionId;
    int numCheckedOut;

    static constexpr int Offset = 1;
    inline static const std::filesystem::path SaveFileLocation
        = std::filesystem::current_path().append("Final Project Code/data/users.txt");

    enum class FieldTag {
        Role,
        First,
        Last,
        Address,
        Phone,
        Email,
        Password,
        InstitutionID,
        NumCheckedOut
    };

    static constexpr auto Role = FieldTag::Role;
    static constexpr auto First = FieldTag::First;
    static constexpr auto Last = FieldTag::Last;
    static constexpr auto Address = FieldTag::Address;
    static constexpr auto Phone = FieldTag::Phone;
    static constexpr auto Email = FieldTag::Email;
    static constexpr auto Password = FieldTag::Password;
    static constexpr auto InstitutionID = FieldTag::InstitutionID;
    static constexpr auto NumCheckedOut = FieldTag::NumCheckedOut;

    User(
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

    bool matches(FieldTag field, const std::string& value) const noexcept {
        switch (field) {
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
        throw "Unreachable";
    }

    std::string serialize() const {
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

    friend class Library;
};
