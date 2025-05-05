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
    std::optional<ResultList<User>> searchUser(Library& lib, Terminal& term) {
    retry: {
            term.printOptions("--- Search for User ---", {
                "ID",
                "First Name",
                "Last Name",
                "Role",
                "Address",
                "Phone Number",
                "Email",
                "Institution ID"
            });
            auto categories = term.promptForInput<
                vector<User::FieldTag>,
                validateNumRange<0, 8>
            >(
                "Enter space-separated list of search categories (0 to cancel)"
            );

            vector<string> queries;
            for (auto& category : categories) {
                std::string res;
                switch (category) {
                    using enum User::FieldTag;
                    case ID:
                        res = term.promptForInput<string, validateLibraryID>("Enter User ID");
                        break;
                    case First:
                        res = term.promptForInput<string>("Enter First Name");
                        break;
                    case Last:
                        res = term.promptForInput<string>("Enter Last Name");
                        break;
                    case Role:
                        res = term.promptForInput<string, validateRole>("Enter Role");
                        break;
                    case Address:
                        res = term.promptForInput<string>("Enter Address");
                        break;
                    case Phone:
                        res = term.promptForInput<string, validatePhone>("Enter Phone Number");
                        break;
                    case Email:
                        res = term.promptForInput<string, validateEmail>("Enter Email");
                        break;
                    case InstitutionID:
                        res = term.promptForInput<string, validateInstitutionID>(
                            "Enter Institution ID"
                        );
                        break;
                    default:
                        return std::nullopt;
                }
                queries.push_back(std::move(res));
            }

            return std::optional(lib.search(categories, std::move(queries)));
        }
    }

    std::optional<ResultList<InventoryItem>> searchInventory(Library& lib, Terminal& term) {
        term.printOptions("--- Search for Inventory ---", {
            "Type",
            "Name",
            "Author",
            "Publisher",
            "Borrower ID"
        });
        auto categories = term.promptForInput<
            std::vector<InventoryItem::FieldTag>,
            validateNumRange<0, 5>
            >(
                "Enter space-separated list of search categories (0 to cancel)"
            );

        vector<string> queries;
        for (auto& category : categories) {
            std::string res;
            switch (category) {
                using enum InventoryItem::FieldTag;
                case Type:
                    res = term.promptForInput<string>("Enter Type");
                    break;
                case Name:
                    res = term.promptForInput<string>("Enter Name");
                    break;
                case Author:
                    res = term.promptForInput<string>("Enter Author");
                    break;
                case Publisher:
                    res = term.promptForInput<string>("Enter Publisher");
                    break;
                case BorrowerID:
                    res = term.promptForInput<string, validateLibraryID>(
                        "Enter Borrower ID"
                    );
                    break;
                default:
                    return std::nullopt;
            }
            queries.push_back(std::move(res));
        }
        return std::optional(lib.search(categories, std::move(queries)));
    }
};
