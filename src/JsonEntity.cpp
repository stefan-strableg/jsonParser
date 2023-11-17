#include "../inc/JsonEntity.hpp"
#include "../inc/Object.hpp"
#include "../inc/Array.hpp"
#include "../inc/Value.hpp"
#include "../inc/string.hpp"

#include <iostream>

namespace json
{
    FormattingOptions defaultJsonFormattingOptions;

    Object formattingExampleObject = Object::createFromString("{\"bottom level array\":[\"type\", \"array\"],\"bottom level object\":{\"type\":\"object\"},\"empty array\":[],\"empty object\":{},\"float Key\":6.9,\"int Key\":42,\"nested arrays\":[\"These:\",[[[\"are\"]]],\"nested\",\"arrays\"],\"nested objects\":{\"some\":{\"nested\":{\"objects\":{}}},\"are\":\"here\"},\"nested objects and arrays\":{\"these\":[\"are\",{\"arr\":[\"rays\"],\"nested\":\"objects\"}]},\"str Key\":\"String\"}");

    std::string FormattingOptions::getFormattingExample() const
    {
        return formattingExampleObject.toStringF(*this);
    }

    JsonEntity *JsonEntity::makeNewFromString(std::string raw)
    {
        strn::trim(raw);
        if (raw.length() < 2)
            return new Value(raw);

        if (raw[0] == '{' && raw[raw.size() - 1] == '}')
            return new Object(Object::createFromString(raw));

        if (raw[0] == '[' && raw[raw.size() - 1] == ']')
        {
            return new Array(Array::createFromString(raw));
        }

        return new Value(raw);
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
    JsonEntity *JsonEntity::makeNew<JsonEntity>(const JsonEntity &raw)
    {
        if (const Object *obj = dynamic_cast<const Object *>(&raw))
            return makeNew(*obj);

        if (const Array *arr = dynamic_cast<const Array *>(&raw))
            return makeNew(*arr);

        if (const Value *val = dynamic_cast<const Value *>(&raw))
            return makeNew(*val);

        throw std::logic_error("JsonEntity was neither Object, nor Array, nor Value");
    }

    template <>
    JsonEntity *JsonEntity::makeNew<Object>(const Object &raw)
    {
        return new Object(raw);
    }

    template <>
    JsonEntity *JsonEntity::makeNew<Array>(const Array &raw)
    {
        return new Array(raw);
    }

    template <>
    JsonEntity *JsonEntity::makeNew<Value>(const Value &raw)
    {
        return new Value(raw);
    }

    template <>
    JsonEntity *JsonEntity::makeNew<std::string>(const std::string &raw)
    {
        return new Value("\"" + raw + "\"");
    }

    template <>
    JsonEntity *JsonEntity::makeNew<const char *>(const char *const &raw)
    {
        return makeNew(std::string(raw));
    }
}

std::ostream &operator<<(std::ostream &os, const json::JsonEntity &entity)
{
    os << entity.toString();
    return os;
}