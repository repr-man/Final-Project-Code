#pragma once
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "library.hpp"
#include "terminal.hpp"
#include "user.hpp"
#include "util.hpp"

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
                        cout << "\nEnter User ID:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case First:
                        cout << "\nEnter First Name:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Last:
                        cout << "\nEnter Last Name:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Role:
                        cout << "\nEnter Role:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Address:
                        cout << "\nEnter Address:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Phone:
                        cout << "\nEnter Phone Number:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Email:
                        cout << "\nEnter Email:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case InstitutionID:
                        cout << "\nEnter Institution ID:\n";
                        queries.push_back(term.promptForInput<string>());
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
                        cout << "\nEnter Type:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Name:
                        cout << "\nEnter Name:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Author:
                        cout << "\nEnter Author:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case Publisher:
                        cout << "\nEnter Publisher:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    case BorrowerID:
                        cout << "\nEnter Borrower ID:\n";
                        queries.push_back(term.promptForInput<string>());
                        break;
                    default:
                        UNREACHABLE;
                }
            }
            return lib.search(categories, std::move(queries));
        }
    }
};
