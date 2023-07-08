#include "../inc/jsonParser.hpp"
#include "../inc/string.hpp"
#include <iostream>
#include <sstream>

namespace json
{

    template <typename T>
    T JsonItem::get()
    {
        if (type != nonJson)
            return T();
        return strn::string_to<T>(*valuePtr);
    }

    template <>
    JsonObject JsonItem::get()
    {
        if (type != object)
            return JsonObject();
        return *static_cast<JsonObject *>(valuePtr);
    }

    template <>
    JsonArray JsonItem::get()
    {
        if (type != array)
            return JsonArray();
        return *static_cast<JsonArray *>(valuePtr);
    }

    JsonItem::JsonItem(const std::string &str)
    {
        std::string strt = strn::trim_c(str);
        if (strt.size() > 2 && *strt.begin() == '{' && *(strt.end() - 1) == '{')
        {
            type = object;
            valuePtr = (void *)new JsonObject(strt);
        }
        else if (strt.size() > 2 && *strt.begin() == '[' && *(strt.end() - 1) == ']')
        {
            type = array;
            valuePtr = (void *)new JsonArray(strt);
        }
        else
        {
            type = nonJson;
            valuePtr = (void *)new std::string(strt);
        }
    }

    JsonItem::JsonItem(std::string &&str)
    {
        strn::trim(str);
        if (str.size() >= 2 && *str.begin() == '{' && *(str.end() - 1) == '{')
        {
            type = object;
            valuePtr = (void *)new JsonObject(str);
        }
        else if (str.size() >= 2 && *str.begin() == '[' && *(str.end() - 1) == ']')
        {
            type = array;
            valuePtr = (void *)new JsonArray(str);
        }
        else
        {
            type = nonJson;
            valuePtr = (void *)new std::string(str);
        }
    }

    std::string JsonItem::getString()
    {
        if (type == object)
            return "";

        if (type == array)
            return ""; // ToDo

        if (type == nonJson)
            return *static_cast<std::string *>(valuePtr);
    }

    JsonItem::~JsonItem()
    {
        delete valuePtr;
    }

    JsonArray::JsonArray()
        : _flags(0)
    {
    }

    JsonArray::JsonArray(std::string str)
        : _flags(0)
    {
        if (str.size() < 2 || *str.begin() != '[' || *(str.end() - 1) == ']')
            return;
        _string = str;
        _flags |= _stringValid;
    }

    bool JsonArray::_buildMap()
    {
    }
    bool JsonArray::_buildString()
    {
        if (_flags & _stringValid)
            return false;
    }

    JsonItem JsonArray::at(size_t n)
    {
        if (!_buildMap())
            return JsonItem();
        if (_data.size() <= n)
            return JsonItem();

        return _data[n];
    }

    JsonObject::JsonObject()
        : _flags(0)
    {
    }

    JsonObject::JsonObject(std::string str)
        : _flags(0)
    {
        strn::trim(str);
        if (str.size() < 2 || *str.begin() != '{' || *(str.end() - 1) != '}')
            return;
        _string = str;
        _flags |= _stringValid;
    }

    bool JsonObject::_buildMap()
    {
        if (_flags & _dataValid)
            return true;
        size_t i = 1, tokStart = 1;
        uint16_t bracketLevel = 0;
        bool first = true;
        std::pair<std::string, JsonItem> keyValPair;
        bool inQuote = false, backslash = false, inColonComa = false;
        for (; i < _string.size() - 1; i++)
        {
            bool resetBackslash = backslash;

            switch (_string[i])
            {
            case '\\':
                backslash = !backslash;
                break;
            case '"':
                if (!backslash && bracketLevel == 0)
                {
                    inColonComa = false;
                    if (!inQuote)
                        tokStart = i + 1;
                    else if (first)
                    {
                        keyValPair.first = _string.substr(tokStart, i - tokStart);
                        first = false;
                    }
                    else
                    {
                        keyValPair.second = _string.substr(tokStart, i - tokStart);
                        _data.insert(keyValPair);
                        first = true;
                    }

                    inQuote = !inQuote;
                }
                break;
            case '{':
                if (!inQuote)
                    bracketLevel++;
                break;
            case '}':
                if (!inQuote)
                    bracketLevel--;
                break;
            case ':':
                if (!inQuote && bracketLevel == 0)
                {
                    inColonComa = true;
                    tokStart = i + 1;
                }
                break;
            case ',':
                if (inColonComa && bracketLevel == 0)
                    if (first)
                    {
                        keyValPair.first = _string.substr(tokStart, i - tokStart);
                        first = false;
                    }
                    else
                    {
                        keyValPair.second = _string.substr(tokStart, i - tokStart);
                        _data.insert(keyValPair);
                        first = true;
                    }
            }

            if (resetBackslash)
                backslash = false;
        }

        if (inColonComa)
        {
            keyValPair.second = _string.substr(tokStart, i - tokStart - 1);
            _data.insert(keyValPair);
        }

        _flags |= _dataValid;
    }
    bool JsonObject::_buildString()
    {
        // ToDo
    }

    JsonItem JsonObject::get(std::string key)
    {
        if (_data.contains(key))
            return _data[key];
        return JsonItem();
    }

    std::string JsonObject::getString()
    {
        if (!_buildString())
            return "";
        return _string;
    }
}