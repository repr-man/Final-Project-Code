#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include "librarystoragetype.hpp"

class User : public LibraryStorageType {
public:
    std::string role, first, last, address, phone, email, password;
    uint64_t id, institutionId;
    uint32_t numCheckedOut;

    static constexpr int Offset = 1;
    inline static const auto SaveFileLocation
        = std::filesystem::current_path().filename() == "Final-Project-Code"
        ? std::filesystem::current_path() / "Final Project Code" / "data" / "users.txt"
        : std::filesystem::current_path() / "data" / "users.txt";

    enum class FieldTag {
        ID,
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

    static constexpr auto ID = FieldTag::ID;
    static constexpr auto Role = FieldTag::Role;
    static constexpr auto First = FieldTag::First;
    static constexpr auto Last = FieldTag::Last;
    static constexpr auto Address = FieldTag::Address;
    static constexpr auto Phone = FieldTag::Phone;
    static constexpr auto Email = FieldTag::Email;
    static constexpr auto Password = FieldTag::Password;
    static constexpr auto InstitutionID = FieldTag::InstitutionID;
    static constexpr auto NumCheckedOut = FieldTag::NumCheckedOut;

    static std::string to_string(const User::FieldTag& item);
    friend std::ostream& operator<<(std::ostream& os, const User::FieldTag& item);
    friend std::istream& operator>>(std::istream& is, User::FieldTag& item);

    User(
        uint64_t id,
        std::string&& role,
        std::string&& first,
        std::string&& last,
        std::string&& address,
        std::string&& phone,
        std::string&& email,
        std::string&& password,
        uint64_t institutionId,
        uint32_t numCheckedOut
    );

    std::string serialize() const override;
    Row provideRow() const override;

    bool matches(FieldTag field, const std::string& value) const noexcept;
    std::string get(FieldTag field) const;
    
    friend class Library;
};
