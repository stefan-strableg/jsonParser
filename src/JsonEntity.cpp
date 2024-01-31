#include "../inc/JsonEntity.hpp"
#include "../inc/IConvertableToJson.hpp"
#include "../inc/Object.hpp"
#include "../inc/Array.hpp"
#include "../inc/Value.hpp"
#include "../inc/string.hpp"

#include <iostream>

namespace json
{
    FormattingOptions defaultJsonFormattingOptions;

    Object formattingExampleObject = Object::fromString("{\"bottom level array\":[\"type\", \"array\"],\"bottom level object\":{\"type\":\"object\"},\"empty array\":[],\"empty object\":{},\"float Key\":6.9,\"int Key\":42,\"nested arrays\":[\"These:\",[[[\"are\"]]],\"nested\",\"arrays\"],\"nested objects\":{\"some\":{\"nested\":{\"objects\":{}}},\"are\":\"here\"},\"nested objects and arrays\":{\"these\":[\"are\",{\"arr\":[\"rays\"],\"nested\":\"objects\"}]},\"str Key\":\"String\"}");

    std::string FormattingOptions::getFormattingExample() const
    {
        return formattingExampleObject.toStringF(*this);
    }

    JsonEntity *JsonEntity::_makeNewValue(const std::string &str)
    {
        return new Value(str);
    }

    JsonEntity *JsonEntity::_makeNewFromString(std::string str)
    {
        strn::trim(str);
        if (str.length() < 2)
            return new Value(str);

        if (str[0] == '{' && str[str.size() - 1] == '}')
            return new Object(Object::fromString(str));

        if (str[0] == '[' && str[str.size() - 1] == ']')
        {
            return new Array(Array::fromString(str));
        }

        return new Value(str);
    }

    JsonEntity::JsonEntity(JsonEntityType t)
        : type(t)
    {
    }

    JsonEntity::JsonEntityType JsonEntity::getType() const
    {
        return type;
    }

    size_t JsonEntity::size() const
    {
        return 1;
    }

    JsonEntity::~JsonEntity()
    {
    }

    bool isValidJson(std::string)
    {
        return false;
    }

    template <>
    JsonEntity *JsonEntity::_makeNew<std::string>(const std::string &str)
    {
        return new Value("\"" + str + "\"");
    }

    template <>
    JsonEntity *JsonEntity::_makeNew<const char *>(const char *const &str)
    {
        return _makeNew(std::string(str));
    }
}

std::ostream &operator<<(std::ostream &os, const json::JsonEntity &entity)
{
    os << entity.toString();
    return os;
}