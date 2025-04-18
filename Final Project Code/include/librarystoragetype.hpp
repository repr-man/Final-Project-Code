#pragma once

#include <concepts>
#include <filesystem>
#include <type_traits>

#include "inventoryitem.hpp"
#include "librarian.hpp"
#include "user.hpp"

template <typename T>
concept LibraryStorageType = (
    std::is_same_v<T, User> ||
    std::is_same_v<T, InventoryItem> ||
    std::is_same_v<T, Librarian>) &&
    requires (T t) {
        typename T::FieldTag;
        { T::Offset } -> std::convertible_to<int>;
        { T::SaveFileLocation } -> std::same_as<const std::filesystem::path&>;
    };
