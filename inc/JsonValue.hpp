#pragma once

#include "JsonInterface.hpp"
#include "string.hpp"

namespace json
{
    class JsonValue : public JsonInterface
    {
    private:
        std::string _data;

    public:
        JsonValue(std::string str);

        void setString(std::string str) override;

        std::string getString() const override;

        template <typename T>
        inline T get()
        {
            return strn::string_to<T>(_data);
        }

        template <typename T>
        inline void set(T val)
        {
            std::ostringstream outStr;
            outStr << val;
            _data = outStr.str();
        }
    };
}