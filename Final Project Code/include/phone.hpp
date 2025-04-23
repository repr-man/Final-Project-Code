#pragma once

#include <string>

class Phone {
    std::string value;

public:
    Phone(std::string&& value);
    operator std::string();
};
