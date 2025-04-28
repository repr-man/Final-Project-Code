#pragma once

#include <cassert>
#include <filesystem>
#include <ranges>
#include <string_view>
#include <vector>
#include "inventoryitem.hpp"
#include "history.hpp"
#include "librarian.hpp"
#include "librarystoragetype.hpp"
#include "resultlist.hpp"
#include "user.hpp"

class Library {
    std::vector<InventoryItem> inventory{};
    std::vector<User> users{};
    std::vector<HistoryItem> history{};
    std::vector<Librarian> librarians{};

    /// Reads the entire contents of a file into a buffer.
    static std::string readFile(const std::filesystem::path& filename);

    /// Creates a view of `std::string_view`s that are split by a delimiter.
    static auto splitBy(const std::string_view text, const char delimiter);

    /// Implements the searching functionality for the earch methods.
    template <typename T> requires LibraryStorageType<T>
    ResultList<T> searchVector(
        std::vector<typename T::FieldTag>& fields,
        std::vector<std::string>& values,
        std::vector<T>& vec
    );

    template <typename T> requires LibraryStorageType<T>
    void remove(T* item);

    friend class ResultList<InventoryItem>;
    friend class ResultList<User>;
    friend class ResultList<HistoryItem>;
    friend class ResultList<Librarian>;

public:
	Library();

    ~Library();

    /// Searches the library for inventory items based on the given fields.
    ResultList<InventoryItem> search(
        std::vector<InventoryItem::FieldTag> fields,
        std::vector<std::string> values
    );

    /// Searches the library for users based on the given fields.
    ResultList<User> search(
        std::vector<User::FieldTag> fields,
        std::vector<std::string> values
    );

    /// Searches the library for history items based on the given fields.
    ResultList<HistoryItem> search(
        std::vector<HistoryItem::FieldTag> fields,
        std::vector<std::string> values
    );

    /// Searches the library for librarians based on the given fields.
    ResultList<Librarian> search(
        std::vector<Librarian::FieldTag> fields,
        std::vector<std::string> values
    );


    /// Returns a list of all the inventory items in the library.
    ResultList<InventoryItem> allInventory() {
        auto vec = std::vector<InventoryItem*>();
        vec.reserve(inventory.size());
        for(int i = 0; i < inventory.size(); ++i) {
            vec.push_back(&inventory[i]);
        }
        return ResultList<InventoryItem>(*this, std::move(vec));
    }

    // to add an inventory item
    void addInventory(std::string&& type, std::string&& name, std::string&& author, std::string&& publisher, std::string&& borrowerID);

    void addHistory(long userID, std::string&& name);
    
    /// Writes the entire contents of a buffer to a file.
    template <typename T> requires LibraryStorageType<T>
    void flushVector();

    /// Writes all the contents to disk.
    void flush();
}; 
