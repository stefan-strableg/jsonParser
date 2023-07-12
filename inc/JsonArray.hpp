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
        /// @brief Copy-Constructor
        JsonArray(const JsonArray &other);
        /// @brief Move-Constructor
        /// @brief Constructor from individual elements
        template <typename T, typename... Ts>
        JsonArray(T t, Ts... ts)
            : JsonArray()
        {
            _data.reserve(sizeof...(Ts) + 1);
            _initialize(t, ts...);
        }

        JsonArray(JsonArray &&other);
        /// @brief Copy-Assignment operator
        JsonArray &operator=(const JsonArray &other);
        /// @brief Move-Assignment operator
        JsonArray &operator=(JsonArray &&other);

        /// @brief Set the JSON-string the array represents
        void setString(std::string jsonString) override;
        /// @brief Get the JSON-String the array represents
        /// @note Use getStringF() for a formatted and more readable JSON-String
        [[nodiscard]] std::string getString() const override;
        /// @brief Returns a formatted string.
        /// @param tabs At how many tabs to start. Usually zero.
        /// @param options Formatting options.
        [[nodiscard]] std::string getStringF(const JsonFormattingOptions &options = defaultJsonFormattingOptions, size_t tabs = 0) const override;

        /// @brief Returns the nth item in the array as an array. Throws a std::runtime_error if the nth item is not an of another type.
        [[nodiscard]] JsonArray &A(size_t n);
        /// @brief Returns the nth item in the array as an object. Throws a std::runtime_error if the nth item is not an of another type.
        [[nodiscard]] JsonObject &O(size_t n);
        /// @brief Returns the nth item in the array as a value. Throws a std::runtime_error if the nth item is not an of another type.
        [[nodiscard]] JsonValue &V(size_t n);
        /// @brief Returns the nth item in the array as a compact string.
        [[nodiscard]] std::string S(size_t n) const;

        /// @brief Gets the nth item in the array.
        /// @tparam T Type of the item. (Converted from string via operator<<(std::ostream&, std::string))
        template <typename T>
        [[nodiscard]] inline T get(size_t n) const
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

        /// @brief Inserts an item at the index n
        template <typename T>
        inline void insert(size_t n, T val)
        {
            std::ostringstream inStr;
            inStr << val;
            _data.insert(_data.begin() + n, JsonInterface::makeNew(inStr.str()));
        }

        /// @brief Erases an item at the given index
        void erase(size_t n);

        /// @brief Erases items between start and end - 1 (inclusive)
        void erase(size_t start, size_t end);

        /// @brief Returns the type of the nth item in the array.
        /// @return "Array", "Object" or "Value"
        [[nodiscard]] std::string getType(size_t n) const;

        /// @brief Returns the number of items in the array
        [[nodiscard]] size_t size() const override;

        /// @brief [library internal] Returns true when the array does not contain any arrays or objects.
        [[nodiscard]] bool _isBottomLayer() const override;

        /// @brief Deconstructor
        ~JsonArray();
    };

}
