#pragma once
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include "Borrowing.hpp"
#include "RegisterUser.hpp"
#include "Return.hpp"
#include "SearchFunction.hpp"
#include "library.hpp"
#include "resultlist.hpp"
#include "terminal.hpp"
#include "user.hpp"
#include "util.hpp"
#include "validators.hpp"

#include "UserHolder.hpp" 



using namespace std;

class Admin : public UserHolder {
public:
    Admin(Library& lib, Terminal& term, User& user, string username, string pass) : UserHolder(lib, term, user, username, pass) {}

    bool login(string inputUser, string inputPass);
    void showMenu();

private: 
    void registerNewUser();
    void editUserInfo();
    void manageInventoryAfterSearch(ResultList<InventoryItem>& res);
    void editInventory();
    void editUserInfoAfterSearch(ResultList<User>& res);
    void updateUserInfo(User& user);
    void showAllUsers();
    void updateInventoryItem(InventoryItem& item);
};
