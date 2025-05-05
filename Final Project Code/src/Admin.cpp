#include "Admin.hpp"
#include <cstdint>


bool Admin::login(string inputUser, string inputPass) {
    return (inputUser == userID && inputPass == password);
}

void Admin::showMenu() {
    int choice;
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Inventory Managment\n";
        cout << "2. User Managment\n";
        cout << "3. Borrow Book\n";
        cout << "4. Search Function\n";
        cout << "5. Active Users\n";
        cout << "6. Return Book\n";
        cout << "7. Logout\n";
        choice = term.promptForInput<uint32_t, validateNumRange<1, 7>>("Enter your choice");

        switch (choice) {
            case 1:
                cout << "Inventory Management...\n";
                editInventory();
                break;
            case 2:
                cout << "User Management...\n";
                editUserInfo();
                break;
            case 3: {
                cout << "Borrowing books...\n";
                Borrowing b;
                b.borrowBook(lib, term);
                break;
            }// end of case 4

            case 4: // this sends them to the SearchFunction Class
                cout << "\n--- Search Menu ---\n";
                cout << "1. User\n";
                cout << "2. Inventory\n";
                cout << "3. Exit\n";
                choice = term.promptForInput<uint32_t, validateNumRange<1, 3>>("Enter your choice");

                SearchFunction s;
                switch (choice) {
                    case 1: {
                        using enum User::FieldTag;
                        auto res = s.searchUser(lib, term);
                        if (!res) break;
                        if (res->size() == 0) {
                            cout << "No results found.\n";
                            break;
                        }
                        term.printTable(*res, ID, Role, First, Last);
                        editUserInfoAfterSearch(*res);
                        break;
                    }
                    case 2: {
                        using enum InventoryItem::FieldTag;
                        auto res = s.searchInventory(lib, term, *this);
                        if (!res) break;
                        if (res->size() == 0) {
                            cout << "No results found.\n";
                            break;
                        }
                        term.printTable(*res, Type, Name, Author, Publisher, BorrowerID);
                        manageInventoryAfterSearch(*res);
                        break;
                    }
                    case 3:
                        cout << "Returning to Admin Menu...\n";
                        return;
                    default:
                        UNREACHABLE;
                }
                break;

            case 5:
                cout << "Viewing all registered users...\n";
                showAllUsers();
                break;
            case 6:
                cout << "Returning books...\n";
                Return r;
                r.ReturnBook(lib, term);
                break;
            case 7:
                cout << "Logging out...\n";
                return;
            default:
                UNREACHABLE;
        }
    }
}

void Admin::registerNewUser() {
    int nextID = generateNextLibraryID(lib);  
    RegisterUser newUser;
    newUser.promptUserData(term, nextID); 
    newUser.printSummary();
    newUser.saveToFile(lib);
}

void Admin::editUserInfo() {
    cout << "\n--- User Management Menu ---\n";
    cout << "1. Register New User\n";
    cout << "2. Update User Information\n";
    cout << "3. Delete User\n";
    cout << "4. Back to Admin Menu\n";
    int choice = term.promptForInput<uint32_t, validateNumRange<1, 4>>("Enter your choice");

    switch (choice) {
        case 1: { // Register New User
            registerNewUser();
            cout << "New user registered successfully!\n";
            break;
        }
        case 2:{ // Update User
            auto users = lib.allUsers();
            if (users.size() == 0) {
                cout << "No users to update.\n";
                break;
            }

            cout << "\n--- Registered Users ---\n";
            for (size_t i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i].first << " " << users[i].last << "\n";
            }

            int userIndex = term.promptForInput<uint32_t>(
                "Enter the number of the user to update (0 to cancel)"
            );
            while (true) {
                if (userIndex == 0) {
                    cout << "Update cancelled.\n";
                    return;
                } else if (userIndex > 0 && userIndex <= users.size()) {
                    break;
                }
                term.printError("Invalid choice `" + to_string((int) userIndex) +
                                "`. Number must be in range [1, " +
                                to_string(users.size()) + "]");
                userIndex = term.promptForInput<uint32_t>("Try again");
            }

            updateUserInfo(users[userIndex - 1]);
            break;
        }

        case 3: { // Delete User
            auto users = lib.allUsers();
            if (users.size() == 0) {
                cout << "No users to delete.\n";
                break;
            }

            cout << "\n--- Registered Users ---\n";
            for (size_t i = 0; i < users.size(); ++i) {
                cout << i + 1 << ". " << users[i].first << " " << users[i].last << "\n";
            }

            int delUserIndex = term.promptForInput<uint32_t>(
                "Enter the number of the user to delete (0 to cancel)"
            );
            while (true) {
                if (delUserIndex == 0) {
                    cout << "Delete cancelled.\n";
                    return;
                } else if (delUserIndex > 0 && delUserIndex <= users.size()) {
                    break;
                }
                term.printError("Invalid choice `" + to_string((int) delUserIndex) +
                                "`. Number must be in range [1, " +
                                to_string(users.size()) + "]");
                delUserIndex = term.promptForInput<uint32_t>("Try again");
            }

            users.remove(delUserIndex - 1);
            cout << "User deleted successfully.\n";
            break;
        }

        case 4:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            UNREACHABLE;
    }
}

void Admin::manageInventoryAfterSearch(ResultList<InventoryItem>& res) {
    cout << "\n--- Inventory Menu ---\n";
    cout << "1. Borrow Items\n";
    cout << "2. Delete Items\n";
    cout << "3. Edit Items\n";
    cout << "4. Back to Admin Menu\n";
    int choice = term.promptForInput<uint32_t, validateNumRange<1, 4>>("Enter your choice");

    switch (choice) {
        case 1:
            Borrowing b;
            b.borrowItems(lib, term, *this, res);
            break;
        case 2: {
            auto selection = term.promptForInput<vector<uint32_t>>(
                "Enter the #s of the items to select (0 to cancel)"
            );
            while (true) {
                if (std::any_of(selection.begin(), selection.end(), [](int i) {
                    return i == 0; })
                ) {
                    cout << "Operation cancelled.\n";
                    return;
                } else if (auto pos = std::find_if(selection.begin(), selection.end(),
                                                   [&](int i) {
                                                   return i < 0 || i > res.size();
                                                   }
                                                   ); pos == selection.end()) {
                    term.printError("Invalid choice `" + to_string(pos - selection.begin()) +
                                    "`. Number must be in range [0, " + to_string(res.size()) +
                                    "]");
                    selection = term.promptForInput<vector<uint32_t>>("Try again");
                } else {
                    break;
                }
            }

            for (auto i : selection) {
                res.remove(i - 1);
            }
            cout << "Items deleted successfully.\n";
            break;
        }
        case 3: {
            auto selection = term.promptForInput<vector<uint32_t>>(
                "Enter the #s of the items to select (0 to cancel)"
            );
            while (true) {
                if (std::any_of(selection.begin(), selection.end(), [](int i) {
                    return i == 0; })
                ) {
                    cout << "Operation cancelled.\n";
                    return;
                } else if (auto pos = std::find_if(selection.begin(), selection.end(),
                                                   [&](int i) {
                                                   return i < 0 || i > res.size();
                                                   }
                                                   ); pos == selection.end()) {
                    term.printError("Invalid choice `" + to_string(pos - selection.begin()) +
                                    "`. Number must be in range [0, " + to_string(res.size()) +
                                    "]");
                    selection = term.promptForInput<vector<uint32_t>>("Try again");
                } else {
                    break;
                }
            }

            for (auto i : selection) {
                cout << "\n=== Item " << i << " ===";
                updateInventoryItem(res[i - 1]);
            }
            cout << "Items updated successfully.\n";
            break;
        }
        case 4:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            UNREACHABLE;
    }
}

void Admin::editInventory() {
    cout << "\n--- Inventory Menu ---\n";
    cout << "1. Add New Item\n";
    cout << "2. Delete Item\n";
    cout << "3. Edit Item\n";
    cout << "4. Back to Admin Menu\n";
    int choice = term.promptForInput<uint32_t, validateNumRange<1, 4>>("Enter your choice");

    switch (choice) {
        case 1: {
            string type = term.promptForInput<string>("Enter type");
            string name = term.promptForInput<string>("Enter name/title");
            string author = term.promptForInput<string>("Enter author");
            string publisher = term.promptForInput<string>("Enter publisher");
            string borrowed = "-1"; 

            auto itemExists = lib.addInventory(
                std::move(type),
                std::move(name),
                std::move(author),
                std::move(publisher),
                std::move(borrowed));
            if (itemExists) {
                term.printError("An item named '" + name + "' already exists in the inventory.");
                break;
            }
            cout << "Item added successfully!\n";
            break;
        }
        case 2: {
            auto items = lib.allInventory();

            if (items.size() == 0) {
                cout << "No items in inventory to delete.\n";
                break;
            }

            cout << "\n--- Current Inventory ---\n";
            for (size_t i = 0; i < items.size(); ++i) {
                cout << i + 1 << ". " << items[i].name << " by " << items[i].author << "\n";
            }

            int delIndex = term.promptForInput<uint32_t>(
                "Enter the number of the item to delete (0 to cancel)"
            );
            while (true) {
                if (delIndex == 0) {
                    cout << "Delete cancelled.\n";
                    break;
                } else if (delIndex > 0 && delIndex <= items.size()) {
                    break;
                }
                term.printError("Invalid choice `" + to_string((int) delIndex) +
                                "`. Number must be in range [1, " +
                                to_string(items.size()) + "]");
                delIndex = term.promptForInput<uint32_t>("Try again");
            }

            items.remove(delIndex - 1);
            cout << "Item deleted successfully.\n";
            break;
        }
        case 3: {
            // Edit Item
            auto items = lib.allInventory();

            if (items.size() == 0) {
                cout << "No items in inventory to edit.\n";
                break;
            }

            cout << "\n--- Current Inventory ---\n";
            for (size_t i = 0; i < items.size(); ++i) {
                cout << i + 1 << ". " << items[i].name << " by " << items[i].author << "\n";
            }

            int editIndex = term.promptForInput<uint32_t>(
                "Enter the number of the item to edit (0 to cancel)"
            );
            while (true) {
                if (editIndex == 0) {
                    cout << "Edit cancelled.\n";
                    return;
                } else if (editIndex > 0 && editIndex <= items.size()) {
                    break;
                }
                term.printError("Invalid choice `" + to_string((int) editIndex) +
                                "`. Number must be in range [1, " +
                                to_string(items.size()) + "]");
                editIndex = term.promptForInput<uint32_t>("Try again");
            }

            updateInventoryItem(items[editIndex - 1]);
            break;
        }
        case 4:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            UNREACHABLE;
    }
}

void Admin::editUserInfoAfterSearch(ResultList<User>& res) {
    cout << "\n--- User Management Menu ---\n";
    cout << "1. Update User Information\n";
    cout << "2. Delete User\n";
    cout << "3. Back to Admin Menu\n";
    int choice = term.promptForInput<uint32_t, validateNumRange<1, 3>>("Enter your choice");

    int userIndex = term.promptForInput<uint32_t>(
        "Enter the number of the user to update (0 to cancel)"
    );
    while (true) {
        if (userIndex == 0) {
            return;
            cout << "Operation cancelled.\n";
        } else if (userIndex > 0 && userIndex <= res.size()) {
            break;
        }
        userIndex = term.promptForInput<uint32_t>(
            "Invalid choice. Enter a valid user number"
        );
    }

    switch (choice) {
        case 1:
            updateUserInfo(res[userIndex - 1]);
            break;
        case 2:
            res.remove(userIndex - 1);
            cout << "User deleted successfully.\n";
            break;
        case 3:
            cout << "Returning to Admin Menu...\n";
            break;
        default:
            UNREACHABLE;
    }
}

void Admin::updateUserInfo(User& user) {
    term.printOptions("--- Select Fields to Update ---", {
        "ID",
        "Role",
        "First Name",
        "Last Name",
        "Address",
        "Phone Number",
        "Email",
        "Institution ID"
    });
    auto fields = term.promptForInput<
        vector<User::FieldTag>,
        validateNumRange<1, 8>
        >(
            "Enter space-separated list of fields to update"
        );

    for (auto& field : fields) {
        switch (field) {
            using enum User::FieldTag;
            case ID:
                user.id = term.promptForInput<uint64_t, validateLibraryID>("Enter new Library ID");
                break;
            case Role:
                user.role = term.promptForInput<string, validateRole>("Enter new role");
                break;
            case First:
                user.first = term.promptForInput<string>("Enter new first name");
                break;
            case Last:
                user.last = term.promptForInput<string>("Enter new last name");
                break;
            case Address:
                user.address = term.promptForInput<string>("Enter new address");
                break;
            case Phone:
                user.phone = term.promptForInput<string, validatePhone>(
                    "Enter new phone number"
                );
                break;
            case Email:
                user.email = term.promptForInput<string, validateEmail>("Enter new email");
                break;
            case InstitutionID:
                user.institutionId = term.promptForInput<uint64_t>("Enter new institution ID");
                break;
            default:
                UNREACHABLE;
        }
    }

    cout << "User information updated successfully.\n";
}

void Admin::showAllUsers() {
    using enum User::FieldTag;

    auto res = lib.allUsers();
    if (res.size() == 0) {
        cout << "No registered users.\n";
        return;
    }
    term.printTable(res, ID, Role, First, Last, Address, Phone,
                    Email, Password, InstitutionID, NumCheckedOut);
}

void Admin::updateInventoryItem(InventoryItem& item) {
    term.printOptions("--- Select Fields to Update ---", {
        "Type",
        "Name",
        "Author",
        "Publisher"
    });
    auto fields = term.promptForInput<
        vector<InventoryItem::FieldTag>,
        validateNumRange<1, 4>
        >(
            "Enter space-separated list of fields to update"
        );

    for (auto& field : fields) {
        switch (field) {
            using enum InventoryItem::FieldTag;
            case Type:
                item.type = term.promptForInput<string>("Enter new type");
                break;
            case Name:
                item.name = term.promptForInput<string>("Enter new name/title");
                break;
            case Author:
                item.author = term.promptForInput<string>("Enter new author");
                break;
            case Publisher:
                item.publisher = term.promptForInput<string>("Enter new publisher");
                break;
            default:
                UNREACHABLE;
        }
    }

    cout << "Item updated successfully.\n";
}
