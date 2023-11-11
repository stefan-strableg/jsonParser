#include "../inc/JsonValue.hpp"
#include "../inc/string.hpp"

namespace json
{
    JsonValue::JsonValue(std::string str)
        : JsonEntity(JsonEntityType::value)
    {
        fromString(str);
    }

    void JsonValue::fromString(std::string raw)
    {
        _data = raw;
    }

    std::string JsonValue::toString() const
    {
        return _data;
    }

    size_t JsonValue::size() const
    {
        return 1;
    }

    bool JsonValue::_isBottomLayer() const
    {
        return true;
    }

    std::string JsonValue::toStringF(const JsonFormattingOptions &, size_t) const
    {
        return _data;
    }
    
    void JsonValue::setString(const std::string& str)
    {
        set<std::string>("\"" + str + "\"");
    }
}