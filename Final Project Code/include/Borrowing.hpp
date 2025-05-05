#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include "Admin.hpp"
#include "UserHolder.hpp"
#include "UserLogin.hpp"
#include "terminal.hpp"

#include "library.hpp"

using namespace std;

/* When a user borrows a book add one to the borrowedbooks count */

class Borrowing {
public:
    void borrowBook(Library& lib, Terminal& term);

    template <typename T>
    requires std::is_base_of_v<UserHolder, T>
    void borrowItems(Library& lib, Terminal& term, T& user, ResultList<InventoryItem>& items);
}; // end of class Borrowing
