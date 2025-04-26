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
            auto rawCategories = term.promptForInput<
                vector<int>,
                validateNumRange<1, 8>
            >(
                "Enter space-separated list of search categories"
            );
            //auto catStream = stringstream(rawCategories);
            vector<User::FieldTag> categories;
            //int category;
            //while (true) {
            //    if (!(catStream >> category)) {
            //        if (catStream.eof()) { break; }
            //        term.printError("Invalid non-integer category. Please try again.");
            //        goto retry;
            //    }
            for (auto& category : rawCategories) {
                switch (category) {
                    using enum User::FieldTag;
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
                        UNREACHABLE;
                }
                queries.push_back(std::move(res));
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
                if (!(catStream >> category)) {
                    if (catStream.eof()) { break; }
                    term.printError("Invalid non-integer category. Please try again.");
                    goto retry;
                }
                switch (category) {
                    using enum InventoryItem::FieldTag;
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
                        UNREACHABLE;
                }
                queries.push_back(std::move(res));
            }
            return lib.search(categories, std::move(queries));
        }
    }
};
