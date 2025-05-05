#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <numeric>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <iomanip>

#include "librarystoragetype.hpp"
#include "main.hpp"
#include "resultlist.hpp"
#include "validators.hpp"
#include "zip_view.hpp"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
constexpr bool isWindows = true;
#else
#include <sys/ioctl.h>
#include <unistd.h>
constexpr bool isWindows = false;
#endif

/// Encapsulates IO-related components of the program.
class Terminal {
    constexpr static uint32_t minTableCellWidth = 5;
    
    uint32_t terminalWidth = 0;

    #ifdef _WIN32
    unsigned long consoleMode;
    constexpr static auto promptArrow = "==> ";
    constexpr static std::string_view reset;
    constexpr static std::string_view colorFade1;
    constexpr static std::string_view colorFade2;
    constexpr static std::string_view colorFade3;
    constexpr static std::string_view colorFade4;
    constexpr static std::string_view colorFade5;
    constexpr static std::string_view colorError;
    #else
    constexpr static auto promptArrow = "══> ";
    constexpr static std::string_view reset = "\x1b[0m";
    constexpr static std::string_view colorFade1 = "\x1b[38;2;223;223;223m";
    constexpr static std::string_view colorFade2 = "\x1b[38;2;191;191;191m";
    constexpr static std::string_view colorFade3 = "\x1b[38;2;159;159;159m";
    constexpr static std::string_view colorFade4 = "\x1b[38;2;127;127;127m";
    constexpr static std::string_view colorFade5 = "\x1b[38;2;95;95;95m";
    constexpr static std::string_view colorError = "\x1b[0;31m";
    #endif

    static std::vector<uint32_t> columnWidths;

    /// Draws a border row of a table.
    template <int N>
    void drawTableRow(
        const std::string_view fillChar,
        const std::string_view startChar,
        const std::string_view middleChar,
        const std::string_view endChar
    ) const {
        using namespace std::views;

        std::cout << startChar;
        for(auto width : columnWidths | take(columnWidths.size() - 1)) {
            for(auto _ : iota(0u, width)) {
                std::cout << fillChar;
            }
            std::cout << middleChar;
        }
        for(auto _ : iota(0u, columnWidths.back())) {
            std::cout << fillChar;
        }
        std::cout << endChar << std::endl;
    }

    /// Draws a row of a table with content.
    template <int N>
    void drawTableRow(
        const std::string_view startChar,
        const std::string_view middleChar,
        const std::string_view endChar,
        const std::string indexColContent,
        const std::vector<std::string>& items,
        std::ios_base &(alignment)(std::ios_base&) = std::right
    ) const {
        using namespace std::views;

        auto oldAlignment = std::cout.flags();
        std::cout << startChar;
        std::cout << std::setw(columnWidths[0]) << indexColContent;
        std::cout << middleChar << alignment;
        for(auto [width, item] : c9::zip(columnWidths | drop(1)
                                                       | take(items.size() - 1),
                                          items)
        ) {
            std::cout << std::setw(width) << item;
            std::cout << middleChar;
        }
        std::cout << std::setw(columnWidths.back()) << items.back();
        std::cout << endChar << std::endl;
        std::cout.flags(oldAlignment);
    }

    /// Implements the fade effect on overly large cell content.
    void trimAndRecolor(std::string& str, int width) const;

public:
    Terminal() {
        #ifdef _WIN32
        // Gets the width of the terminal.
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        terminalWidth = info.dwSize.X;

        // Puts the terminal into VT100 mode.
        consoleMode = GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &consoleMode);
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), consoleMode | 0x0001 | 0x0004);
        #else
        // Gets the width of the terminal.
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        terminalWidth = w.ws_col;
        #endif

        // Assuming that we have one less character to work with lets us avoid
        // some weird problems with tables' borders.
        terminalWidth -= 1;
    }

    ~Terminal() {
        #ifdef _WIN32
        // Puts the terminal back to its original mode.
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), consoleMode);
        #endif
    }

    /// Prompts the user for input and returns the result.  Handles EOF and
    /// and Ctrl-C properly.
    ///
    /// The return value is typed.  So, it you want get an int from the user,
    /// you would write:
    /// ```
    /// int choice = terminal.promptForInput<int>();
    /// ```
    /// It will automatically loop until the user enters a valid value of the
    /// appropriate type.
    template <typename T, auto validator = validateIdentity>
    requires (std::integral<T> || std::is_same_v<T, std::string>)
    T promptForInput() const {
        T input;
        while(true) {
            std::string buf;
            std::cin >> std::ws;
            std::getline(std::cin, buf);

            if(std::cin.eof()) {
                Main::safeExit();
            }

            auto errMsg = validator(buf);
            if(errMsg.size() > 0) {
                printError(errMsg);
                std::cout << "Please try again: ";
                continue;
            }

            auto strStream = std::stringstream(buf);
            if constexpr (std::is_same_v<T, std::string>) {
                // Don't split on whitespace.
                input = std::string(buf);
            } else {
                strStream >> input;
            }

            if(std::cin.eof()) {
                std::cout << std::endl;
                Main::safeExit();
            } else if(std::cin.fail() || strStream.fail()) {
                printError("Invalid input.");
                std::cout << "Please try again: ";
                strStream.clear();
                strStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else
            return input;
        }
    }

    template <std::ranges::random_access_range V, auto validator = validateIdentity>
    requires (std::is_same_v<V, std::vector<typename V::value_type>>)
    std::vector<typename V::value_type>
    promptForInput() const {
        V input;
    retry: while(true) {
            input.clear();
            std::string buf;
            std::cin >> std::ws;
            std::getline(std::cin, buf);

            if(std::cin.eof()) {
                Main::safeExit();
            }

            auto strStream = std::stringstream(buf);
            std::string item;
            while (strStream >> item) {
                auto errMsg = validator(item);
                if(errMsg.size() > 0) {
                    printError(errMsg);
                    std::cout << "Please try again: ";
                    goto retry;
                }

                auto dataStream = std::stringstream(item);
                typename V::value_type data;
                dataStream >> data;
                if(std::cin.eof()) {
                    std::cout << std::endl;
                    Main::safeExit();
                } else if(std::cin.fail() || dataStream.fail()) {
                    printError("Invalid input.");
                    std::cout << "Please try again: ";
                    dataStream.clear();
                    dataStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    input.push_back(data);
                }
            }
            return input;
        }
    }

    /// Same as `promptForInput` but prints a prompt text first.  Helps maintain
    /// stylistic consistency throughout the application.
    template <typename T, auto validator = validateIdentity>
    T promptForInput(std::string_view prompt) const {
        std::cout << prompt << ": ";
        return promptForInput<T, validator>();
    }

    void printOptions(
        std::string_view prompt,
        std::initializer_list<std::string_view> options
    ) const;

    void printError(std::string message) const;

    /// Prints a formatted table of strings.
    ///
    /// Note: We will probably remove this function once we fully flesh out
    /// how we want to model our data and provide it via the `LibraryStorageType`
    /// interface.
    template <IsLibraryStorageType T, typename... ColumnNames>
    requires (std::same_as<ColumnNames, typename T::FieldTag> && ...)
    void printTable(
        std::vector<Row>&& rows,
        ColumnNames... columnNames
    ) const {
        using namespace std::views;

        // Collect the tags and their associated names.
        constexpr int N = sizeof...(ColumnNames);
        columnWidths.resize(N + 1);
        auto colTags = std::array{ columnNames... };
        auto colNames = std::vector<std::string>{};
        colNames.resize(colTags.size());
        for (auto [tag, name] : c9::zip(colTags, colNames)) {
            name = T::to_string(tag);
        }

        // Start off with each column's width being the width of the column name.
        uint32_t indexColumnWidth = std::to_string(rows.size()).size();
        uint32_t fullWidth = N + 1 + indexColumnWidth;
        columnWidths.resize(N + 1);
        columnWidths[0] = indexColumnWidth;
        for (auto [width, name] : c9::zip(columnWidths | drop(1), colNames)) {
            width = name.size();
        }

        // Calculate the preferred width of each column.
        for (auto& row : rows) {
            auto rowItems = row.get<T>(columnNames...);
            for (auto [width, item] : c9::zip(columnWidths | drop(1), rowItems)) {
                width = std::max((unsigned long)width, (unsigned long)(item.size()));
            }
        }
        fullWidth += std::accumulate(columnWidths.begin(), columnWidths.end(), 0);

        if (fullWidth > terminalWidth) {
            // We need to shrink the width of some columns proportionally.
            uint32_t adjustedWidth = N + 1 + indexColumnWidth;
            for (auto [width, name] : c9::zip(columnWidths | drop(1), colNames)) {
                width = std::max(minTableCellWidth, width * terminalWidth / fullWidth);
                adjustedWidth += width;

                if (name.size() > width) {
                    trimAndRecolor(name, width);
                }
            }

            for (auto& row : rows) {
                for (const auto& [width, col] : c9::zip(columnWidths, row.get<T>(columnNames...))) {
                    if (col.size() > width) {
                        trimAndRecolor(col, width);
                    }
                }
            }

            if (adjustedWidth > terminalWidth + 1) {
                printError("Your terminal window is too small to display the table.");
                return;
            }
        }

        // Draw the table.
        if constexpr(isWindows) {
            drawTableRow<N>("-", "+", "+", "+");
            drawTableRow<N>("|", "|", "|", "#", colNames, std::internal);
            drawTableRow<N>("-", "+", "+", "+");
            for(int i = 0; i < rows.size() - 1; ++i) {
                drawTableRow<N>("|", "|", "|", std::to_string(i + 1), rows[i].get<T>(columnNames...));
                drawTableRow<N>("-", "+", "+", "+");
            }
            drawTableRow<N>("|", "|", "|", std::to_string(rows.size()),
                            rows.back().get<T>(columnNames...));
            drawTableRow<N>("-", "+", "+", "+");
        } else {
            drawTableRow<N>("━", "┏", "┳", "┓");
            drawTableRow<N>("┃", "┃", "┃", "#", colNames, std::internal);
            drawTableRow<N>("━", "┡", "╇", "┩");
            for(int i = 0; i < rows.size() - 1; ++i) {
                drawTableRow<N>("│", "│", "│", std::to_string(i + 1), rows[i].get<T>(columnNames...));
                drawTableRow<N>("─", "├", "┼", "┤");
            }
            drawTableRow<N>("│", "│", "│", std::to_string(rows.size()),
                            rows.back().get<T>(columnNames...));
            drawTableRow<N>("─", "└", "┴", "┘");
        }
    }



    /// Prints a table of objects that implement the `Printable` interface from a `ResultList`.
    template <IsLibraryStorageType T, typename... ColumnNames>
    requires (std::same_as<ColumnNames, typename T::FieldTag> && ...)
    void printTable(
        const ResultList<T>& results,
        ColumnNames... columnNames
    ) const {
        if(results.size() == 0) {
            std::cout << "No results found.\n" << std::endl;
            return;
        }

        auto tmp = std::vector<Row>();
        tmp.reserve(results.size());
        for(int i = 0; i < results.size(); ++i) {
            tmp.push_back(results[i].provideRow());
        }
        printTable<T>(std::move(tmp), columnNames...);
    }
};
