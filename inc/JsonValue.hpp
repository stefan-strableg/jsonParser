#pragma once

#include "JsonInterface.hpp"
#include "string.hpp"

namespace json
{

    /// @brief Represents a JSON-value
    class JsonValue : public JsonInterface
    {
    private:
        std::string _data;

    public:
        /// @brief Constructor from string
        JsonValue(std::string str);

        /// @brief Parameterized Constructor
        template <typename T>
        inline JsonValue(T t)
        {
            std::ostringstream outStr;
            outStr << t;
            _data = outStr.str();
        }

        /// @brief Assignment operator
        template <typename T>
        inline JsonValue &operator=(T t)
        {
            set(t);
            return *this;
        }

        /// @brief Sets the value
        void setString(std::string str) override;

        /// @brief Gets the value as raw string
        std::string getString() const override;

        /// @brief Gets the value as T.
        /// @tparam T Type of the item. (Converted from string via operator<<(std::ostream&, std::string))
        template <typename T>
        inline T get() const
        {
            return strn::string_to<T>(_data);
        }

        /// @brief Sets the item
        template <typename T>
        inline void set(T val)
        {
            std::ostringstream outStr;
            outStr << val;
            _data = outStr.str();
        }

        /// @brief Returns the size of the object in JSON-items
        size_t size() const override;

        /// @brief [library internal] Returns true when the array does not contain any arrays or objects.
        bool _isBottomLayer() const override;

        /// @brief Same as getStringF in this class
        std::string getStringF(const JsonFormattingOptions &options = defaultJsonFormattingOptions, size_t tabs = 0) const override;
    };
}