#pragma once

#include <string>
#include <cstdint>
#include <sstream>
#include <iostream>

#include "Conversions.hpp"
#include "compat.hpp"

namespace json
{

    class Object;
    class Array;
    class Value;

    /// @brief Options to control the formatting of generated JSON-string
    struct FormattingOptions
    {
        /// @brief Sets the first Bracket of Objects and Arrays to be in the next line, at the same indentation as the key
        bool firstBracketInNewline = false;
        /// @brief Sets the last Bracket of Objects and Arrays to be in the next line, at the same indentation as the key
        bool lastBracketInNewline = true;
        /// @brief Inlines short arrays if the contain no objects or arrays
        bool inlineShortBottomLevelArrays = true;
        /// @brief Inlines short objects if the contain no objects or arrays
        bool inlineShortBottomLevelObjects = true;
        /// @brief Only inlines objects that are below this character limit (to avoid long lines)
        size_t maxLengthToInline = 64;
        /// @brief Whether to put a space before colons in objects
        bool spaceBeforeColon = false;
        /// @brief Whether to put a space after colons in objects
        bool spaceAfterColon = true;
        /// @brief Whether to put a space after colons in arrays when inlining
        bool spaceAfterComma = true;
        /// @brief Whether to put a space after opening and before closing brackets
        bool spaceAfterOpeningBeforeClosingBrackets = true;
        /// @brief Forces the entire output to be inline
        bool forceInline = false;
        /// @brief Keeps the output as compact as possible (essentially the same as calling toString() instead of toStringF())
        bool forceCompact = false;
        /// @brief How many spaces to use as a tab. Set to 0 to instead use a '\t'
        uint8_t tabSpaces = 4;

        /// @brief Returns a example JSON-string to showcase the format
        [[nodiscard]] std::string getFormattingExample() const;

    private:
        friend Object;
        friend Array;
        friend Value;

        /// @brief [library internal] Returns a string representing the indentation
        [[nodiscard]] inline std::string _getTab(uint8_t tabs) const
        {
            std::string ret;
            while (tabs--)
                ret += tabSpaces == 0 ? std::string(1, '\t') : std::string(tabSpaces, ' ');
            return ret;
        }
    };

    extern FormattingOptions defaultJsonFormattingOptions;

    /// @brief Base class for all Json-entities
    class JsonEntity
    {
    public:
        enum JsonEntityType : uint8_t
        {
            VALUE,
            ARRAY,
            OBJECT
        } type;

        virtual inline JsonEntity *toJson() const { return nullptr; };

        /// @brief Parameterized Constructor
        JsonEntity(JsonEntityType type_);

        /// @brief Set the JSON-string the object represents
        virtual void loadString(std::string raw) = 0;

        /// @brief Get the JSON-string the object represents
        [[nodiscard]] virtual std::string toString() const = 0;

        /// @brief Returns a formatted string.
        /// @param tabs Indendation of all lines. Usually Zero.
        /// @param options Formatting options.
        [[nodiscard]] virtual std::string toStringF(const FormattingOptions &options = defaultJsonFormattingOptions, size_t tabs = 0) const = 0;

        [[nodiscard]] virtual size_t size() const = 0;

        /// @brief Returns the type of the Json-entity
        [[nodiscard]] JsonEntity::JsonEntityType getType() const;

        /// @brief Returns true when the array does not contain any arrays or objects. Used internally for formatting.
        [[nodiscard]] virtual bool isBottomLayer() const = 0;

        /// @brief Deconstructor
        virtual ~JsonEntity();

    private:
        /// @brief [Library internal] Creates a new Json-entity and returns a pointer to it.
        template <typename T>
        [[nodiscard]] static inline typename std::enable_if<isConvertibleToJson<T>::value, JsonEntity *>::type
        _makeNew(const T &jsonCloneable)
        {
            return jsonCloneable.toJson();
        }

        template <typename T>
        [[nodiscard]] static inline typename std::enable_if<!isConvertibleToJson<T>::value, JsonEntity *>::type
        _makeNew(const T &raw)
        {
            std::string str;
            std::ostringstream osstr;
            osstr << raw;
            str = osstr.str();

            return _makeNewFromString(str);
        }

        [[nodiscard]] static JsonEntity *_makeNew(const std::string &str)
        {
            return _makeNewValue("\"" + str + "\"");
        }

        [[nodiscard]] static JsonEntity *_makeNew(const char *str)
        {
            return _makeNew(std::string(str));
        }

        [[nodiscard]] static JsonEntity *_makeNewValue(const std::string &str);

        [[nodiscard]] static JsonEntity *_makeNewFromString(std::string raw);

        friend Object;
        friend Array;
        friend Value;
    };

}

std::ostream &operator<<(std::ostream &os, const json::JsonEntity &entity);