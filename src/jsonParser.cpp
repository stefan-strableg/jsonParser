#include "../inc/jsonParser.hpp"
#include "../inc/string.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace json
{

    template <typename T>
    T JsonItem::get()
    {
        if (type != string)
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
        if (strt.size() > 2 && *strt.begin() == '{' && *(strt.end() - 1) == '}')
        {
            type = JsonItemType::object;
            valuePtr = (void *)new JsonObject(strt);
        }
        else if (strt.size() > 2 && *strt.begin() == '[' && *(strt.end() - 1) == ']')
        {
            type = JsonItemType::array;
            valuePtr = (void *)new JsonArray(strt);
        }
        else if (strt.size() > 2 && *strt.begin() == '\"' && *(strt.end() - 1) == '\"')
        {
            type = JsonItemType::string;
            valuePtr = (void *)new std::string(strt);
        }
        else
        {
            type = JsonItemType::nonString;
            valuePtr = (void *)new std::string(strt);
        }
    }

    std::string JsonItem::getString()
    {
        if (type == object)
            return static_cast<JsonObject *>(valuePtr)->getString();

        if (type == array)
            return static_cast<JsonArray *>(valuePtr)->getString();

        if (type == string)
            return *static_cast<std::string *>(valuePtr);

        if (type == nonString)
            return *static_cast<std::string *>(valuePtr);

        return "";
    }

    void JsonItem::_freeValuePtr()
    {
        switch (type)
        {
        case JsonItemType::array:
            delete static_cast<JsonArray *>(valuePtr);
            break;
        case JsonItemType::object:
            delete static_cast<JsonObject *>(valuePtr);
            break;
        case JsonItemType::string:
        case JsonItemType::nonString:
            delete static_cast<std::string *>(valuePtr);
            break;
        }
    }

    JsonItem::~JsonItem()
    {
        _freeValuePtr();
    }

    JsonItem::JsonItem(const JsonItem &other)
        : type(other.type)
    {
        switch (type)
        {
        case JsonItemType::object:
            valuePtr = static_cast<void *>(new JsonObject(*static_cast<JsonObject *>(other.valuePtr)));
            break;
        case JsonItemType::array:
            valuePtr = static_cast<void *>(new JsonArray(*static_cast<JsonArray *>(other.valuePtr)));
            break;
        case JsonItemType::string:
        case JsonItemType::nonString:
            valuePtr = static_cast<void *>(new std::string(*static_cast<std::string *>(other.valuePtr)));
            break;
        }
    }

    JsonItem::JsonItem(JsonItem &&other)
        : type(other.type), valuePtr(other.valuePtr)
    {
        other.valuePtr = nullptr;
    }

    JsonItem &JsonItem::operator=(const JsonItem &other)
    {
        if (this == &other)
            return *this;

        _freeValuePtr();
        type = other.type;
        switch (type)
        {
        case JsonItemType::object:
            valuePtr = static_cast<void *>(new JsonObject(*static_cast<JsonObject *>(other.valuePtr)));
        case JsonItemType::array:
            valuePtr = static_cast<void *>(new JsonArray(*static_cast<JsonArray *>(other.valuePtr)));
        case JsonItemType::string:
        case JsonItemType::nonString:
            valuePtr = static_cast<void *>(new std::string(*static_cast<std::string *>(other.valuePtr)));
        }

        return *this;
    }

    JsonItem &JsonItem::operator=(JsonItem &&other)
    {
        if (this == &other)
            return *this;

        _freeValuePtr();
        type = other.type;
        valuePtr = other.valuePtr;
        other.valuePtr = nullptr;

        return *this;
    }

    JsonArray::JsonArray()
        : _flags(0)
    {
    }

    JsonArray::JsonArray(std::string str)
        : _flags(0)
    {
        strn::trim(str);
        if (str.size() < 2 || *str.begin() != '[' || *(str.end() - 1) != ']')
            return;
        _string = str;
        _flags |= _flagStringValid;
        _buildMap();
        _flags &= ~_flagStringValid;
        _buildString();
    }

    bool JsonArray::_buildMap()
    {
        if (_flags & _flagDataValid)
            return true;
        if (!(_flags & _flagStringValid))
            return false;

        size_t i = 1,
               tokStart = 1;
        uint16_t bracesLevel = 0,
                 bracketsLevel = 0;
        bool inQuote = false,
             backslash = false;

        for (; i < _string.size(); i++)
        {
            bool resetBackslash = backslash;

            switch (_string[i])
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
                    _data.push_back(_string.substr(tokStart, i - tokStart));
                    tokStart = i + 1;
                }
            }
            std::cout << "\n"
                      << "\n"
                      << _string.substr(0, i + 1) << "\n"
                      << "inQuote: " << inQuote << "\n"
                      << "[] - level: " << bracketsLevel << "\n"
                      << "{} - level: " << bracesLevel << "\n";
            for (auto &e : _data)
            {
                std::cout << e.getString() << "\n";
            }
            std::cin.get();

            if (resetBackslash)
                backslash = false;
        }
        _data.push_back(_string.substr(tokStart, i - tokStart - 1));

        _flags |= _flagDataValid;

        return true;
    }

    bool JsonArray::_buildString()
    {

        if (_flags & _flagStringValid)
            return true;
        if (!(_flags & _flagDataValid))
            return false;

        std::ostringstream outStr;

        outStr << '[';

        size_t i = 0;
        for (auto &e : _data)
        {
            i++;
            outStr << e.getString();
            if (i < _data.size())
                outStr << ',';
        }

        outStr << ']';

        _string = outStr.str();
        _flags |= _flagStringValid;
        return true;
    }

    std::string JsonArray::getString()
    {
        if (!_buildString())
            return "";
        return _string;
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
        : JsonObject()
    {
        strn::trim(str);
        if (str.size() < 2 || *str.begin() != '{' || *(str.end() - 1) != '}')
            return;
        _string = str;
        _flags |= _flagStringValid;
        _buildMap();
        _flags &= ~_flagStringValid;
        _buildString();
    }

    bool JsonObject::_buildMap()
    {
        if (_flags & _flagDataValid)
            return true;
        if (!(_flags & _flagStringValid))
            return false;

        size_t i = 1,
               tokStart = 1;
        uint16_t bracesLevel = 0,
                 bracketsLevel = 0;
        std::pair<std::string, JsonItem> keyValPair = {"", JsonItem("")};
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
                    std::cout << "substr(" << tokStart << ", " << i - tokStart << ") = " << _string.substr(tokStart, i - tokStart) << "\n";
                    keyValPair.second = _string.substr(tokStart, i - tokStart);
                    _data.insert(keyValPair);
                    inColonComma = false;
                }
            }

            if (resetBackslash)
                backslash = false;

            std::cout << "\n"
                      << "\n"
                      << _string.substr(0, i + 1) << "\n"
                      << "inQuote: " << inQuote << "\n"
                      << "inColonComma: " << inColonComma << "\n"
                      << "[] - level: " << bracketsLevel << "\n"
                      << "{} - level: " << bracesLevel << "\n"
                      << "KeyValPair: {" << keyValPair.first << ":" << keyValPair.second.getString() << "}\n";
            for (auto &e : _data)
            {
                std::cout << e.first << ": " << e.second.getString() << "\n";
            }
            std::cin.get();
        }

        if (inColonComma)
        {
            keyValPair.second = _string.substr(tokStart, i - tokStart);
            _data.insert(keyValPair);
        }

        _flags |= _flagDataValid;

        return true;
    }

    bool JsonObject::_buildString()
    {
        if (_flags & _flagStringValid)
            return true;
        if (!(_flags & _flagDataValid))
            return false;

        std::ostringstream outStr;

        outStr << '{';

        size_t i = 0;
        for (auto &e : _data)
        {
            i++;
            outStr << '\"' << e.first << "\":" << e.second.getString();
            if (i < _data.size())
                outStr << ',';
        }

        outStr << '}';

        _string = outStr.str();
        _flags |= _flagStringValid;
        return true;
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

    void JsonObject::dump()
    {
        _buildMap();
        std::cout << "Dumping " << _data.size() << " objects:\n";
        for (auto &e : _data)
        {
            std::cout << e.first << ": " << e.second.getString() << "\n";
        }
    }

    void JsonObject::dumpMetadata()
    {
        std::cout << "String valid: " << (_flags & _flagStringValid ? "YES" : "NO") << "\n"
                  << "Data valid: " << (_flags & _flagDataValid ? "YES" : "NO") << "\n";
    }
}