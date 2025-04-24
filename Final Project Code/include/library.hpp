#pragma once

#include <cassert>
#include <filesystem>
#include <string_view>
#include <vector>

#include "inventoryitem.hpp"
#include "librarian.hpp"
#include "librarystoragetype.hpp"
#include "resultlist.hpp"
#include "user.hpp"

class Library {
    std::vector<InventoryItem> inventory{};
    std::vector<User> users{};
    std::vector<Librarian> librarians{};

    /// Reads the entire contents of a file into a buffer.
    static std::string readFile(const std::string& filename);

    /// Creates a view of `std::string_view`s that are split by a delimiter.
    static auto splitBy(const std::string_view text, const char delimiter);

    /// Implements the searching functionality for the earch methods.
    template <typename T> requires LibraryStorageType<T>
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

    /// Searches the library for librarians based on the given fields.
    ResultList<Librarian> search(
        std::vector<Librarian::FieldTag> fields,
        std::vector<std::string> values
    );

    void addInventory(const std::string& type, const std::string& name, const std::string& author, const std::string& publisher, const std::string& borrowerID);

    /// Writes the entire contents of a buffer to a file.
    template <typename T> requires LibraryStorageType<T>
    void flushVector();

    /// Writes all the contents to disk.
    void flush();
}; 
