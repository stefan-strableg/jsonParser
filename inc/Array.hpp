#pragma once

#include "JsonEntity.hpp"
#include "string.hpp"
#include "compat.hpp"

#include <vector>

namespace json
{
    class Object;
    class Value;

    /// @brief Represents a JSON-array
    class Array : public JsonEntity
    {
    private:
        std::vector<JsonEntity *> _data;

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
        JsonEntity *getJsonClone() const override;

        /// @brief Default-Constructor
        Array();

        /// @brief Copy-Constructor
        Array(const Array &other);

        /// @brief Create from a JSON-string
        static Array fromString(const std::string &str);

        /// @brief Move-Constructor
        Array(Array &&other);

        /// @brief Create from individual elements
        template <typename T, typename... Ts>
        static inline Array fromValues(T t, Ts... ts)
        {
            Array ret;
            ret._data.reserve(sizeof...(Ts) + 1);
            ret._initialize(t, ts...);
            return ret;
        }

        template <typename Container>
        static inline Array fromContainer(Container &container)
        {
            return fromContainer(container.begin(), container.end());
        }

        template <typename Iterator>
        static inline Array fromContainer(Iterator begin, Iterator end)
        {
            Array ret;

            ret._data.reserve(end - begin);
            while (begin != end)
            {
                ret.push_back(*(begin++));
            }

            return ret;
        }

        /// @brief Copy-Assignment operator
        Array &operator=(const Array &other);

        /// @brief Move-Assignment operator
        Array &operator=(Array &&other);

        /// @brief Set the JSON-string the array represents
        void loadString(std::string raw) override;
        /// @brief Get the JSON-String the array represents
        /// @note Use toStringF() for a formatted and more readable JSON-String
        [[nodiscard]] std::string toString() const override;

        /// @brief Returns a formatted string.
        /// @param tabs  Indendation of all lines. Usually Zero.
        /// @param options Formatting options.
        [[nodiscard]] std::string toStringF(const FormattingOptions &options = defaultJsonFormattingOptions, size_t tabs = 0) const override;

        /// @brief Returns the nth entity in the array as an array. Throws a std::runtime_error if the nth entity is not an of another type.
        [[nodiscard]] Array &A(size_t index);

        /// @brief Returns the nth entity in the array as an object. Throws a std::runtime_error if the nth entity is not an of another type.
        [[nodiscard]] Object &O(size_t index);

        /// @brief Returns the nth entity in the array as a value. Throws a std::runtime_error if the nth entity is not an of another type.
        [[nodiscard]] Value &V(size_t index);

        /// @brief Returns the nth entity in the array as a compact string.
        [[nodiscard]] std::string S(size_t index) const;

        /// @brief Returns whether a value exists and is equal to "true" or not
        [[nodiscard]] bool getBool(size_t index) const;

        /// @brief Returns the value at key or an empty string if key doesnt exist or is no string
        [[nodiscard]] std::string getString(size_t index) const;

        /// @brief Gets the nth entity in the array.
        /// @tparam T Type of the entity. (Converted from string via operator<<(std::ostream&, std::string))
        template <typename T>
        [[nodiscard]] inline T get(size_t index) const
        {
            if (_data.size() <= index)
                return T();
            return strn::string_to<T>(_data[index]->toString());
        }

        /// @brief Adds an entity to the back of the array
        template <typename T>
        inline void push_back(T value)
        {
            insert(_data.size(), value);
        }

        /// @brief Inserts an entity at the index n
        template <typename T>
        inline void insert(size_t n, T value)
        {
            _data.insert(_data.begin() + n, JsonEntity::makeNew(value));
        }

        /// @brief Erases an entity at the given index
        void remove(size_t index);

        /// @brief Erases entities between start and end - 1 (inclusive)
        void remove(size_t start, size_t length);

        /// @brief Returns the type of the nth entity in the array.
        /// @return "Array", "Object" or "Value"
        [[nodiscard]] std::string getType(size_t index) const;

        /// @brief Returns the number of entities in the array
        [[nodiscard]] size_t size() const override;

        /// @brief [library internal] Returns true when the array does not contain any arrays or objects.
        [[nodiscard]] bool isBottomLayer() const override;

        /// @brief Deconstructor
        ~Array();
    };
}
