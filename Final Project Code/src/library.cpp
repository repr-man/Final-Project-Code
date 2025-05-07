#include "library.hpp"
#include <algorithm>
#include <cstdint>
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
ResultList<T> Library::search(
    const std::vector<typename T::FieldTag>& fields,
    const std::vector<std::string>& values
) {
    assert(fields.size() == values.size());

    auto& vec = *computeVectorFromOffset<T>();
    std::vector<T*> results;
    for(auto& item : vec) {
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

ResultList<InventoryItem> Library::search(
    const std::vector<InventoryItem::FieldTag>& fields,
    const std::vector<std::string>& values
) { return search<InventoryItem>(fields, values); }

ResultList<User> Library::search(
    const std::vector<User::FieldTag>& fields,
    const std::vector<std::string>& values
) { return search<User>(fields, values); }

ResultList<HistoryItem> Library::search(
    const std::vector<HistoryItem::FieldTag>& fields,
    const std::vector<std::string>& values
) { return search<HistoryItem>(fields, values); }

ResultList<Librarian> Library::search(
    const std::vector<Librarian::FieldTag>& fields,
    const std::vector<std::string>& values
) { return search<Librarian>(fields, values); }

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
            std::stoull(std::string(segments[0])),
            std::string(segments[1]),
            std::string(segments[2]),
            std::string(segments[3]),
            std::string(segments[4]),
            std::string(segments[5]),
            std::string(segments[6]),
            std::string(segments[7]),
            std::stoull(std::string(segments[8])),
            std::stoul(std::string(segments[9]))
        ));
    }

    auto historyFileText = readFile(HistoryItem::SaveFileLocation);
    for(auto& line : splitBy(historyFileText, '\n')) {
        auto segments = splitBy(line, ';');
        history.push_back(HistoryItem(
            std::stoull(std::string(segments[0])),
            std::string(segments[1])
        ));
    }

    auto librariansFileText = readFile(Librarian::SaveFileLocation);
    for(auto& line : splitBy(librariansFileText, '\n')) {
        librarians.push_back(Librarian(
            std::stoull(std::string(line))
        ));
    }
}

template <typename T> requires IsLibraryStorageType<T>
ResultList<T> Library::all() {
    auto& libVec = *computeVectorFromOffset<T>();
    auto vec = std::vector<T*>();
    vec.reserve(libVec.size());
    for(int i = 0; i < libVec.size(); ++i) {
        vec.push_back(&libVec[i]);
    }
    return ResultList<T>(*this, std::move(vec));
}

template ResultList<InventoryItem> Library::all<InventoryItem>();
template ResultList<User> Library::all<User>();
template ResultList<HistoryItem> Library::all<HistoryItem>();
template ResultList<Librarian> Library::all<Librarian>();

// to add inventory
bool Library::addInventory(string&& type, string&& name, string&& author, string&& publisher, string&& borrowerID) {
    auto existing = search({InventoryItem::Name}, {name});
    if (existing.size() > 0) {
        return true;
    }
    InventoryItem newItem(std::move(type), std::move(name), std::move(author), std::move(publisher), std::stoi(borrowerID));
    inventory.push_back(newItem);
    flushVector<InventoryItem>();
    return false;
}

void Library::addHistory(uint64_t userID, std::string&& name) {
    auto newItem = HistoryItem(userID, std::move(name));
    history.push_back(newItem);
    flushVector<HistoryItem>();
}

void Library::addUser(
    uint64_t userID,
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
        std::move(std::stoull(institutionId)),
        0
    );
    users.push_back(newItem);
    flushVector<User>();
}

void Library::addLibrarian(uint64_t userID) {
    auto newItem = Librarian(userID);
    librarians.push_back(newItem);
    flushVector<Librarian>();
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
