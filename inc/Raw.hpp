#pragma once
#include <string>
#include "compat.hpp"

namespace json
{
    struct Raw : public std::string
    {
        Raw(const std::string &str) : std::string(str){};
    };
}