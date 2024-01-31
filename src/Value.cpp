#include "../inc/Value.hpp"
#include "../inc/string.hpp"

namespace json
{
    JsonEntity *Value::toJson() const
    {
        return new Value(*this);
    }

    Value::Value()
        : JsonEntity(JsonEntity::value)
    {
    }

    Value::Value(std::string str)
        : JsonEntity(JsonEntityType::value)
    {
        loadString(str);
    }

    void Value::loadString(std::string raw)
    {
        _data = raw;
    }

    std::string Value::toString() const
    {
        return _data;
    }

    size_t Value::size() const
    {
        return 1;
    }

    bool Value::isBottomLayer() const
    {
        return true;
    }

    std::string Value::toStringF(const FormattingOptions &, size_t) const
    {
        return _data;
    }

    void Value::setString(const std::string &str)
    {
        set<std::string>("\"" + str + "\"");
    }
}