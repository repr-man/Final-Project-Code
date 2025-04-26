#pragma once

#include <filesystem>
#include <string>

#include "printable.hpp"

class Librarian : public Printable<3> {
public:
    std::string first, last, password;

    static constexpr int Offset = 2;
    inline static const auto SaveFileLocation
        = std::filesystem::path(Printable<3>::SaveFileLocation) / "librarians.txt";

    enum class FieldTag {
        First,
        Last,
        Password
    };

    static constexpr auto First = FieldTag::First;
    static constexpr auto Last = FieldTag::Last;
    static constexpr auto Password = FieldTag::Password;

    Librarian(
        std::string&& first,
        std::string&& last,
        std::string&& password
    );

    bool matches(FieldTag field, const std::string& value) const;

    std::string serialize() const;

    std::array<std::string, 3> providePrintableData() const override;
    
    friend class Library;
};
