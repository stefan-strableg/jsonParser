#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>

namespace strn
{
    /// @brief Trims a string from the left (in place)
    static inline void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    }

    /// @brief Trims a string from the right (in place)
    static inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    /// @brief Trims a string from both ends (in place)
    static inline void trim(std::string &s)
    {
        rtrim(s);
        ltrim(s);
    }

    /// @brief Trims a string from the left (copy)
    [[nodiscard]] static inline std::string ltrim_c(std::string s)
    {
        ltrim(s);
        return s;
    }

    /// @brief Trims a string from the right (copy)
    [[nodiscard]] static inline std::string rtrim_c(std::string s)
    {
        rtrim(s);
        return s;
    }

    /// @brief Trims a string from both ends (copy)
    [[nodiscard]] static inline std::string trim_c(std::string s)
    {
        trim(s);
        return s;
    }

    /// @brief Finds the nth occurrence of c in str. n is null-indexed so that n = 0 returns the first occurence
    /// @return Index of the nth occurence of n or str.size() if not found
    [[nodiscard]] size_t find(const std::string &str, char c, size_t n);

    /// @brief Returns the nth token (substring) of a string that is seperated into tokens by a seperator character. Note that n is null-indexed so that n = 0 returns the first token
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