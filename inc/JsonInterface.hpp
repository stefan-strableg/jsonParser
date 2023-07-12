#pragma once

#include <string>

namespace json
{
    /// @brief Options to control the formatting of generated JSON-string
    struct JsonFormattingOptions
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
        /// @brief Keeps the output as compact as possible (essentially the same as calling getString() instead of getStringF())
        bool forceCompact = false;
        /// @brief How many spaces to use as a tab. Set to 0 to instead use a '\\t'
        uint8_t tabSpaces = 4;

        /// @brief [library internal] Returns a string representing the indentation
        [[nodiscard]] inline std::string _getTab(uint8_t tabs) const
        {
            std::string ret;
            while (tabs--)
                ret += tabSpaces == 0 ? std::string(1, '\t') : std::string(tabSpaces, ' ');
            return ret;
        }

        /// @brief Returns a example JSON-string to showcase the format
        [[nodiscard]] std::string getFormattingExample() const;
    };

    extern JsonFormattingOptions defaultJsonFormattingOptions;

    /// @brief Base class for all Json-items
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
        /// @brief [Library internal] Creates a new Json-item and returns a pointer to it.
        [[nodiscard]] static JsonInterface *makeNew(std::string str);

        /// @brief Parameterized Constructor
        JsonInterface(JsonInterfaceType t);

        /// @brief Set the JSON-string the object represents
        virtual void setString(std::string str) = 0;

        /// @brief Get the JSON-string the object represents
        [[nodiscard]] virtual std::string getString() const = 0;

        /// @brief Returns a formatted string.
        /// @param tabs At how many tabs to start. Usually zero.
        /// @param options Formatting options.
        [[nodiscard]] virtual std::string getStringF(const JsonFormattingOptions &options = defaultJsonFormattingOptions, size_t tabs = 0) const = 0;

        [[nodiscard]] virtual size_t size() const;

        /// @brief [library internal] Returns the type of the Json-item
        [[nodiscard]] JsonInterface::JsonInterfaceType _getType() const;

        /// @brief [library internal] Returns true when the array does not contain any arrays or objects.
        [[nodiscard]] virtual bool _isBottomLayer() const = 0;

        /// @brief Deconstructor
        virtual ~JsonInterface();
    };

    bool isValidJson(std::string str);
}

std::ostream &operator<<(std::ostream &os, const json::JsonInterface &j);