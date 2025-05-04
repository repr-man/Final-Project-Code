#pragma once

#include "Borrowing.hpp"
#include "SearchFunction.hpp"
#include "inventoryitem.hpp"
#include "library.hpp"
#include "terminal.hpp"
#include "user.hpp"
#include "UserHolder.hpp"
#include "util.hpp"
#include "validators.hpp"
#include "history.hpp"

#include <cstdio>
#include <iostream>
#include <string>


using namespace std;

class UserLogin : public UserHolder {
public:
    UserLogin(Library& lib, Terminal& term, User& user, string id, string pass) : UserHolder(lib, term, user, id, pass) {}

    static void attemptLogin(Library& lib, Terminal& term);

    void printUserSummary();

    void showMenu();
};
