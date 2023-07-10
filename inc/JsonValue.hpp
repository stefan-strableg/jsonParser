#pragma once

#include "JsonInterface.hpp"

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
        T get();
    };
}