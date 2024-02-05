#include "../inc/Array.hpp"
#include "../inc/Object.hpp"
#include "../inc/Value.hpp"
#include "../inc/Raw.hpp"
#include <iostream>
#include <sstream>

namespace json
{
    JsonEntity *Array::toJson() const
    {
        return new Array(*this);
    }

    Array::Array()
        : JsonEntity(JsonEntityType::ARRAY)
    {
    }

    Array::Array(const Array &other)
        : Array()
    {
        for (const auto &entity : other._data)
        {
            _data.push_back(JsonEntity::_makeNew(*entity));
        }
    }

    Array::Array(Array &&other)
        : Array()
    {
        for (auto &entity : other._data)
        {
            _data.push_back(entity);
            entity = nullptr;
        }
    }

    Array Array::fromString(const std::string &str)
    {
        Array ret;
        ret.loadString(str);
        return ret;
    }

    Array &Array::operator=(const Array &other)
    {
        if (this == &other)
            return *this;

        for (auto &entity : _data)
        {
            delete entity;
        }

        _data.clear();

        for (const auto &entity : other._data)
        {
            _data.push_back({JsonEntity::_makeNew(*entity)});
        }
        return *this;
    }

    Array &Array::operator=(Array &&other)
    {
        std::destroy(_data.begin(), _data.end());

        _data.clear();

        for (auto &entity : other._data)
        {
            _data.push_back(entity);
            entity = nullptr;
        }

        return *this;
    }

    void Array::loadString(std::string raw)
    {
        size_t currentIndex = 1,
               tokenStartIndex = 1;
        uint16_t bracesLevel = 0,
                 bracketsLevel = 0;
        bool inQuote = false,
             backslash = false;

        for (; currentIndex < raw.size(); currentIndex++)
        {
            bool resetBackslash = backslash;

            switch (raw[currentIndex])
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
                    _data.push_back(JsonEntity::_makeNew(Raw(strn::trim_c(raw.substr(tokenStartIndex, currentIndex - tokenStartIndex)))));
                    tokenStartIndex = currentIndex + 1;
                }
            }

            if (resetBackslash)
                backslash = false;
        }
        _data.push_back(JsonEntity::_makeNew(Raw(strn::trim_c(raw.substr(tokenStartIndex, currentIndex - tokenStartIndex - 1)))));
    }

    std::string Array::toString() const
    {
        std::ostringstream outStream;

        outStream << '[';

        size_t i = 0;
        for (auto &e : _data)
        {
            i++;
            outStream << e->toString();
            if (i < _data.size())
                outStream << ',';
        }

        outStream << ']';

        return outStream.str();
    }

    Array &Array::A(size_t index)
    {
        if (_data.size() <= index)
            throw std::out_of_range("JsonArray::A: index " + std::to_string(index) + " is out of bounds");
        if (_data[index]->getType() != JsonEntityType::ARRAY)
            throw std::runtime_error("JsonArray::A: Element " + std::to_string(index) + " is not of type array");
        return dynamic_cast<Array &>(*_data[index]);
    }

    Object &Array::O(size_t index)
    {
        if (_data.size() <= index)
            throw std::out_of_range("JsonArray::O: index " + std::to_string(index) + " is out of bounds");
        if (_data[index]->getType() != JsonEntityType::OBJECT)
            throw std::runtime_error("JsonArray::O: Element " + std::to_string(index) + " is not of type object");
        return dynamic_cast<Object &>(*_data[index]);
    }

    Value &Array::V(size_t index)
    {
        if (_data.size() <= index)
            throw std::out_of_range("JsonArray::V: index " + std::to_string(index) + " is out of bounds");
        if (_data[index]->getType() != JsonEntityType::VALUE)
            throw std::runtime_error("JsonArray::V: Element " + std::to_string(index) + " is not of type VALUE");
        return dynamic_cast<Value &>(*_data[index]);
    }

    std::string Array::S(size_t index) const
    {
        if (_data.size() <= index)
            throw std::out_of_range("JsonArray::S: index " + std::to_string(index) + " is out of bounds");
        if (_data.at(index)->getType() != JsonEntityType::VALUE)
            throw std::runtime_error("JsonArray::S: Element " + std::to_string(index) + " is not of type string");
        return _data.at(index)->toString();
    }

    bool Array::getBool(size_t index) const
    {
        return _data.size() >= index && _data.at(index)->getType() == JsonEntityType::VALUE && _data.at(index)->toString() == "true";
    }

    std::string Array::getString(size_t index) const
    {
        std::string ret;
        if (_data.size() > index && _data.at(index)->getType() == JsonEntityType::VALUE)
            ret = _data.at(index)->toString();
        else
            return std::string();

        if (ret.size() < 2 || ret[0] != '\"' || ret[ret.size() - 1] != '\"')
            return std::string();

        return ret.substr(1, ret.size() - 2);
    }

    void Array::append(const Array &other)
    {
        _data.reserve(_data.size() + other._data.size());
        for (size_t i = 0; i < other._data.size(); i++)
        {
            _data.push_back(JsonEntity::_makeNew(*other._data.at(i)));
        }
    }

    void Array::remove(size_t index)
    {
        delete _data[index];
        _data.erase(_data.begin() + index);
    }

    void Array::remove(size_t start, size_t length)
    {
        std::destroy(_data.begin() + start, _data.begin() + start + length);
        _data.erase(_data.begin() + start, _data.begin() + start + length);
    }

    std::string Array::getType(size_t index) const
    {
        if (_data.size() <= index)
            throw std::out_of_range("JsonArray::getType index " + std::to_string(index) + " is out of bounds");

        switch (_data[index]->getType())
        {
        case JsonEntityType::ARRAY:
            return "array";
        case JsonEntityType::OBJECT:
            return "object";
        case JsonEntityType::VALUE:
            return "value";
        }
        return "invalid type";
    }

    size_t Array::size() const
    {
        return _data.size();
    }

    bool Array::isBottomLayer() const
    {
        for (const auto &entity : _data)
        {
            if (entity->getType() == JsonEntityType::OBJECT || entity->getType() == JsonEntityType::ARRAY)
                return false;
        }
        return true;
    }

    std::string Array::toStringF(const FormattingOptions &options, size_t tabs) const
    {
        if (options.forceCompact)
            return toString();
        std::ostringstream outStream;
        bool isInline = options.forceInline || (options.inlineShortBottomLevelArrays && isBottomLayer() && toString().size() < options.maxLengthToInline);

        // if (!isInline && (options.firstBracketInNewline) && tabs != 0)
        //     outStr << '\n'
        //            << options._getTab(tabs);

        outStream << '[';

        if (isInline && options.spaceAfterOpeningBeforeClosingBrackets)
            outStream << ' ';

        if (!isInline)
            outStream << '\n';

        tabs++;

        size_t i = 0;
        for (auto &entity : _data)
        {
            i++;

            if (!isInline)
                outStream << options._getTab(tabs);

            if (!isInline && options.firstBracketInNewline && entity->getType() != JsonEntityType::VALUE && entity->getType() != JsonEntityType::ARRAY)
                outStream << '\n'
                          << options._getTab(tabs);
            outStream << entity->toStringF(options, tabs);

            if (i < _data.size())
            {
                outStream << ',';
                if (options.spaceAfterComma && isInline)
                    outStream << ' ';
            }
            if (!isInline && (options.lastBracketInNewline || i < _data.size()))
                outStream << '\n';
        }

        if (!isInline)
            outStream << (tabs != 0 ? options._getTab(tabs - 1) : "");

        if (isInline && options.spaceAfterOpeningBeforeClosingBrackets)
            outStream << ' ';

        outStream << ']';

        return outStream.str();
    }

    Array::~Array()
    {
        for (const auto &entity : _data)
        {
            delete entity;
        }
    }

    std::vector<JsonEntity *>::iterator Array::begin()
    {
        return _data.begin();
    }

    std::vector<JsonEntity *>::iterator Array::end()
    {
        return _data.end();
    }

    std::vector<JsonEntity *>::const_iterator Array::cbegin()
    {
        return _data.cbegin();
    }

    std::vector<JsonEntity *>::const_iterator Array::cend()
    {
        return _data.cend();
    }
}
