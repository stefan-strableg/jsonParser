#pragma once
#include "compat.hpp"

namespace json
{
    class JsonEntity;

    template <typename T, typename = void>
    struct isConvertibleToJson : std::false_type
    {
    };

    template <typename T>
    struct isConvertibleToJson<T, std::void_t<decltype((JsonEntity*)std::declval<T>().toJson())>> : std::true_type
    {
    };

    struct Array;

    template <typename T, typename = void>
    struct isConvertibleFromJsonArray : std::false_type
    {
    };

    template <typename T>
    struct isConvertibleFromJsonArray<T, std::void_t<decltype(std::declval<T>() = T::fromJsonArray(std::declval<const Array &>()))>> : std::true_type
    {
    };

    struct Object;

    template <typename T, typename = void>
    struct isConvertibleFromJsonObject : std::false_type
    {
    };

    template <typename T>
    struct isConvertibleFromJsonObject<T, std::void_t<decltype(std::declval<T>() = T::fromJsonObject(std::declval<const Object &>()))>> : std::true_type
    {
    };
}