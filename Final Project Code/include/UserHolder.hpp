#pragma once
#include "library.hpp"
#include "terminal.hpp"
#include "user.hpp"

using namespace std;

class UserHolder {
protected:
    Library& lib;
    Terminal& term;
    string userID, password;

public:
    User& user;

    UserHolder(Library& lib, Terminal& term, User& user, string id, string pass) : lib(lib), term(term), user(user), userID(id), password(pass) {}
};
