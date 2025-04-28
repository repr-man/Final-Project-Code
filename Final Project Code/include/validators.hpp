#pragma once

#include <cctype>
#include <ranges>
#include <string>
#include <string_view>

constexpr int eatIPv4Segment(std::string_view str);

template <int lo, int hi>
const std::string validateNumRange(std::string_view str) {
    try {
        int val = std::stoi(std::string(str));
        if (val < lo || val > hi) {
            auto low = std::to_string(lo);
            auto high = std::to_string(hi);
            return "Input must be a number in range [" + low + "-" + high + "].";
        }
        return "";
    } catch (...) {
        return "Input must be a number.";
    }
}

const std::string validateIdentity(std::string_view str);
const std::string validateLibraryID(std::string_view str);
const std::string validateRole(std::string_view role);
const std::string validateInstitutionID(std::string_view str);
const std::string validatePhone(std::string value);
const std::string validateEmail(std::string_view e);
