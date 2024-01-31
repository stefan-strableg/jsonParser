#pragma once
#include "compat.hpp"

namespace json
{
    class JsonEntity;

    struct ICloneable
    {
    public:
        virtual JsonEntity *getJsonClone() const = 0;
    };

}