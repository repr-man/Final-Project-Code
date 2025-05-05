#include "Borrowing.hpp"


void Borrowing::borrowBook(Library& lib, Terminal& term) {
    string firstName = term.promptForInput<string>("Enter First Name");
    string lastName = term.promptForInput<string>("Enter Last Name");
    auto user = lib.search({User::First, User::Last}, {firstName, lastName});
    if (user.size() == 0) {
        term.printError("User not found in system.");
        return;
    }

    cout << "\nUser Found:\n";
    cout << "ID: " << user[0].id << "\n";
    cout << "Name: " << user[0].first << " " << user[0].last << "\n";
    cout << "Email: " << user[0].email << "\n";
    cout << "Phone: " << user[0].phone << "\n";
    cout << "User Type: " << user[0].role << "\n";

    if (user[0].numCheckedOut >= 4) {
        cout << "This user has already borrowed 4 books.\n";
        return;
    }

    auto bookTitle = term.promptForInput<string>("Enter the title of the book to borrow");
    auto item = lib.search({InventoryItem::Name}, {bookTitle});
    if (item.size() == 0) {
        term.printError("Book not found.");
        return;
    } else if (item[0].borrowerID != -1) {
        term.printError("Book is already borrowed by another user.");
        return;
    }

    user[0].numCheckedOut += 1;
    item[0].borrowerID = user[0].id;
    lib.addHistory(user[0].id, std::move(bookTitle));

    cout << "Book successfully borrowed by User " << user[0].first << " " << user[0].last << ". Total books borrowed: " << user[0].numCheckedOut << "\n";
}

template <typename T>
requires std::is_base_of_v<UserHolder, T>
void Borrowing::borrowItems(Library& lib, Terminal& term, T& user, ResultList<InventoryItem>& items) {
    bool doneBorrowing = false;
    while (!doneBorrowing) {
        auto toBorrow = term.promptForInput<vector<uint32_t>>(
            "Enter the #s of items to borrow (0 to cancel)"
        );
        if (toBorrow.size() > 4) {
            term.printError("You may not borrow more than 4 items.");
            continue;
        }
        bool shouldContinue = false;
        for (auto i : toBorrow) {
            if (i == 0) {
                doneBorrowing = true;
                break;
            }
            if (i > items.size()) {
                term.printError("Invalid item number `" + to_string((int) i) + "`.");
                shouldContinue = true;
                break;
            }
            if (items[i - 1].borrowerID != -1) {
                term.printError("Item `" + items[i - 1].name + "` is already borrowed.");
                shouldContinue = true;
                break;
            }
        }
        if (doneBorrowing) { break; }
        if (shouldContinue) { continue; }

        if constexpr (std::is_same_v<T, UserLogin>) {
            if (user.user.numCheckedOut >= 4) {
                term.printError("You have already borrowed 4 books.");
                return;
            }
            auto first = term.promptForInput<string>("Enter admin first name");
            auto last = term.promptForInput<string>("Enter admin last name");
            auto password = term.promptForInput<string>("Enter admin password");
            auto usersRes = lib.search(
                {User::First, User::Last, User::Password},
                {first, last, password}
            );
            if (usersRes.size() == 0) {
                term.printError("Invalid admin credentials.");
                continue;
            }
            auto librariansRes = lib.search(
                {Librarian::Id},
                {std::to_string(usersRes[0].id)}
            );
            if (librariansRes.size() == 0) {
                term.printError("User is not an admin.");
                continue;
            }
        }

        for (auto i : toBorrow) {
            items[i - 1].borrowerID = user.user.id;
            user.user.numCheckedOut += 1;
            lib.flushVector<User>();
            auto name = items[i - 1].name;
            lib.addHistory(user.user.id, std::move(name));
        }
        break;
    }
}

template void Borrowing::borrowItems<UserLogin>(
    Library& lib, Terminal& term, UserLogin& user, ResultList<InventoryItem>& items
);
template void Borrowing::borrowItems<Admin>(
    Library& lib, Terminal& term, Admin& user, ResultList<InventoryItem>& items
);
