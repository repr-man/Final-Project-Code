#include "library.hpp"
#include <algorithm>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

using namespace std;

template <typename T> requires IsLibraryStorageType<T>
std::vector<T>* Library::computeVectorFromOffset() {
    return (std::vector<T>*)(this) + T::Offset;
}

/// Reads the entire contents of a file into a buffer.
std::string Library::readFile(const std::filesystem::path& filename) {
    //auto path = std::filesystem::current_path().append(filename);
    std::ifstream file = std::ifstream(filename);
    return std::string(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

// Creates a view of `std::string_view`s that are split by a delimiter.
auto Library::splitBy(const std::string_view text, const char delimiter) {
    std::vector<std::string_view> res;
    int pos = 0;
    auto end = text.find(delimiter, pos);
    while (pos < text.size() && end != std::string_view::npos) {
        if (end - pos > 0) {
            res.push_back(text.substr(pos, end - pos));
        }
        pos = end + 1;
        end = text.find(delimiter, pos);
    }
    if (text.substr(pos).size() > 0) {
        res.push_back(text.substr(pos));
    }
    return res;
}

template <typename T> requires IsLibraryStorageType<T>
void Library::flushVector() {
    auto vec = (std::vector<T>*)(this) + T::Offset;
    auto file = std::ofstream(T::SaveFileLocation);
    for(auto& item : *vec) {
        file << item.serialize() << '\n';
    }
    file.close();
}


template <typename T> requires IsLibraryStorageType<T>
ResultList<T> Library::searchVector(
    std::vector<typename T::FieldTag>& fields,
    std::vector<std::string>& values,
    std::vector<T>& vec
) {
    assert(fields.size() == values.size());

    std::vector<T*> results;
    for(auto& item: vec) {
        for (int i = 0; i < fields.size();i++) {
            if (!item.matches(fields[i], values[i])) {
                goto end;
            }
        }
        results.push_back(&item);
    end:;
    }

    return ResultList(*this, std::move(results));
}

Library::Library() {
    auto bookFileText = readFile(InventoryItem::SaveFileLocation);
    for(auto& line : splitBy(bookFileText, '\n')) {
        auto segments = splitBy(line, ';');
        inventory.push_back(InventoryItem(
            std::string(segments[0]),
            std::string(segments[1]),
            std::string(segments[2]),
            std::string(segments[3]),
            std::stoi(std::string(segments[4]))
        ));
    }

    auto usersFileText = readFile(User::SaveFileLocation);
    for(auto& line : splitBy(usersFileText, '\n')) {
        auto segments = splitBy(line, ';');
        users.push_back(User(
            std::stoll(std::string(segments[0])),
            std::string(segments[1]),
            std::string(segments[2]),
            std::string(segments[3]),
            std::string(segments[4]),
            std::string(segments[5]),
            std::string(segments[6]),
            std::string(segments[7]),
            std::stoll(std::string(segments[8])),
            std::stoi(std::string(segments[9]))
        ));
    }

    auto historyFileText = readFile(HistoryItem::SaveFileLocation);
    for(auto& line : splitBy(historyFileText, '\n')) {
        auto segments = splitBy(line, ';');
        history.push_back(HistoryItem(
            std::stoll(std::string(segments[0])),
            std::string(segments[1])
        ));
    }

    auto librariansFileText = readFile(Librarian::SaveFileLocation);
    for(auto& line : splitBy(librariansFileText, '\n')) {
        auto segments = splitBy(line, ';');
        librarians.push_back(Librarian(
            std::string(segments[0]),
            std::string(segments[1]),
            std::string(segments[2])
        ));
    }
}



/// Searches the library for inventory items based on the given fields.
ResultList<InventoryItem> Library::search(
    std::vector<InventoryItem::FieldTag> fields,
    std::vector<std::string> values
) {
    return searchVector(fields, values, inventory);
}

/// Searches the library for users based on the given fields.
ResultList<User> Library::search(
    std::vector<User::FieldTag> fields,
    std::vector<std::string> values
) {
    return searchVector(fields, values, users);
}

/// Searches the library for history items based on the given fields.
ResultList<HistoryItem> Library::search(
    std::vector<HistoryItem::FieldTag> fields,
    std::vector<std::string> values
) {
    return searchVector(fields, values, history);
}

/// Searches the library for librarians based on the given fields.
ResultList<Librarian> Library::search(
    std::vector<Librarian::FieldTag> fields,
    std::vector<std::string> values
) {
    return searchVector(fields, values, librarians);
}

ResultList<InventoryItem> Library::allInventory() {
    auto vec = std::vector<InventoryItem*>();
    vec.reserve(inventory.size());
    for(int i = 0; i < inventory.size(); ++i) {
        vec.push_back(&inventory[i]);
    }
    return ResultList<InventoryItem>(*this, std::move(vec));
}

ResultList<User> Library::allUsers() {
    auto vec = std::vector<User*>();
    vec.reserve(users.size());
    for(int i = 0; i < users.size(); ++i) {
        vec.push_back(&users[i]);
    }
    return ResultList<User>(*this, std::move(vec));
}

// to add inventory
void Library::addInventory(string&& type, string&& name, string&& author, string&& publisher, string&& borrowerID) {
    InventoryItem newItem(std::move(type), std::move(name), std::move(author), std::move(publisher), std::stoi(borrowerID));
    inventory.push_back(newItem);
    flushVector<InventoryItem>();
}

void Library::addHistory(long userID, std::string&& name) {
    auto newItem = HistoryItem(userID, std::move(name));
    history.push_back(newItem);
    flushVector<HistoryItem>();
}

void Library::addUser(
    long userID,
    std::string&& role,
    std::string&& first,
    std::string&& last,
    std::string&& address,
    std::string&& phone,
    std::string&& email,
    std::string&& password,
    std::string&& institutionId
) {
    auto newItem = User(
        userID,
        std::move(role),
        std::move(first),
        std::move(last),
        std::move(address),
        std::move(phone),
        std::move(email),
        std::move(password),
        std::move(std::stoll(institutionId)),
        0
    );
    users.push_back(newItem);
    flushVector<User>();
}


template <typename T> requires IsLibraryStorageType<T>
void Library::remove(T* item) {
    auto vec = computeVectorFromOffset<T>();
    int idx = item - &*vec->cbegin();
    vec->erase(vec->begin() + idx);
}

template void Library::remove<InventoryItem>(InventoryItem* item);
template void Library::remove<User>(User* item);
template void Library::remove<HistoryItem>(HistoryItem* item);
template void Library::remove<Librarian>(Librarian* item);

Library::~Library() { 
    flush();
}

// Writes all the contents to disk.
void Library::flush() {
    Library::flushVector<InventoryItem>();
    Library::flushVector<User>();
    Library::flushVector<HistoryItem>();
    Library::flushVector<Librarian>();
}
