#pragma once

#include <string>

namespace json
{
    struct JsonFormattingOptions
    {
        bool firstObjectBraceInNewLine = true;
        bool firstArrayBracketInNewLine = true;
        bool inlineArraysWhenPossible = false;  // NYI
        bool inlineObjectsWhenPossible = false; // NYI
        bool spaceBeforeColon = true;
        bool spaceAfterColon = true;
        bool spaceAfterComma = true; // NYI because inlineArraysWhenPossible is also NYI
        uint8_t tabSpaces = 2;       // 0 means use '\t' instead of spaces

        inline std::string getTab(uint8_t tabs) const
        {
            std::string ret;
            while (tabs--)
                ret += tabSpaces == 0 ? std::string(1, '\t') : std::string(tabSpaces, ' ');
            return ret;
        }
    };

    extern JsonFormattingOptions defaultJsonFormattingOptions;

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

        virtual std::string getStringF(size_t tabs = 0, const JsonFormattingOptions &options = defaultJsonFormattingOptions) const = 0;

        JsonInterface::JsonInterfaceType _getType();

        virtual ~JsonInterface();
    };
}

std::ostream &operator<<(std::ostream &os, const json::JsonInterface &j);