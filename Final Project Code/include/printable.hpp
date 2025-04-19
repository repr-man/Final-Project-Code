#pragma once

#include <array>
#include <string>

/// An interface for types that can provide information to a table.
/// `N` is the number of columns of data the type provides.
template <int N>
class Printable {
public:
    virtual ~Printable() {}
    virtual std::array<std::string, N> providePrintableData() const = 0;
};
