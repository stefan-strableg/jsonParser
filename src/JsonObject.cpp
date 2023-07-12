#include "../inc/JsonObject.hpp"
#include "../inc/JsonArray.hpp"
#include "../inc/JsonValue.hpp"
#include "../inc/string.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

namespace json
{
    JsonObject::JsonObject()
        : JsonInterface(JsonInterfaceType::object)
    {
    }

    JsonObject::JsonObject(std::string str)
        : JsonInterface(JsonInterfaceType::object)
    {
        setString(str);
    }

    JsonObject::JsonObject(const JsonObject &other)
        : JsonObject()
    {
        _data.clear();
        for (const auto &item : other._data)
        {
            _data.insert({item.first, JsonInterface::makeNew(item.second->getString())});
        }
    }

    JsonObject::JsonObject(JsonObject &&other)
        : JsonObject()
    {
        _data.clear();
        for (auto &item : other._data)
        {
            _data.insert(item);
            item.second = nullptr;
        }
    }

    JsonObject &JsonObject::operator=(const JsonObject &other)
    {
        if (this == &other)
            return *this;

        for (auto &item : _data)
        {
            delete item.second;
        }

        _data.clear();

        for (const auto &item : other._data)
        {
            _data.insert({item.first, JsonInterface::makeNew(item.second->getString())});
        }
        return *this;
    }

    JsonObject &JsonObject::operator=(JsonObject &&other)
    {
        for (auto &item : _data)
        {
            delete item.second;
        }

        _data.clear();

        for (auto &item : other._data)
        {
            _data.insert(item);
            item.second = nullptr;
        }

        return *this;
    }

    void JsonObject::setString(std::string _string)
    {
        for (auto &item : _data)
        {
            delete item.second;
        }
        _data.clear();

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
                if (!inColonComma && bracesLevel == 0 && bracketsLevel == 0)
                {
                    if (!inQuote)
                        tokStart = i;
                    else
                    {
                        keyValPair.first = _string.substr(tokStart + 1, i - tokStart - 1);
                    }
                }
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

    bool JsonObject::readFromFile(std::string path)
    {
        std::ifstream inFile(path);
        if (!inFile.is_open())
            return false;
        std::string inFileContents((std::istreambuf_iterator<char>(inFile)),
                                   (std::istreambuf_iterator<char>()));
        setString(inFileContents);
        return true;
    }

    bool JsonObject::writeToFile(std::string path, JsonFormattingOptions options)
    {
        std::ofstream outFile(path);
        if (!outFile.is_open())
            return false;
        outFile << getStringF(options);
        return true;
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
        if (_data[key]->_getType() != JsonInterfaceType::value)
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

    std::string JsonObject::getType(std::string key)
    {
        if (!_data.contains(key))
            throw std::runtime_error("JsonObject::getType: there is no element with key " + key);

        switch (_data[key]->_getType())
        {
        case JsonInterfaceType::array:
            return "array";
        case JsonInterfaceType::object:
            return "object";
        case JsonInterfaceType::value:
            return "value";
        }

        return "invalid type";
    }

    void JsonObject::remove(std::string key)
    {
        if (!_data.contains(key))
            return;
        delete _data[key];
        _data.erase(key);
    }

    size_t JsonObject::size() const
    {
        return _data.size();
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

    bool JsonObject::_isBottomLayer() const
    {
        for (const auto &e : _data)
        {
            if (e.second->_getType() == JsonInterfaceType::object || e.second->_getType() == JsonInterfaceType::array)
                return false;
        }
        return true;
    }

    std::string JsonObject::getStringF(const JsonFormattingOptions &options, size_t tabs) const
    {
        std::ostringstream outStr;
        bool isInline = options.forceInline || (options.inlineBottomLevelObjects && _isBottomLayer() && getString().size() < options.maxLengthToInline);

        // if (!isInline && options.firstBracketInNewline && tabs != 0)
        // {
        //     outStr << '\n'
        //            << options.getTab(tabs);
        // }

        outStr << '{';

        if (isInline && options.spaceAfterOpeningBeforeClosingBrackets)
            outStr << ' ';

        if (!isInline)
            outStr << '\n';

        tabs++;

        size_t i = 0;
        for (auto &e : _data)
        {
            i++;
            if (!isInline)
                outStr << options.getTab(tabs);

            outStr << '\"' << e.first << '\"'
                   << (options.spaceBeforeColon ? " " : "")
                   << ':'
                   << (options.spaceAfterColon ? " " : "");

            if (!isInline && options.firstBracketInNewline && e.second->_getType() != JsonInterfaceType::value)
                outStr << '\n'
                       << options.getTab(tabs);
            outStr << e.second->getStringF(options, tabs);

            if (i < _data.size())
            {
                outStr << ',';
                if (options.spaceAfterComma)
                    outStr << ' ';
            }

            if (!isInline && (options.lastBracketInNewline || i < _data.size()))
                outStr << '\n';
        }

        if (!isInline && options.lastBracketInNewline)
            outStr << (tabs != 0 ? options.getTab(tabs - 1) : "");

        if (isInline && options.spaceAfterOpeningBeforeClosingBrackets)
            outStr << ' ';

        outStr << '}';

        return outStr.str();
    }

    JsonObject::~JsonObject()
    {
        for (auto &item : _data)
        {
            delete item.second;
        }
    }

}