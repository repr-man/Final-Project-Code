#pragma once

#include <cassert>
#include <cstdint>
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
#include <iostream>
#include <ostream>

class Library {
    std::vector<InventoryItem> inventory{};
    std::vector<User> users{};
    std::vector<HistoryItem> history{};
    std::vector<Librarian> librarians{};

    /// Reads the entire contents of a file into a buffer.
    //static std::string readFile(const std::string& filename);
    //

    template <typename T> requires IsLibraryStorageType<T>
    std::vector<T>* computeVectorFromOffset();

    std::string readFile(const std::filesystem::path& filename);

    /// Creates a view of `std::string_view`s that are split by a delimiter.
    static auto splitBy(const std::string_view text, const char delimiter);

    /// Implements the searching functionality for the earch methods.
    template <typename T> requires IsLibraryStorageType<T>
    ResultList<T> searchVector(
        std::vector<typename T::FieldTag>& fields,
        std::vector<std::string>& values,
        std::vector<T>& vec
    );

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
    ResultList<InventoryItem> allInventory();

    /// Returns a list of all the users in the library.
    ResultList<User> allUsers();

    /// Returns a list of all the users in the library.
    ResultList<HistoryItem> allHistory();

    /// Returns a list of all the users in the library.
    ResultList<Librarian> allLibrarians();

    // to add an inventory item
    bool addInventory(
        std::string&& type,
        std::string&& name, std::string&& author, std::string&& publisher, std::string&& borrowerID);

    void addHistory(uint64_t userID, std::string&& name);

    void addUser(
        uint64_t userID,
        std::string&& role,
        std::string&& first,
        std::string&& last,
        std::string&& address,
        std::string&& phone,
        std::string&& email,
        std::string&& password,
        std::string&& institutionId
    );

    template <typename T> requires IsLibraryStorageType<T>
    void remove(T* item);

    /// Writes the entire contents of a buffer to a file.
    template <typename T> requires IsLibraryStorageType<T>
    void flushVector();

    /// Writes all the contents to disk.
    void flush();
}; 
