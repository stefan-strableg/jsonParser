#include "../inc/JsonValue.hpp"
#include "../inc/string.hpp"

namespace json
{
    JsonValue::JsonValue(std::string str)
        : JsonInterface(JsonInterfaceType::value)
    {
        setString(str);
    }

    void JsonValue::setString(std::string str)
    {
        _data = str;
    }

    std::string JsonValue::getString() const
    {
        return _data;
    }
}