#include "../inc/JsonObject.hpp"
#include "../inc/JsonArray.hpp"
#include "../inc/JsonValue.hpp"
#include "../inc/string.hpp"
#include <iostream>
#include <sstream>

namespace json
{

    JsonObject::JsonObject(std::string str)
        : JsonInterface(JsonInterfaceType::object)
    {
        setString(str);
    }

    void JsonObject::setString(std::string _string)
    {
        size_t i = 1,
               tokStart = 1;
        uint16_t bracesLevel = 0,
                 bracketsLevel = 0;
        std::pair<std::string, JsonInterface *> keyValPair = {"", nullptr};
        bool inQuote = false,
             backslash = false,
             inColonComma = false;
        for (; i < _string.size() - 1; i++)
        {
            bool resetBackslash = backslash;

            switch (_string[i])
            {
            case '\\':
                backslash = !backslash;
                break;
            case '"':
                if (!inColonComma && !backslash && bracesLevel == 0 && bracketsLevel == 0)
                {
                    if (!inQuote)
                        tokStart = i;
                    else
                    {
                        keyValPair.first = _string.substr(tokStart + 1, i - tokStart - 1);
                    }

                    inQuote = !inQuote;
                }
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
            case ':':
                if (!inQuote && bracesLevel == 0 && bracketsLevel == 0)
                {
                    inColonComma = true;
                    tokStart = i + 1;
                }
                break;
            case ',':
                if (inColonComma && !inQuote && bracesLevel == 0 && bracketsLevel == 0)
                {
                    keyValPair.second = JsonInterface::makeNew(_string.substr(tokStart, i - tokStart));
                    _data.insert(keyValPair);
                    inColonComma = false;
                }
            }

            if (resetBackslash)
                backslash = false;
        }

        if (inColonComma)
        {
            keyValPair.second = JsonInterface::makeNew(_string.substr(tokStart, i - tokStart));
            _data.insert(keyValPair);
        }
    }

    std::string JsonObject::getString() const
    {
        std::ostringstream outStr;

        outStr << '{';

        size_t i = 0;
        for (auto &e : _data)
        {
            i++;
            outStr << '\"' << e.first << "\":" << e.second->getString();
            if (i < _data.size())
                outStr << ',';
        }

        outStr << '}';

        return outStr.str();
    }

    JsonArray &JsonObject::A(std::string key)
    {
        if (!_data.contains(key))
            throw std::out_of_range("JsonObject::A: there is no element with key " + key);
        if (_data[key]->_getType() != JsonInterfaceType::array)
            throw std::runtime_error("JsonObject::A: Value of element with key " + key + " is not of type array");
        return dynamic_cast<JsonArray &>(*_data[key]);
    }

    JsonObject &JsonObject::O(std::string key)
    {
        if (!_data.contains(key))
            throw std::out_of_range("JsonObject::O: there is no element with key " + key);
        if (_data[key]->_getType() != JsonInterfaceType::object)
            throw std::runtime_error("JsonObject::O: Value of element with key " + key + " is not of type object");
        return dynamic_cast<JsonObject &>(*_data[key]);
    }

    JsonValue &JsonObject::V(std::string key)
    {
        if (!_data.contains(key))
            throw std::out_of_range("JsonObject::V: there is no element with key " + key);
        if (_data[key]->_getType() != JsonInterfaceType::object)
            throw std::runtime_error("JsonObject::V: Value of element with key " + key + " is not of type value");
        return dynamic_cast<JsonValue &>(*_data[key]);
    }

    std::string JsonObject::S(std::string key)
    {
        if (!_data.contains(key))
            throw std::out_of_range("JsonObject::S: there is no element with key " + key);
        if (_data[key]->_getType() != JsonInterfaceType::value)
            throw std::runtime_error("JsonObject::S: Value of element with key " + key + " is not of type string");
        return _data[key]->getString();
    }

    bool JsonObject::isNull(std::string key)
    {
        if (!_data.contains(key))
            throw std::runtime_error("JsonObject::isNull: there is no element with key " + key);
        return _data[key]->getString() == "null";
    }

    bool JsonObject::contains(std::string key)
    {
        return _data.contains(key);
    }

    bool JsonObject::isEmpty()
    {
        return _data.empty();
    }

}