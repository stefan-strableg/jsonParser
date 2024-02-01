#pragma once
#include "compat.hpp"

namespace json
{
    class JsonEntity;

    /// @brief This is the Interface implemented by all classes that can be converted to a JSON- Object or Array (or Value)
    struct IConvertableToJson
    {
    public:
        /// @brief Creates a new Json-Entity and returns a pointer to it.
        virtual JsonEntity *toJson() const = 0;
    };

}