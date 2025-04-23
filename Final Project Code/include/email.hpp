#pragma once

#include <string>

class Email {
    std::string email;

    static int eatIPv4Segment(std::string_view str);
public:
    Email(std::string&& e);
    operator std::string();
};
