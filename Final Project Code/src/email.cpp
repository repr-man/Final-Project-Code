#include "email.hpp"
#include "terminal.hpp"

#include <ranges>
#include <string>
#include <string_view>

int Email::eatIPv4Segment(std::string_view str) {
    for (int i = 0; i < 3; ++i) {
        if (str[i] == '.') {
            return i;
        } else if (!std::isdigit(str[i])) {
            return -1;
        }
    }
    return 3;
}

Email::Email(std::string&& e) : email(e) {
    auto em = std::string_view(email);
    if (em.size() == 0) {
        throw Terminal::InvalidInput("Email must not be empty.");
    }

    auto sectionPos = 0;
    bool hasComment = false;

    // Comment
    if (em[sectionPos] == '(') {
        auto pos = em.find(')');
        if (pos == std::string_view::npos) {
            throw Terminal::InvalidInput("Email comment must be enclosed in parentheses.");
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
                throw Terminal::InvalidInput("Email local-part must be enclosed in quotes.");
            } else if (pos == 1) {
                throw Terminal::InvalidInput("Email local-part must not be empty.");
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
            throw Terminal::InvalidInput("Email local-part must not start with a period.");
        }
        bool sawPeriod = false;
        for (int i = sectionPos; i < em.size(); ++i) {
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
                case '0' ... '9':
                case 'A' ... 'Z':
                case 'a' ... 'z':
                    sawPeriod = false;
                    break;
                case '(':
                case '@':
                    if (sawPeriod) {
                        throw Terminal::InvalidInput(
                            "Email local-part may not end with a period."
                        );
                    }
                    sectionPos = i;
                    goto doneLocalPart;
                case '.':
                    if (sawPeriod) {
                        throw Terminal::InvalidInput(
                            "Email local-part may not contain multiple consecutive periods."
                        );
                    }
                    sawPeriod = true;
                    break;
                case '\0':
                    throw Terminal::InvalidInput("Email local-part must contain '@domain'.");
                default:
                    throw Terminal::InvalidInput("Invalid character in email local-part.");
            }
        }
    }
    doneLocalPart:
    if (sectionPos - localPartStart > 64) {
        throw Terminal::InvalidInput("Email local-part must be at most 64 characters long.");
    }

    // Comment
    if (em[sectionPos] == '(') {
        if (hasComment) {
            throw Terminal::InvalidInput("Email may not have multiple comments.");
        } 

        auto pos = em.find(')', sectionPos);
        if (pos == std::string_view::npos) {
            throw Terminal::InvalidInput("Email comment must be enclosed in parentheses.");
        }
        sectionPos = pos + 1;
    }

    // Domain
    if (em[sectionPos] != '@') {
        throw Terminal::InvalidInput("Email must contain '@domain'.");
    }
    ++sectionPos;

    if (em[sectionPos] == '\0') {
        throw Terminal::InvalidInput("Email must contain a domain.");
    }

    if (em.substr(sectionPos).starts_with("[IPV6")) {
        throw Terminal::InvalidInput("IPv6 addresses not yet supported.");
    } else if (em[sectionPos] == '[') {
        // IPv4 address
        ++sectionPos;
        auto segments = 0;
        while (segments < 4) {
            int delta = eatIPv4Segment(em.substr(sectionPos));
            if (delta < 1) {
                throw Terminal::InvalidInput("Invalid IPv4 in email address.");
            }
            sectionPos += delta;
            ++segments;
        }
        if (em[sectionPos] != ']') {
            throw Terminal::InvalidInput("Missing closing bracket in IPv4 in email address.");
        }
        ++sectionPos;
        if (sectionPos != em.size()) {
            throw Terminal::InvalidInput(
                "Extra characters after closing bracket in IPv4 in email address."
            );
        }
    } else {
        // LDH
        auto domain = em.substr(sectionPos);
        if (domain.size() > 253) {
            throw Terminal::InvalidInput("Email domain must be at most 253 characters long.");
        }

        try {
            auto tmp = std::stoi(std::string(domain));
            throw Terminal::InvalidInput("Email domain must not be a number.");
        } catch (...) {}

        for (auto label : domain | std::views::split('.')) {
            if (label.size() > 63) {
                throw Terminal::InvalidInput("Email label must be at most 63 characters long.");
            }
            if (label.size() == 0) {
                throw Terminal::InvalidInput("Email label must not be empty.");
            }
            if (label[0] == '-' || label[label.size() - 1] == '-') {
                throw Terminal::InvalidInput(
                    "Email label must not start or end with a hyphen."
                );
            }
            for (auto c : label) {
                switch (c) {
                    case 'a' ... 'z':
                    case 'A' ... 'Z':
                    case '0' ... '9':
                    case '-':
                        break;
                    default:
                        throw Terminal::InvalidInput("Invalid character in email label.");
                }
            }
        }
    }
}

Email::operator std::string() {
    return email;
}
