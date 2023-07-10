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
    }

    void JsonArray::_validateMap()
    {
        if (_flags & _flagMapValid)
            return;
        if (!(_flags & _flagStringValid))
            throw std::runtime_error("JsonArray::_validateMap: Cannot build map, string is not valid");

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
            // std::cout << "\n"
            //           << "\n"
            //           << _string.substr(0, i + 1) << "\n"
            //           << "inQuote: " << inQuote << "\n"
            //           << "[] - level: " << bracketsLevel << "\n"
            //           << "{} - level: " << bracesLevel << "\n";
            // for (auto &e : _data)
            // {
            //     std::cout << e.getString() << "\n";
            // }
            // std::cin.get();

            if (resetBackslash)
                backslash = false;
        }
        _data.push_back(_string.substr(tokStart, i - tokStart - 1));

        _flags |= _flagMapValid;
    }

    void JsonArray::_validateString()
    {
        if (_flags & _flagStringValid)
            return;
        if (!(_flags & _flagMapValid))
            throw std::runtime_error("JsonArray::_validateString: Cannot build string, map is invalid");

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
    }

    void JsonArray::_invalidateMap()
    {
        _flags &= ~_flagMapValid;
    }

    void JsonArray::_invalidateString()
    {
        _flags &= ~_flagStringValid;
    }

    std::string JsonArray::getString()
    {
        _validateString();
        return _string;
    }

    JsonItem JsonArray::_at(size_t n)
    {
        _validateMap();
        if (_data.size() <= n)
            return JsonItem();

        return _data[n];
    }

    template <typename T>
    T JsonArray::get(size_t n)
    {
        _validateMap();
        return strn::string_to<T>(_at(n).getString());
    }

    template <>
    std::string JsonArray::get(size_t n)
    {
        _validateMap();
        return _at(n).getString();
    }

    template <>
    JsonArray JsonArray::get(size_t n)
    {
        _validateMap();
        auto item = _at(n);
        if (item.type != JsonItem::JsonItemType::array)
            return JsonArray();
        return *static_cast<JsonArray *>(item.valuePtr);
    }

    template <>
    JsonObject JsonArray::get(size_t n)
    {
        _validateMap();
        auto item = _at(n);
        if (item.type != JsonItem::JsonItemType::object)
            return JsonObject();
        return *static_cast<JsonObject *>(item.valuePtr);
    }

    JsonArray JsonArray::getA(size_t n)
    {
        return get<JsonArray>(n);
    }

    JsonObject JsonArray::getO(size_t n)
    {
        return get<JsonObject>(n);
    }

    std::string JsonArray::getS(size_t n)
    {
        return get<std::string>(n);
    }

    JsonArray &JsonArray::A(size_t n)
    {
        _validateMap();
        if (_data.size() <= n)
            throw std::runtime_error("JsonArray::A: Index out of range");
        if (_data[n].type != JsonItem::JsonItemType::array)
            throw std::runtime_error("JsonArray::A: JsonObject's index " + std::to_string(n) + " is not of type array");

        _invalidateString();
        return *static_cast<JsonArray *>(_data[n].valuePtr);
    }

    JsonObject &JsonArray::O(size_t n)
    {
        _validateMap();
        if (_data.size() <= n)
            throw std::runtime_error("JsonArray::O: Index out of range");
        if (_data[n].type != JsonItem::JsonItemType::object)
            throw std::runtime_error("JsonArray::O: JsonObject's index " + std::to_string(n) + " is not of type object");

        _invalidateString();
        return *static_cast<JsonObject *>(_data[n].valuePtr);
    }

    std::string &JsonArray::S(size_t n)
    {
        _validateMap();
        if (_data.size() <= n)
            throw std::runtime_error("JsonArray::S: Index out of range");
        if (_data[n].type != JsonItem::JsonItemType::string && _data[n].type != JsonItem::JsonItemType::nonString)
            throw std::runtime_error("JsonArray::S: JsonObject's index " + std::to_string(n) + " is not of type string");

        _invalidateString();
        return *static_cast<std::string *>(_data[n].valuePtr);
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
    }

    void JsonObject::_validateMap()
    {
        if (_flags & _flagMapValid)
            return;
        if (!(_flags & _flagStringValid))
            throw std::runtime_error("JsonObject::_validateMap: Cannot build Map, string is invalid");

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
                    keyValPair.second = _string.substr(tokStart, i - tokStart);
                    _data.insert(keyValPair);
                    inColonComma = false;
                }
            }

            if (resetBackslash)
                backslash = false;

            // std::cout << "\n"
            //           << "\n"
            //           << _string.substr(0, i + 1) << "\n"
            //           << "inQuote: " << inQuote << "\n"
            //           << "inColonComma: " << inColonComma << "\n"
            //           << "[] - level: " << bracketsLevel << "\n"
            //           << "{} - level: " << bracesLevel << "\n"
            //           << "KeyValPair: {" << keyValPair.first << ":" << keyValPair.second.getString() << "}\n";
            // for (auto &e : _data)
            // {
            //     std::cout << e.first << ": " << e.second.getString() << "\n";
            // }
            // std::cin.get();
        }

        if (inColonComma)
        {
            keyValPair.second = _string.substr(tokStart, i - tokStart);
            _data.insert(keyValPair);
        }

        _flags |= _flagMapValid;
    }

    void JsonObject::_validateString()
    {
        if (_flags & _flagStringValid)
            return;
        if (!(_flags & _flagMapValid))
            throw std::runtime_error("JsonObject::_validateString: Cannot build string, map is invalid");

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
    }

    void JsonObject::_invalidateMap()
    {
        _flags &= ~_flagMapValid;
    }

    void JsonObject::_invalidateString()
    {
        _flags &= ~_flagStringValid;
    }

    template <typename T>
    T JsonObject::get(std::string key)
    {
        _validateMap();
        if (!_data.contains(key))
            return T();
        return strn::string_to<T>(_data[key].getString());
    }

    template <>
    std::string JsonObject::get(std::string key)
    {
        _validateMap();
        if (!_data.contains(key))
            return "";

        return _data[key].getString();
    }

    template <>
    JsonArray JsonObject::get(std::string key)
    {
        _validateMap();
        if (!_data.contains(key))
            return JsonArray();
        if (_data[key].type != JsonItem::JsonItemType::array)
            return JsonArray();
        return *static_cast<JsonArray *>(_data[key].valuePtr);
    }

    template <>
    JsonObject JsonObject::get(std::string key)
    {
        _validateMap();
        if (!_data.contains(key))
            return JsonObject();
        if (_data[key].type != JsonItem::JsonItemType::object)
            return JsonObject();
        return *static_cast<JsonObject *>(_data[key].valuePtr);
    }

    JsonArray JsonObject::getA(std::string key)
    {
        return get<JsonArray>(key);
    }

    JsonObject JsonObject::getO(std::string key)
    {
        return get<JsonObject>(key);
    }

    std::string JsonObject::getS(std::string key)
    {
        return get<std::string>(key);
    }

    JsonArray &JsonObject::A(std::string key)
    {
        _validateMap();
        if (!_data.contains(key))
            throw std::runtime_error("JsonObject::A: JsonObject does not contain a member called " + key);
        if (_data[key].type != JsonItem::JsonItemType::array)
            throw std::runtime_error("JsonObject::A: JsonObject's member " + key + " is not of type array");

        _invalidateString();
        return *static_cast<JsonArray *>(_data[key].valuePtr);
    }

    JsonObject &JsonObject::O(std::string key)
    {
        _validateMap();
        if (!_data.contains(key))
            throw std::runtime_error("JsonObject::O: JsonObject does not contain a member called " + key);
        if (_data[key].type != JsonItem::JsonItemType::object)
            throw std::runtime_error("JsonObject::O: JsonObject's member " + key + " is not of type object");

        _invalidateString();
        return *static_cast<JsonObject *>(_data[key].valuePtr);
    }

    std::string &JsonObject::S(std::string key)
    {
        _validateMap();
        if (!_data.contains(key))
            throw std::runtime_error("JsonObject::S: JsonObject does not contain a member called " + key);
        if (_data[key].type != JsonItem::JsonItemType::string && _data[key].type != JsonItem::JsonItemType::nonString)
            throw std::runtime_error("JsonObject::S: JsonObject's member " + key + " is not of type string or nonString");

        _invalidateString();
        return *static_cast<std::string *>(_data[key].valuePtr);
    }

    void JsonObject::insert(std::string key, std::string item)
    {
        _data.insert({key, JsonItem(item)});
        _flags &= ~_flagStringValid;
    }

    std::string JsonObject::getString()
    {
        _validateString();
        return _string;
    }

    void JsonObject::dump()
    {
        _validateMap();
        std::cout << "Dumping " << _data.size() << " objects:\n";
        for (auto &e : _data)
        {
            std::cout << e.first << ": " << e.second.getString() << "\n";
        }
    }

    void JsonObject::dumpMetadata()
    {
        _validateMap();
        std::cout << "String valid: " << (_flags & _flagStringValid ? "YES" : "NO") << "\n"
                  << "Data valid: " << (_flags & _flagMapValid ? "YES" : "NO") << "\n";
    }
}