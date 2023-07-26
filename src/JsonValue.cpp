#include "../inc/JsonValue.hpp"
#include "../inc/string.hpp"

namespace json
{
    JsonValue::JsonValue(std::string str)
        : JsonEntity(JsonEntityType::value)
    {
        setString(str);
    }

    void JsonValue::setString(std::string raw)
    {
        _data = raw;
    }

    std::string JsonValue::getString() const
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

    std::string JsonValue::getStringF(const JsonFormattingOptions &options, size_t tabs) const
    {
        return _data;
    }
}