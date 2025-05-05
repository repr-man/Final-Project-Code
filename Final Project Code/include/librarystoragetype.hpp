#pragma once

#include <array>
#include <concepts>
#include <filesystem>
#include <ranges>
#include <span>
#include <tuple>
#include <type_traits>
#include <vector>

class Row;

/// An interface for types that can provide information to a table.
/// `N` is the number of columns of data the type provides.
class LibraryStorageType {
public:
    virtual ~LibraryStorageType() {}
    virtual Row provideRow() const = 0;
};

template <typename T>
concept IsLibraryStorageType =
    std::is_base_of_v<LibraryStorageType, T> &&
    requires (T t) {
        typename T::FieldTag;
        { T::Offset } -> std::convertible_to<int>;
        { T::SaveFileLocation } -> std::same_as<const std::filesystem::path&>;
        requires requires (typename T::FieldTag tag) {
            { t.get(tag) } -> std::convertible_to<std::string>;
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

