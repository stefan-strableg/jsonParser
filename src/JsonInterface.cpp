#include "../inc/JsonInterface.hpp"
#include "../inc/JsonObject.hpp"
#include "../inc/JsonArray.hpp"
#include "../inc/JsonValue.hpp"
#include "../inc/string.hpp"

namespace json
{
    JsonFormattingOptions defaultJsonFormattingOptions;

    JsonObject formattingExampleObject("{\"bottom level array\":[\"type\", \"array\"],\"bottom level object\":{\"type\":\"object\"},\"empty array\":[],\"empty object\":{},\"float Key\":6.9,\"int Key\":42,\"nested arrays\":[\"These:\",[[[\"are\"]]],\"nested\",\"arrays\"],\"nested objects\":{\"some\":{\"nested\":{\"objects\":{}}},\"are\":\"here\"},\"nested objects and arrays\":{\"these\":[\"are\",{\"arr\":[\"rays\"],\"nested\":\"objects\"}]},\"str Key\":\"String\"}");

    std::string JsonFormattingOptions::getFormattingExample() const
    {
        return formattingExampleObject.getStringF(*this);
    }

    JsonEntity::JsonEntity(JsonInterfaceType t)
        : type(t)
    {
    }

    JsonEntity *JsonEntity::makeNew(std::string str)
    {
        strn::trim(str);
        if (str.size() > 2 && *str.begin() == '{' && *(str.end() - 1) == '}')
        {
            return new JsonObject(str);
        }
        else if (str.size() > 2 && *str.begin() == '[' && *(str.end() - 1) == ']')
        {
            auto ret = new JsonArray();
            ret->setString(str);
            return ret;
        }
        else
        {
            return new JsonValue(str);
        }
    }

    JsonEntity::JsonInterfaceType JsonEntity::_getType() const
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

    bool isValidJson(std::string str)
    {
        return false;
    }
}

std::ostream &operator<<(std::ostream &os, const json::JsonEntity &j)
{
    os << j.getString();
    return os;
}