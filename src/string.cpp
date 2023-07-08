#include "../inc/string.hpp"
#include <iostream>
namespace strn
{
    size_t find(const std::string &str, char c, size_t n)
    {
        n++;
        size_t i = 0;
        while (i < str.size())
        {
            if (str[i] == c)
                n--;
            if (n == 0)
                return i;
            i++;
        }
        return str.size();
    }

    std::string token(const std::string &str, char seperator, size_t n)
    {
        int begin_char = find(str, seperator, n - 1),
            end_char = find(str, seperator, n);

        begin_char++; // dont include the seperator

        if (n == 0)
            begin_char = 0;

        return str.substr(begin_char, end_char - begin_char);
    }
}