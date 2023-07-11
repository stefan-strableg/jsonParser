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

    size_t JsonValue::size() const
    {
        return 1;
    }

    bool JsonValue::_isBottomLayer() const
    {
        return true;
    }

    std::string JsonValue::getStringF(size_t tabs, const JsonFormattingOptions &options) const
    {
        return _data;
    }
}