#pragma once
#include <string>
#include <sstream>

namespace MM::str
{
    /// @brief Finds the nth occurrence of c in str. n is null-indexed so that n = 0 returns the first occurence
    /// @return Index of the nth occurence of n or str.size() if not found
    [[nodiscard]] size_t find(const std::string &str, char c, size_t n);

    /// @brief Returns the nth token (substring) of a string that is seperated into tokens by a seperator character. Note that is null-indexed so that N = 0 returns the first element
    /// @example ";Hello;beautiful;World;" would result in the tokens "", "Hello", "beautiful", "World", and ""
    /// @return nth token
    [[nodiscard]] std::string token(const std::string &str, char seperator, size_t n);

    /// @brief Converts a string to any type using the istream >> operators
    /// @tparam T Target type
    /// @return Converted string or default constructed type if string is empty
    template <typename T>
    [[nodiscard]] inline T string_to(const std::string &str)
    {
        if (str.empty()) // to avoid weird behavior when reading empty strings
            return T();
        T ret;
        std::stringstream iss(str);
        iss >> ret;
        return ret;
    }
}