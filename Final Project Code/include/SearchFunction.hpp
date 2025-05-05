#pragma once
#include <cstdio>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "Admin.hpp"
#include "UserLogin.hpp"
#include "inventoryitem.hpp"
#include "library.hpp"
#include "terminal.hpp"
#include "user.hpp"
#include "util.hpp"
#include "validators.hpp"

using namespace std;

class SearchFunction {
public:
    std::optional<ResultList<User>> searchUser(Library& lib, Terminal& term);

    template <typename T> requires std::is_base_of_v<UserHolder, T>
        std::optional<ResultList<InventoryItem>> searchInventory(
        Library& lib,
        Terminal& term,
        const T& user
    );
};
