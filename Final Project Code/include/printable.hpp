#pragma once

#include <array>
#include <filesystem>
#include <string>

/// An interface for types that can provide information to a table.
/// `N` is the number of columns of data the type provides.
template <int N>
class Printable {
public:
    #ifdef _WIN32
    // Visual Studio path.
    inline static const auto SaveFileLocation
        = std::filesystem::current_path() / "data";
    #else
    // Ninja path.
    inline static const auto SaveFileLocation
        = std::filesystem::current_path() / "Final Project Code/data";
    #endif

    virtual ~Printable() {}
    virtual std::array<std::string, N> providePrintableData() const = 0;
};
