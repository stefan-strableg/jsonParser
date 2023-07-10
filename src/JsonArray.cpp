#include "../inc/JsonArray.hpp"
#include "../inc/JsonObject.hpp"
#include "../inc/JsonValue.hpp"
#include <iostream>
#include <sstream>

namespace json
{
    JsonArray::JsonArray(std::string str)
        : JsonInterface(JsonInterfaceType::array)
    {
        setString(str);
    }

    void JsonArray::setString(std::string jsonString)
    {
        size_t i = 1,
               tokStart = 1;
        uint16_t bracesLevel = 0,
                 bracketsLevel = 0;
        bool inQuote = false,
             backslash = false;

        for (; i < jsonString.size(); i++)
        {
            bool resetBackslash = backslash;

            switch (jsonString[i])
            {
            case '\\':
                backslash = !backslash;
                break;
            case '\"':
                if (!backslash)
                    inQuote = !inQuote;
                break;
            case '{':
                if (!inQuote)
                    bracesLevel++;
                break;
            case '}':
                if (!inQuote)
                    bracesLevel--;
                break;
            case '[':
                if (!inQuote)
                    bracketsLevel++;
                break;
            case ']':
                if (!inQuote)
                    bracketsLevel--;
                break;
            case ',':
                if (!inQuote && bracesLevel == 0 && bracketsLevel == 0)
                {
                    _data.push_back(JsonInterface::makeNew(jsonString.substr(tokStart, i - tokStart)));
                    tokStart = i + 1;
                }
            }

            if (resetBackslash)
                backslash = false;
        }
        _data.push_back(JsonInterface::makeNew(jsonString.substr(tokStart, i - tokStart - 1)));
    }

    std::string JsonArray::getString() const
    {
        std::ostringstream outStr;

        outStr << '[';

        size_t i = 0;
        for (auto &e : _data)
        {
            i++;
            outStr << e->getString();
            if (i < _data.size())
                outStr << ',';
        }

        outStr << ']';

        return outStr.str();
    }

    JsonArray &JsonArray::A(size_t n)
    {
        if (_data.size() <= n)
            throw std::out_of_range("JsonArray::A: index " + std::to_string(n) + " is out of bounds");
        if (_data[n]->_getType() != JsonInterfaceType::array)
            throw std::runtime_error("JsonArray::A: Element " + std::to_string(n) + " is not of type array");
        return dynamic_cast<JsonArray &>(*_data[n]);
    }

    JsonObject &JsonArray::O(size_t n)
    {
        if (_data.size() <= n)
            throw std::out_of_range("JsonArray::O: index " + std::to_string(n) + " is out of bounds");
        if (_data[n]->_getType() != JsonInterfaceType::object)
            throw std::runtime_error("JsonArray::O: Element " + std::to_string(n) + " is not of type object");
        return dynamic_cast<JsonObject &>(*_data[n]);
    }

    JsonValue &JsonArray::V(size_t n)
    {
        if (_data.size() <= n)
            throw std::out_of_range("JsonArray::V: index " + std::to_string(n) + " is out of bounds");
        if (_data[n]->_getType() != JsonInterfaceType::value)
            throw std::runtime_error("JsonArray::V: Element " + std::to_string(n) + " is not of type value");
        return dynamic_cast<JsonValue &>(*_data[n]);
    }

    std::string JsonArray::S(size_t n)
    {
        if (_data.size() <= n)
            throw std::out_of_range("JsonArray::S: index " + std::to_string(n) + " is out of bounds");
        if (_data[n]->_getType() != JsonInterfaceType::value)
            throw std::runtime_error("JsonArray::S: Element " + std::to_string(n) + " is not of type string");
        return _data[n]->getString();
    }
}