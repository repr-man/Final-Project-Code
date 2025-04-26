#pragma once

#include <filesystem>
#include <string>

#include "printable.hpp"

class User : public Printable<10> {
public:
    std::string role, first, last, address, phone, email, password;
    long id, institutionId;
    int numCheckedOut;

    static constexpr int Offset = 1;
    inline static const auto SaveFileLocation
        = std::filesystem::path(Printable<10>::SaveFileLocation) / "users.txt";

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
    );

    bool matches(FieldTag field, const std::string& value) const noexcept;

    std::string serialize() const;

    std::array<std::string, 10> providePrintableData() const override;

    friend class Library;
};
