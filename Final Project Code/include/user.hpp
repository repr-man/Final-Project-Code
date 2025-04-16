#pragma once

#include <string>

class User {
    std::string role, first, last, address, phone, email, password;
    long id, institutionId;
    int numCheckedOut;

public:
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
};
