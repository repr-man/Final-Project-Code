#pragma once

#include <array>
#include <concepts>
#include <iostream>
#include <filesystem>
#include <ranges>
#include <span>
#include <tuple>
#include <type_traits>
#include <vector>

class Row;

/// An interface for types that are stored in a `Library` and can provide
/// rows of information to a table.
class LibraryStorageType {
public:
    virtual ~LibraryStorageType() {}
    virtual Row provideRow() const = 0;
    virtual std::string serialize() const = 0;
};

template <typename T>
concept IsLibraryStorageType =
    std::is_base_of_v<LibraryStorageType, T> &&
    requires (T t) {
        { T::Offset } -> std::convertible_to<int>;
        { T::SaveFileLocation } -> std::same_as<const std::filesystem::path&>;

        requires std::is_enum_v<typename T::FieldTag>;
        requires requires (typename T::FieldTag tag) {
            { t.get(tag) } -> std::same_as<std::string>;
            { t.matches(tag, std::string{}) } -> std::same_as<bool>;
            { T::to_string(tag) } -> std::same_as<std::string>;
            { std::cout << tag } -> std::same_as<std::ostream&>;
            { std::cin >> tag } -> std::same_as<std::istream&>;
        };
    };


class Row {
    std::vector<std::string> items;

public:
    Row(std::vector<std::string>&& items) : items(items) {}
    
    template <typename T, typename... FieldTags>
    requires IsLibraryStorageType<T> && (std::same_as<FieldTags, typename T::FieldTag> && ...)
    auto get(FieldTags... fields) const {
        auto tmp = std::array{fields...} | std::views::transform([&](auto field) {
            return items[static_cast<int>(field)];
        });
        return std::vector<std::string>(tmp.begin(), tmp.end());
    }
};

