#pragma once

#include "JsonInterface.hpp"
#include "string.hpp"
#include <vector>

namespace json
{
    class JsonObject;
    class JsonValue;

    /// @brief Represents a JSON-array
    class JsonArray : public JsonInterface
    {
    private:
        std::vector<JsonInterface *> _data;

        template <typename T, typename... Ts>
        void _initialize(T t, Ts... ts)
        {
            push_back(t);
            _initialize(ts...);
        }

        void _initialize()
        {
        }

    public:
        /// @brief Default-Constructor
        JsonArray();

        /// @brief Constructor from a JSON-string
        JsonArray(std::string str);
        /// @brief Copy-Constructor
        JsonArray(const JsonArray &other);
        /// @brief Move-Constructor
        JsonArray(JsonArray &&other);
        /// @brief Copy-Assignment operator
        JsonArray &operator=(const JsonArray &other);
        /// @brief Move-Assignment operator
        JsonArray &operator=(JsonArray &&other);

        /// @brief Constructor from individual elements
        template <typename T, typename... Ts>
        JsonArray(T t, Ts... ts)
            : JsonArray()
        {
            _data.reserve(sizeof...(Ts) + 1);
            _initialize(t, ts...);
        }

        /// @brief Set the JSON-string the array represents
        void setString(std::string jsonString) override;

        /// @brief Get the JSON-String the array represents
        /// @note Use getStringF() for a formatted and more readable JSON-String
        std::string getString() const override;

        /// @brief Returns the nth item in the array as an array. Throws a std::runtime_error if the nth item is not an of another type.
        JsonArray &A(size_t n);
        /// @brief Returns the nth item in the array as an object. Throws a std::runtime_error if the nth item is not an of another type.
        JsonObject &O(size_t n);
        /// @brief Returns the nth item in the array as a value. Throws a std::runtime_error if the nth item is not an of another type.
        JsonValue &V(size_t n);
        /// @brief Returns the nth item in the array as a compact string.
        std::string S(size_t n);

        /// @brief Returns the type of the nth item in the array.
        /// @return "Array", "Object" or "Value"
        std::string getType(size_t n);

        /// @brief Returns the number of items in the array
        size_t size() const override;

        /// @brief Gets the nth item in the array.
        /// @tparam T Type of the item. (Converted from string via operator<<(std::ostream&, std::string))
        template <typename T>
        inline T get(size_t n)
        {
            if (_data.size() <= n)
                return T();
            return strn::string_to<T>(_data[n]->getString());
        }

        /// @brief Adds an item to the back of the array
        template <typename T>
        inline void push_back(T val)
        {
            std::ostringstream inStr;
            inStr << val;
            _data.push_back(JsonInterface::makeNew(inStr.str()));
        }

        /// @brief [library internal] Returns true when the array does not contain any arrays or objects.
        bool _isBottomLayer() const override;

        /// @brief Returns a formatted string.
        /// @param tabs At how many tabs to start. Usually zero.
        /// @param options Formatting options.
        std::string getStringF(size_t tabs = 0, const JsonFormattingOptions &options = defaultJsonFormattingOptions) const override;

        /// @brief Deconstructor
        ~JsonArray();
    };

}
