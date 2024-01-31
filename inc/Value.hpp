#pragma once

#include "JsonEntity.hpp"
#include "string.hpp"

namespace json
{

    /// @brief Represents a JSON-value
    class Value : public JsonEntity
    {
    private:
        std::string _data;

    public:
        /// @brief Default Constructor
        Value();

        /// @brief Constructor from string
        Value(std::string raw);

        /// @brief Parameterized Constructor
        template <typename T>
        inline Value(T t)
            : Value()
        {
            std::ostringstream outStream;
            outStream << t;
            _data = outStream.str();
        }

        /// @brief Assignment operator
        template <typename T>
        inline Value &operator=(T t)
        {
            set(t);
            return *this;
        }

        /// @brief Sets the value
        void loadString(std::string raw) override;

        /// @brief Gets the value as raw string
        std::string toString() const override;

        /// @brief Same as toString in this class
        std::string toStringF(const FormattingOptions &options = defaultJsonFormattingOptions, size_t tabs = 0) const override;

        /// @brief Gets the value as T.
        /// @tparam T Type of the entity. (Converted from string via operator<<(std::ostream&, std::string))
        template <typename T>
        inline T get() const
        {
            return strn::string_to<T>(_data);
        }

        /// @brief Sets the entity
        template <typename T>
        inline void set(const T &val)
        {
            std::ostringstream outStream;
            outStream << val;
            _data = outStream.str();
        }

        /// @brief sets a given string as String. Same as set<std::string>("\"" + str + "\"")
        void setString(const std::string &str);

        /// @brief Returns the size of the object in JSON-entities
        size_t size() const override;

        /// @brief [library internal] Returns true when the array does not contain any arrays or objects.
        bool isBottomLayer() const override;
    };
}