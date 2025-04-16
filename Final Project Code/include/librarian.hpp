#pragma once

#include <string>

class Librarian {
    std::string first, last, password;

public:
    Librarian(
        std::string&& first,
        std::string&& last,
        std::string&& password
    ) : first(first),
        last(last),
        password(password)
    {}
};
