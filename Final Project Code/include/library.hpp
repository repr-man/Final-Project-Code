#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <string_view>
#include <vector>

#include "inventoryitem.hpp"
#include "user.hpp"
#include "librarian.hpp"
#include "zip_view.hpp"

class Library {
    std::vector<InventoryItem> inventory{};
    std::vector<User> users{};
    std::vector<Librarian> librarians{};

    /// Reads the entire contents of a file into a buffer.
    static std::string readFile(const std::string& filename) {
        auto path = std::filesystem::current_path().append(filename);
        auto file = std::ifstream(path);
        return std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
    }

    // Creates a view of `std::string_view`s that are split by a delimiter.
    static auto splitBy(const std::string_view text, const char delimiter) {
        return std::views::split(text, delimiter)
        | std::views::transform([](auto&& it){
            auto [start, end] = it;
            return std::string_view(start, end);
        })
        | std::views::filter([](auto&& it){ return !it.empty(); });
    }

public:
	Library() {
        auto bookFileText = readFile("Final Project Code/data/book.txt");
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

        auto usersFileText = readFile("Final Project Code/data/users.txt");
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

        auto librariansFileText = readFile("Final Project Code/data/librarians.txt");
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

    auto search(
        std::vector<InventoryItem::FieldTag> fields,
        std::vector<std::string> values
    ) const {
        assert(fields.size() == values.size());

        std::vector<InventoryItem> results;
        for(auto& item : inventory) {
            for(const auto& [field, value] : c9::zip(fields, values)) {
                if(!item.matches(field, value)) {
                    break;
                }
                results.push_back(item);
            }
        }
        return results;
        end:;
    }

    auto search(
        std::vector<User::FieldTag> fields,
        std::vector<std::string> values
    ) const {
        assert(fields.size() == values.size());

        std::vector<User> results;
        for(auto& user : users) {
            for(const auto& [field, value] : c9::zip(fields, values)) {
                if(!user.matches(field, value)) goto end;
            }
            results.push_back(user);
        end:;
        }
        return results;
    }

    auto search(
        std::vector<Librarian::FieldTag> fields,
        std::vector<std::string> values
    ) const {
        assert(fields.size() == values.size());

        std::vector<Librarian> results;
        for(auto& librarian : librarians) {
            for(const auto& [field, value] : c9::zip(fields, values)) {
                if(!librarian.matches(field, value)) {
                    goto end;
                }
            }
            results.push_back(librarian);
        end:;
        }

        return results;
    }
}; 
