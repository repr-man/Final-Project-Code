#include "terminal.hpp"

void Terminal::printOptions(
    std::string_view prompt,
    std::initializer_list<std::string_view> options
) const {
    std::cout << '\n' << prompt << '\n';
    int i = 1;
    for(auto option : options) {
        std::cout << i << ". " << option << '\n';
        ++i;
    }
    std::cout << std::endl;
}

void Terminal::printError(std::string_view message) const {
    if constexpr(isWindows) {
        std::cout << "### ERROR ###\n" << message << '\n';
    } else {
        std::cout << colorError << "ERROR: " << message << '\n' << reset;
    }
}

void Terminal::trimAndRecolor(std::string& str, int width) const {
    if constexpr(isWindows) {
        auto last3 = str[width - 3];
        auto last2 = str[width - 2];
        auto last1 = str[width - 1];
        str.resize(width - 3);
        str.append("...");
    } else {
        auto last5 = str[width - 5];
        auto last4 = str[width - 4];
        auto last3 = str[width - 3];
        auto last2 = str[width - 2];
        auto last1 = str[width - 1];
        str.resize(width - 5);
        str.append(colorFade1);
        str.push_back(last5);
        str.append(colorFade2);
        str.push_back(last4);
        str.append(colorFade3);
        str.push_back(last3);
        str.append(colorFade4);
        str.push_back(last2);
        str.append(colorFade5);
        str.push_back(last1);
        str.append(reset);
    }
}

//template std::vector<int> Terminal::promptForInput<std::vector<int>, validateNumRange<1, 8>>() const;
//template std::string Terminal::promptForInput<std::string, validateIdentity>() const;
//template std::string Terminal::promptForInput<std::string, validateLibraryID>() const;
//template std::string Terminal::promptForInput<std::string, validateRole>() const;
//template std::string Terminal::promptForInput<std::string, validateInstitutionID>() const;
//template std::string Terminal::promptForInput<std::string, validatePhone>() const;
//template std::string Terminal::promptForInput<std::string, validateEmail>() const;
