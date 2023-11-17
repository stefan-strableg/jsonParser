#pragma once
#include "JsonEntity.hpp"
#include "string.hpp"
#include <sstream>
#include <iostream>
#include <map>

namespace json
{
    class Array;
    class Value;

    /// @brief Represents a JSON-object
    class Object : public JsonEntity
    {
    private:
        std::map<std::string, JsonEntity *> _data;

    public:
        /// @brief Default-Constructor
        Object();

        /// @brief Create from a JSON-string
        static Object createFromString(const std::string &str);

        /// @brief Copy-Constructor
        Object(const Object &other);

        /// @brief Move-Constructor
        Object(Object &&other);

        /// @brief Inserts an entity into the object
        template <typename T>
        inline void insert(std::string key, T value)
        {
            remove(key);
            _data.insert({key, JsonEntity::makeNew(value)});
        }

        /// @brief Constructor from individual key-value pairs
        template <typename T1, typename... Ts>
        inline Object(std::string key, T1 value, Ts... ts)
            : Object(ts...)
        {
            insert(key, value);
        }

        /// @brief Copy-Assignment operator
        Object &operator=(const Object &other);

        /// @brief Move-Assignment operator
        Object &operator=(Object &&other);

        /// @brief Set the JSON-string the object represents
        void fromString(std::string raw) override;
        /// @brief Get the JSON-string the object represents
        [[nodiscard]] std::string toString() const override;

        /// @brief Returns a formatted string.
        /// @param tabs  Indendation of all lines. Usually Zero.
        /// @param options Formatting options.
        [[nodiscard]] std::string toStringF(const FormattingOptions &options = defaultJsonFormattingOptions, size_t tabs = 0) const override;

        /// @brief Reads a .json file
        /// @param path A path accepted by fstream
        bool readFromFile(std::string path);

        /// @brief Writes to a .json file
        /// @param path A path accepted by fstream
        bool writeToFile(std::string path, FormattingOptions options = defaultJsonFormattingOptions) const;

        /// @brief Returns the value of key as an array. Throws a std::runtime_error if the nth entity is not an of another type.
        [[nodiscard]] Array &A(std::string key);

        /// @brief Returns the value of key as an object. Throws a std::runtime_error if the nth entity is not an of another type.
        [[nodiscard]] Object &O(std::string key);

        /// @brief Returns the value of key as a value. Throws a std::runtime_error if the nth entity is not an of another type.
        [[nodiscard]] Value &V(std::string key);

        /// @brief Returns the value of key as a compact string.
        [[nodiscard]] std::string S(std::string key) const;

        /// @brief Returns whether a value exists and is equal to "true" or not
        [[nodiscard]] bool getBool(std::string key) const;

        /// @brief Returns the value at key or an empty string if key doesnt exist or is no string
        [[nodiscard]] std::string getString(std::string key) const;

        /// @brief Gets the value of key.
        /// @tparam T Type of the entity. (Converted from string via operator<<(std::ostream&, std::string))
        template <typename T>
        [[nodiscard]] inline T get(std::string key) const
        {
            if (!_data.contains(key))
                return T();
            return strn::string_to<T>(_data.at(key)->toString());
        }

        /// @brief Returns the type of the value of key.
        /// @return "Array", "Object" or "Value"
        [[nodiscard]] std::string getType(std::string key) const;

        /// @brief Removes an element from the array. Does nothing if the element doesn't exist.
        void remove(std::string key);

        /// @brief Returns the number of entities in the array
        [[nodiscard]] size_t size() const override;

        /// @brief Returns true if the value of key is null
        [[nodiscard]] bool isNull(std::string key) const;

        /// @brief Returns true if the object contains key
        [[nodiscard]] bool contains(std::string key) const;

        /// @brief Returns true if the object is empty
        [[nodiscard]] bool isEmpty() const;

        /// @brief [library internal] Returns true when the array does not contain any arrays or objects.
        [[nodiscard]] bool isBottomLayer() const override;

        /// @brief Destructor
        ~Object();
    };

}