#include "../inc/JsonEntity.hpp"
#include "../inc/Object.hpp"
#include "../inc/Array.hpp"
#include "../inc/Value.hpp"
#include "../inc/string.hpp"

namespace json
{
    FormattingOptions defaultJsonFormattingOptions;

    Object formattingExampleObject("{\"bottom level array\":[\"type\", \"array\"],\"bottom level object\":{\"type\":\"object\"},\"empty array\":[],\"empty object\":{},\"float Key\":6.9,\"int Key\":42,\"nested arrays\":[\"These:\",[[[\"are\"]]],\"nested\",\"arrays\"],\"nested objects\":{\"some\":{\"nested\":{\"objects\":{}}},\"are\":\"here\"},\"nested objects and arrays\":{\"these\":[\"are\",{\"arr\":[\"rays\"],\"nested\":\"objects\"}]},\"str Key\":\"String\"}");

    std::string FormattingOptions::getFormattingExample() const
    {
        return formattingExampleObject.toStringF(*this);
    }

    JsonEntity::JsonEntity(JsonEntityType t)
        : type(t)
    {
    }

    JsonEntity *JsonEntity::makeNew(std::string raw)
    {
        strn::trim(raw);
        if (raw.size() > 2 && *raw.begin() == '{' && *(raw.end() - 1) == '}')
        {
            return new Object(raw);
        }
        else if (raw.size() > 2 && *raw.begin() == '[' && *(raw.end() - 1) == ']')
        {
            auto ret = new Array();
            ret->fromString(raw);
            return ret;
        }
        else
        {
            return new Value(raw);
        }
    }

    JsonEntity::JsonEntityType JsonEntity::_getType() const
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
}

std::ostream &operator<<(std::ostream &os, const json::JsonEntity &entity)
{
    os << entity.toString();
    return os;
}