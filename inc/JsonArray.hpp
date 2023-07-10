#pragma once

#include "JsonInterface.hpp"
#include <vector>

namespace json
{
    class JsonObject;
    class JsonValue;

    class JsonArray : public JsonInterface
    {
    private:
        std::vector<JsonInterface *> _data;

    public:
        JsonArray(std::string str);

        void setString(std::string jsonString) override;

        std::string getString() const override;

        JsonArray &A(size_t n);
        JsonObject &O(size_t n);
        JsonValue &V(size_t n);
        std::string S(size_t n);
    };
}