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

public:
	Library();

    ~Library();

    /// Implements the searching functionality for the earch methods.
    template <typename T> requires IsLibraryStorageType<T>
    ResultList<T> search(
        const std::vector<typename T::FieldTag>& fields,
        const std::vector<std::string>& values
    );

    ResultList<InventoryItem> search(
        const std::vector<InventoryItem::FieldTag>& fields,
        const std::vector<std::string>& values
    );
    ResultList<User> search(
        const std::vector<User::FieldTag>& fields,
        const std::vector<std::string>& values
    );
    ResultList<HistoryItem> search(
        const std::vector<HistoryItem::FieldTag>& fields,
        const std::vector<std::string>& values
    );
    ResultList<Librarian> search(
        const std::vector<Librarian::FieldTag>& fields,
        const std::vector<std::string>& values
    );

    template <typename T> requires IsLibraryStorageType<T>
    ResultList<T> all();

    // to add an inventory item
    bool addInventory(
        std::string&& type,
        std::string&& name,
        std::string&& author,
        std::string&& publisher,
        std::string&& borrowerID
    );

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

    void addLibrarian(uint64_t userID);

    template <typename T> requires IsLibraryStorageType<T>
    void remove(T* item);

    /// Writes the entire contents of a buffer to a file.
    template <typename T> requires IsLibraryStorageType<T>
    void flushVector();

    /// Writes all the contents to disk.
    void flush();
}; 
