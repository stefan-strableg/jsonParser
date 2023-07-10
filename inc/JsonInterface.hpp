#pragma once

#include <string>

namespace json
{
    class JsonInterface
    {
    protected:
        enum JsonInterfaceType : uint8_t
        {
            value,
            array,
            object
        } type;

    public:
        static JsonInterface *makeNew(std::string str);

        JsonInterface(JsonInterfaceType t);

        virtual void setString(std::string str) = 0;

        virtual std::string getString() const = 0;

        JsonInterface::JsonInterfaceType _getType();
    };
}