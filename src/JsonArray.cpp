#include "../inc/JsonArray.hpp"
#include "../inc/JsonObject.hpp"
#include "../inc/JsonValue.hpp"
#include <iostream>
#include <sstream>

namespace json
{

    JsonArray::JsonArray()
        : JsonInterface(JsonInterfaceType::array)
    {
    }

    JsonArray::JsonArray(const JsonArray &other)
        : JsonArray()
    {
        _data.clear();
        for (const auto &item : other._data)
        {
            _data.push_back(JsonInterface::makeNew(item->getString()));
        }
    }

    JsonArray::JsonArray(JsonArray &&other)
        : JsonArray()
    {
        _data.clear();
        for (auto &item : other._data)
        {
            _data.push_back(item);
            item = nullptr;
        }
    }

    JsonArray &JsonArray::operator=(const JsonArray &other)
    {
        if (this == &other)
            return *this;

        for (auto &item : _data)
        {
            delete item;
        }

        _data.clear();

        for (const auto &item : other._data)
        {
            _data.push_back({JsonInterface::makeNew(item->getString())});
        }
        return *this;
    }

    JsonArray &JsonArray::operator=(JsonArray &&other)
    {
        for (auto &item : _data)
        {
            delete item;
        }

        _data.clear();

        for (const auto &item : other._data)
        {
            _data.push_back(item);
        }
        return *this;
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

    std::string JsonArray::S(size_t n) const
    {
        if (_data.size() <= n)
            throw std::out_of_range("JsonArray::S: index " + std::to_string(n) + " is out of bounds");
        if (_data.at(n)->_getType() != JsonInterfaceType::value)
            throw std::runtime_error("JsonArray::S: Element " + std::to_string(n) + " is not of type string");
        return _data.at(n)->getString();
    }

    void JsonArray::erase(size_t n)
    {
        _data.erase(_data.begin() + n);
    }

    void JsonArray::erase(size_t start, size_t end)
    {
        _data.erase(_data.begin() + start, _data.begin() + end);
    }

    std::string JsonArray::getType(size_t n) const
    {
        if (_data.size() <= n)
            throw std::out_of_range("JsonArray::getType index " + std::to_string(n) + " is out of bounds");

        switch (_data[n]->_getType())
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

    size_t JsonArray::size() const
    {
        return _data.size();
    }

    bool JsonArray::_isBottomLayer() const
    {
        for (const auto &e : _data)
        {
            if (e->_getType() == JsonInterfaceType::object || e->_getType() == JsonInterfaceType::array)
                return false;
        }
        return true;
    }

    std::string JsonArray::getStringF(const JsonFormattingOptions &options, size_t tabs) const
    {
        if (options.forceCompact)
            return getString();
        std::ostringstream outStr;
        bool isInline = options.forceInline || (options.inlineShortBottomLevelArrays && _isBottomLayer() && getString().size() < options.maxLengthToInline);

        // if (!isInline && (options.firstBracketInNewline) && tabs != 0)
        //     outStr << '\n'
        //            << options._getTab(tabs);

        outStr << '[';

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
                outStr << options._getTab(tabs);

            if (!isInline && options.firstBracketInNewline && e->_getType() != JsonInterfaceType::value && e->_getType() != JsonInterfaceType::array)
                outStr << '\n'
                       << options._getTab(tabs);
            outStr << e->getStringF(options, tabs);

            if (i < _data.size())
            {
                outStr << ',';
                if (options.spaceAfterComma && isInline)
                    outStr << ' ';
            }
            if (!isInline && (options.lastBracketInNewline || i < _data.size()))
                outStr << '\n';
        }

        if (!isInline)
            outStr << (tabs != 0 ? options._getTab(tabs - 1) : "");

        if (isInline && options.spaceAfterOpeningBeforeClosingBrackets)
            outStr << ' ';

        outStr << ']';

        return outStr.str();
    }

    JsonArray::~JsonArray()
    {
        for (const auto &item : _data)
        {
            delete item;
        }
    }
}
