#include "library.hpp"
#include "zip_view.hpp"
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;

/// Reads the entire contents of a file into a buffer.
std::string Library::readFile(const std::string& filename) {
    auto path = std::filesystem::current_path().append(filename);
    std::ifstream file = std::ifstream(path);
    return std::string(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

// Creates a view of `std::string_view`s that are split by a delimiter.
auto Library::splitBy(const std::string_view text, const char delimiter) {
    return std::views::split(text, delimiter)
    | std::views::transform([](auto&& it){
        auto [start, end] = it;
        return std::string_view(start, end);
    })
    | std::views::filter([](auto&& it){ return !it.empty(); });
}

template <typename T> requires LibraryStorageType<T>
void Library::flushVector() {
    auto vec = (std::vector<T>*)(this) + T::Offset;
    auto file = std::ofstream(T::SaveFileLocation);
    for(auto& item : *vec) {
        file << item.serialize() << '\n';
    }
    file.close();
}

template <typename T> requires LibraryStorageType<T>
ResultList<T> Library::searchVector(
    std::vector<typename T::FieldTag>& fields,
    std::vector<std::string>& values,
    std::vector<T>& vec
) {
    assert(fields.size() == values.size());

    std::vector<T*> results;
    for(auto& item: vec) {
        for(const auto& [field, value] : c9::zip(fields, values)) {
            if(!item.matches(field, value)) {
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
    for(auto line : splitBy(bookFileText, '\n')) {
        auto segments = splitBy(line, ';');
        auto it = segments.begin();
        inventory.push_back(InventoryItem(
            std::string(*it++),
            std::string(*it++),
            std::string(*it++),
            std::string(*it++),
            std::stoi(std::string(*it++))
        ));
    }

    auto usersFileText = readFile(User::SaveFileLocation);
    for(auto line : splitBy(usersFileText, '\n')) {
        auto segments = splitBy(line, ';');
        auto it = segments.begin();
        users.push_back(User(
            std::stol(std::string(*it++)),
            std::string(*it++),
            std::string(*it++),
            std::string(*it++),
            std::string(*it++),
            std::string(*it++),
            std::string(*it++),
            std::string(*it++),
            std::stol(std::string(*it++)),
            std::stoi(std::string(*it++))
        ));
    }

    auto historyFileText = readFile(HistoryItem::SaveFileLocation);
    for(auto line : splitBy(historyFileText, '\n')) {
        auto segments = splitBy(line, ';');
        auto it = segments.begin();
        history.push_back(HistoryItem(
            std::stol(std::string(*it++)),
            std::string(*it++)
        ));
    }

    auto librariansFileText = readFile(Librarian::SaveFileLocation);
    for(auto line : splitBy(librariansFileText, '\n')) {
        auto segments = splitBy(line, ';');
        auto it = segments.begin();
        librarians.push_back(Librarian(
            std::string(*it++),
            std::string(*it++),
            std::string(*it++)
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

// to add inventory
void Library::addInventory(string&& type, string&& name, string&& author, string&& publisher, string&& borrowerID) {
    InventoryItem newItem(
        std::move(type),
        std::move(name),
        std::move(author),
        std::move(publisher),
        std::stoi(borrowerID)
    );
    inventory.push_back(newItem);

}

template <typename T> requires LibraryStorageType<T>
void Library::remove(T* item) {
    auto vec = (std::vector<T>*)(this) + T::Offset;
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

