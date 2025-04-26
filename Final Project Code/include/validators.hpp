#pragma once

#include <cctype>
#include <filesystem>
#include <ranges>
#include <string>
#include <string_view>

constexpr std::string_view validateIdentity(std::string_view str) {
    return "";
}

template <int lo, int hi>
constexpr std::string_view validateNumRange(std::string_view str) {
    int val = std::stoi(std::string(str));
    if (val < lo || val > hi) {
        constexpr static auto low = std::to_string(lo);
        constexpr static auto high = std::to_string(hi);
        constexpr static auto res = "Input must be a number in range [" + low + "-" + high + "].";
        return res;
    }
}

constexpr std::string_view validateLibraryID(std::string_view str) {
    if (str.size() < 10) {
        return "Library ID must be at least 10 digits.";
    } else if (str.size() > 10) {
        return "Library ID must be at most 10 digits.";
    }
    for (auto c : str) {
        if (!std::isdigit(c)) {
            return "Library ID must be 10 numbers (0-9).";
        }
    }
    return "";
}

constexpr std::string_view validateRole(std::string_view role) {
    if (role != "student" && role != "faculty" && role != "staff") {
        return "Role must be one of: 'student', 'faculty', 'staff'.";
    }
    return "";
}

constexpr std::string_view validateInstitutionID(std::string_view str) {
    for (auto c : str) {
        if (!std::isdigit(c)) {
            return "Institution ID must be a number.";
        }
    }
    return "";
}

constexpr std::string_view validatePhone(std::string value) {
    if (
        value.size() != 12
        || !std::isdigit(value[0])
        || !std::isdigit(value[1])
        || !std::isdigit(value[2])
        ||  value[3] != '-'
        || !std::isdigit(value[4])
        || !std::isdigit(value[5])
        || !std::isdigit(value[6])
        ||  value[7] != '-'
        || !std::isdigit(value[8])
        || !std::isdigit(value[9])
        || !std::isdigit(value[10])
        || !std::isdigit(value[11])
    ) {
        return "Phone number does not have the format '###-###-####'.";
    }
    return "";
}

constexpr int eatIPv4Segment(std::string_view str) {
    for (int i = 0; i < 3; ++i) {
        if (str[i] == '.') {
            return i;
        } else if (!std::isdigit(str[i])) {
            return -1;
        }
    }
    return 3;
}

constexpr std::string_view validateEmail(std::string_view e) {
    auto em = e;
    if (em.size() == 0) {
        return "Email must not be empty.";
    }

    auto sectionPos = 0;
    bool hasComment = false;

    // Comment
    if (em[sectionPos] == '(') {
        auto pos = em.find(')');
        if (pos == std::string_view::npos) {
            return "Email comment must be enclosed in parentheses.";
        }
        hasComment = true;
        sectionPos = pos + 1;
    }

    // Local-part
    auto localPartStart = sectionPos;
    if (em[sectionPos] == '"') {
        auto pos = em.find('"', 1);
        do {
            if (pos == std::string_view::npos) {
                return "Email local-part must be enclosed in quotes.";
            } else if (pos == 1) {
                return "Email local-part must not be empty.";
            } else if (em[pos - 1] == '\\') {
                // Count the number of backslashes preceding the quote.
                auto count = 1;
                while (em[pos - count - 1] == '\\') {
                    ++count;
                }
                // If the number of backslashes is odd, then the quote is escaped.
                if (count & 1) {
                    pos = em.find('"', pos + 1);
                    continue;
                }
                // Else, we exit the loop.
            }
        } while (false);
        sectionPos = pos + 1;
    } else {
        if (em[sectionPos] == '.') {
            return "Email local-part must not start with a period.";
        }
        bool sawPeriod = false;
        bool done = false;
        for (int i = sectionPos; i < em.size() && !done; ++i) {
            switch (em[i]) {
                case '!':
                case '#':
                case '$':
                case '%':
                case '&':
                case '\'':
                case '*':
                case '+':
                case '-':
                case '/':
                case '=':
                case '?':
                case '^':
                case '_':
                case '`':
                case '{':
                case '|':
                case '}':
                case '~':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'A':
                case 'Z':
                case 'a':
                case 'z':
                    sawPeriod = false;
                    break;
                case '(':
                case '@':
                    if (sawPeriod) {
                        return "Email local-part may not end with a period.";
                    }
                    sectionPos = i;
                    done = true;
                    break;
                case '.':
                    if (sawPeriod) {
                        return "Email local-part may not contain multiple consecutive periods.";
                    }
                    sawPeriod = true;
                    break;
                case '\0':
                    return "Email local-part must contain '@domain'.";
                default:
                    if ((em[i] >= 'a' && em[i] <= 'z') || (em[i] >= 'A' && em[i] <= 'Z')) {
                        sawPeriod = false;
                        break;
                    }
                    return "Invalid character in email local-part.";
            }
        }
    }
    if (sectionPos - localPartStart > 64) {
        return "Email local-part must be at most 64 characters long.";
    }

    // Comment
    if (em[sectionPos] == '(') {
        if (hasComment) {
            return "Email may not have multiple comments.";
        } 

        auto pos = em.find(')', sectionPos);
        if (pos == std::string_view::npos) {
            return "Email comment must be enclosed in parentheses.";
        }
        sectionPos = pos + 1;
    }

    // Domain
    if (em[sectionPos] != '@') {
        return "Email must contain '@domain'.";
    }
    ++sectionPos;

    if (em[sectionPos] == '\0') {
        return "Email must contain a domain.";
    }

    if (em.substr(sectionPos).starts_with("[IPV6")) {
        return "IPv6 addresses not yet supported.";
    } else if (em[sectionPos] == '[') {
        // IPv4 address
        ++sectionPos;
        auto segments = 0;
        while (segments < 4) {
            int delta = eatIPv4Segment(em.substr(sectionPos));
            if (delta < 1) {
                return "Invalid IPv4 in email address.";
            }
            sectionPos += delta;
            ++segments;
        }
        if (em[sectionPos] != ']') {
            return "Missing closing bracket in IPv4 in email address.";
        }
        ++sectionPos;
        if (sectionPos != em.size()) {
            return "Extra characters after closing bracket in IPv4 in email address.";
        }
    } else {
        // LDH
        auto domain = em.substr(sectionPos);
        if (domain.size() > 253) {
            return "Email domain must be at most 253 characters long.";
        }

        try {
            auto tmp = std::stoi(std::string(domain));
            return "Email domain must not be a number.";
        } catch (...) {}

        for (auto label : domain | std::views::split('.')) {
            if (label.size() > 63) {
                return "Email label must be at most 63 characters long.";
            }
            if (label.size() == 0) {
                return "Email label must not be empty.";
            }
            if (label[0] == '-' || label[label.size() - 1] == '-') {
                return "Email label must not start or end with a hyphen.";
            }
            for (auto c : label) {
                switch (c) {
                case 'a':
                    case 'z':
                    case 'A':
                    case 'Z':
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '-':
                        break;
                    default:
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                            break; 
                        }
                        return "Invalid character in email label.";
                }
            }
        }
    }
    return "";
}
