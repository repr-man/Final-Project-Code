#pragma once

#include <string>
#include <string_view>

class Email {
    std::string email;

    static int eatIPv4Segment(std::string_view str);
public:
    Email(std::string&& e);
    operator std::string();
};
