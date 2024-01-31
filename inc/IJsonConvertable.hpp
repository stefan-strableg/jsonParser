#pragma once
#include "JsonEntity.hpp"

namespace json
{
    struct IJsonConvertable
    {
    public:
        virtual JsonEntity *getNewJsonEntity() const = 0;
    };
}