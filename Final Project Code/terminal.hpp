#pragma once

#include <algorithm>
#include <array>
#include <initializer_list>
#include <ios>
#include <limits>
#include <string_view>
#include <type_traits>
#include <utility>
#include <iostream>
#include <string>
#include <vector>


#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif


template <typename S>
concept IsString = std::is_convertible_v<S, std::string>;

/// Encapsulates IO-related components of the program.
class Terminal {
    constexpr static std::string_view reset = "\x1b[0m";
    constexpr static std::string_view colorFade1 = "\x1b[38;2;223;223;223m";
    constexpr static std::string_view colorFade2 = "\x1b[38;2;191;191;191m";
    constexpr static std::string_view colorFade3 = "\x1b[38;2;159;159;159m";
    constexpr static std::string_view colorFade4 = "\x1b[38;2;127;127;127m";
    constexpr static std::string_view colorFade5 = "\x1b[38;2;95;95;95m";
    constexpr static auto minTableCellWidth = 5;
    
    int terminalWidth = 0;

    #ifdef _WIN32
    int consoleMode;
    #endif

    /// Draws a border row of a table.
    template <int N>
    void drawTableRow(
        const std::string_view fillChar,
        const std::string_view startChar,
        const std::string_view middleChar,
        const std::string_view endChar,
        const std::array<int, N>& widths
    ) const {
        std::cout << startChar;
        for(int i = 0; i < widths.size() - 1; ++i) {
            for(int j = 0; j < widths[i]; ++j) {
                std::cout << fillChar;
            }
            std::cout << middleChar;
        }
        for(int j = 0; j < widths.back(); ++j) {
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
        const std::array<int, N>& widths,
        const std::array<std::string, N>& items,
        std::ios_base &(alignment)(std::ios_base&) = std::right
    ) const {
        auto oldAlignment = std::cout.flags();
        std::cout << alignment << startChar;
        for(int i = 0; i < widths.size() - 1; ++i) {
            std::cout << setw(widths[i]) << items[i];
            std::cout << middleChar;
        }
        std::cout << setw(widths.back()) << items.back();
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

    template <typename T>
    T promptForInput(T defaultValue) const {
        T input;

        while(true) {
            std::cout << "══>> ";
            std::cin >> input;

            if(std::cin.eof())
                exit(0);

            if(std::cin.fail()) {
                std::cout << "Invalid input.  Please enter a " << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        return input;
    }

    void printOptions(
        std::string_view prompt,
        std::initializer_list<std::string_view> options
    ) const;


    /// Prints a formatted table of strings.
    ///
    /// Note: We will probably remove this function once we fully flesh out
    /// how we want to model our data and provide it via the `Printable`
    /// interface.
    template <IsString... ColumnNames>
    void printTable(
        std::vector<std::array<std::string, sizeof...(ColumnNames)>>&& rows,
        ColumnNames... columnNames
    ) const {
        constexpr int N = sizeof...(ColumnNames);
        std::array<std::string, N> colNames{columnNames...};
        auto fullWidth = N + 1;

        // Calculate the preferred width of each column.
        auto columnWidths = array<int, N>();
        for(int i = 0; i < N; ++i) {
            auto width = colNames[i].size();
            for(auto& row : rows) {
                width = max(width, row[i].size());
            }
            columnWidths[i] = width;
            fullWidth += width;
        }

        if(fullWidth > terminalWidth) {
            // We need to shrink the width of some columns proportionally.
            auto adjustedWidth = N + 1;
            for(int i = 0; i < columnWidths.size(); ++i) {
                columnWidths[i] = max(minTableCellWidth, columnWidths[i] * terminalWidth / fullWidth);
                adjustedWidth += columnWidths[i];
                if(colNames[i].size() > columnWidths[i]) {
                    trimAndRecolor(colNames[i], columnWidths[i]);
                }
                for(auto& row : rows) {
                    if(row[i].size() > columnWidths[i]) {
                        trimAndRecolor(row[i], columnWidths[i]);
                    }
                }
            }

            if(adjustedWidth > terminalWidth) {
                std::cout << "Your terminal window is too small to display the table." << std::endl;
                exit(-1);
            }
        }

        // Draw the table.
        drawTableRow<N>("━", "┏", "┳", "┓", columnWidths);
        drawTableRow<N>("┃", "┃", "┃", columnWidths, colNames, std::internal);
        drawTableRow<N>("━", "┞", "╇", "┩", columnWidths);
        for(int i = 0; i < rows.size() - 1; ++i) {
            drawTableRow<N>("│", "│", "│", columnWidths, rows[i]);
            drawTableRow<N>("─", "├", "┼", "┤", columnWidths);
        }
        drawTableRow<N>("│", "│", "│", columnWidths, rows.back());
        drawTableRow<N>("─", "└", "┴", "┘", columnWidths);
    }


    
    /// An interface for types that can provide information to a table.
    /// `N` is the number of columns of data the type provides.
    template <int N>
    class Printable {
    public:
        virtual ~Printable() {}
        virtual std::array<std::string, N> providePrintableData() = 0;
    };

    /// Prints a table of objects that implement the `Printable` interface.
    template <typename T, IsString... ColumnNames>
    requires std::is_base_of_v<Printable<sizeof...(ColumnNames)>, T>
    void printTable(
        std::vector<T>&& rows,
        ColumnNames... columnNames
    ) const {
        constexpr int N = sizeof...(ColumnNames);
        auto tmp = std::vector<std::array<std::string, N>>();
        tmp.reserve(rows.size());
        for(auto& row : rows) {
            tmp.push_back(row.providePrintableData());
        }
        printTable(
            std::move(tmp),
            columnNames...
        );
    }

    /// Prints a table of objects that implement the `Printable` interface.
    /// Takes an initializer list.  Mostly for testing.
    template <typename T, IsString... ColumnNames>
    requires std::is_base_of_v<Printable<sizeof...(ColumnNames)>, T>
    void printTable(
        std::initializer_list<T>&& rows,
        ColumnNames... columnNames
    ) const {
        printTable(std::vector<T>(rows), columnNames...);
    }
};
