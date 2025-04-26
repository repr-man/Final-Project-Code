#pragma once
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "library.hpp"
#include "terminal.hpp"
#include "user.hpp"
#include "util.hpp"
#include "validators.hpp"

using namespace std;

class SearchFunction {
public:
    auto searchUser(Library& lib, Terminal& term) {
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
            auto rawCategories = term.promptForInput<string>(
                "Enter space-separated list of search categories"
            );
            auto catStream = stringstream(rawCategories);
            vector<User::FieldTag> categories;
            int category;
            while (true) {
                using enum User::FieldTag;
                if (!(catStream >> category)) {
                    if (catStream.eof()) { break; }
                    term.printError("Invalid non-integer category. Please try again.");
                    goto retry;
                }
                switch (category) {
                    case 1:
                        categories.push_back(ID);
                        break;
                    case 2:
                        categories.push_back(First);
                        break;
                    case 3:
                        categories.push_back(Last);
                        break;
                    case 4:
                        categories.push_back(Role);
                        break;
                    case 5:
                        categories.push_back(Address);
                        break;
                    case 6:
                        categories.push_back(Phone);
                        break;
                    case 7:
                        categories.push_back(Email);
                        break;
                    case 8:
                        categories.push_back(InstitutionID);
                        break;
                    default:
                        term.printError(
                            "Invalid search category '"s
                            + to_string(category)
                            + "'. Please try again."
                        );
                        goto retry;
                }
            }

            vector<string> queries;
            for (auto& category : categories) {
                using enum User::FieldTag;
                switch (category) {
                    case ID:
                        queries.push_back(term.promptForInput<string>("Enter User ID"));
                        break;
                    case First:
                        queries.push_back(term.promptForInput<string>("Enter First Name"));
                        break;
                    case Last:
                        queries.push_back(term.promptForInput<string>("Enter Last Name"));
                        break;
                    case Role:
                        queries.push_back(term.promptForInput<string>("Enter Role"));
                        break;
                    case Address:
                        queries.push_back(term.promptForInput<string>("Enter Address"));
                        break;
                    case Phone:
                        queries.push_back(term.promptForInput<string>("Enter Phone Number"));
                        break;
                    case Email:
                        queries.push_back(term.promptForInput<string, validateEmail>("Enter Email"));
                        break;
                    case InstitutionID:
                        queries.push_back(term.promptForInput<string>("Enter Institution ID"));
                        break;
                    default:
                        UNREACHABLE;
                }
            }

            return lib.search(categories, std::move(queries));
        }
    }

    auto searchInventory(Library& lib, Terminal& term) {
    retry: {
            term.printOptions("--- Search for Inventory ---", {
                "Type",
                "Name",
                "Author",
                "Publisher",
                "Borrower ID"
            });
            auto rawCategories = term.promptForInput<string>(
                "Enter space-separated list of search categories"
            );
            auto catStream = stringstream(rawCategories);
            vector<InventoryItem::FieldTag> categories;
            int category;
            while (true) {
                using enum InventoryItem::FieldTag;
                if (!(catStream >> category)) {
                    if (catStream.eof()) { break; }
                    term.printError("Invalid non-integer category. Please try again.");
                    goto retry;
                }
                switch (category) {
                    case 1:
                        categories.push_back(Type);
                        break;
                    case 2:
                        categories.push_back(Name);
                        break;
                    case 3:
                        categories.push_back(Author);
                        break;
                    case 4:
                        categories.push_back(Publisher);
                        break;
                    case 5:
                        categories.push_back(BorrowerID);
                        break;
                    default:
                        term.printError(
                            "Invalid search category '"s
                            + to_string(category)
                            + "'. Please try again."
                        );
                        goto retry;
                }
            }

            vector<string> queries;
            for (auto& category : categories) {
                using enum InventoryItem::FieldTag;
                switch (category) {
                    case Type:
                        queries.push_back(term.promptForInput<string>("Enter Type"));
                        break;
                    case Name:
                        queries.push_back(term.promptForInput<string>("Enter Name"));
                        break;
                    case Author:
                        queries.push_back(term.promptForInput<string>("Enter Author"));
                        break;
                    case Publisher:
                        queries.push_back(term.promptForInput<string>("Enter Publisher"));
                        break;
                    case BorrowerID:
                        queries.push_back(term.promptForInput<string>("Enter Borrower ID"));
                        break;
                    default:
                        UNREACHABLE;
                }
            }
            return lib.search(categories, std::move(queries));
        }
    }
};
