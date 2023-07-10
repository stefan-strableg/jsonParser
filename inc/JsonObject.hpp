#pragma once
#include "JsonInterface.hpp"
#include "string.hpp"
#include <sstream>
#include <iostream>
#include <map>

namespace json
{
    class JsonArray;
    class JsonValue;

    class JsonObject : public JsonInterface
    {
    private:
        std::map<std::string, JsonInterface *> _data;

    public:
        JsonObject()
            : JsonInterface(JsonInterfaceType::object)
        {
        }
        JsonObject(std::string str);

        template <typename T>
        inline void add(std::string key, T val)
        {
            std::ostringstream inStr;
            inStr << val;
            _data.insert({key, JsonInterface::makeNew(inStr.str())});
        }

        template <typename T1, typename... Ts>
        inline JsonObject(std::string key, T1 value, Ts... ts)
            : JsonObject(ts...)
        {
            add(key, value);
        }

        void setString(std::string str) override;

        std::string getString() const override;

        JsonArray &A(std::string key);
        JsonObject &O(std::string key);
        JsonValue &V(std::string key);
        std::string S(std::string key);

        template <typename T>
        T get(std::string key)
        {
            if (!_data.contains(key))
                return T();
            return strn::string_to<T>(_data[key]->getString());
        }

        bool isNull(std::string key);
        bool contains(std::string key);
        bool isEmpty();
    };

}