#pragma once

#include "JsonInterface.hpp"
#include "string.hpp"
#include <vector>

namespace json
{
    class JsonObject;
    class JsonValue;

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
        JsonArray();

        JsonArray(std::string str);
        JsonArray(const JsonArray &other);
        JsonArray(JsonArray &&other);
        JsonArray &operator=(const JsonArray &other);
        JsonArray &operator=(JsonArray &&other);

        template <typename T, typename... Ts>
        JsonArray(T t, Ts... ts)
            : JsonArray()
        {
            _data.reserve(sizeof...(Ts) + 1);
            _initialize(t, ts...);
        }

        void setString(std::string jsonString) override;

        std::string getString() const override;

        JsonArray &A(size_t n);
        JsonObject &O(size_t n);
        JsonValue &V(size_t n);
        std::string S(size_t n);

        std::string getType(size_t n);

        template <typename T>
        inline T get(size_t n)
        {
            if (_data.size() <= n)
                return T();
            return strn::string_to<T>(_data[n]->getString());
        }

        template <typename T>
        inline void push_back(T val)
        {
            std::ostringstream inStr;
            inStr << val;
            _data.push_back(JsonInterface::makeNew(inStr.str()));
        }

        std::string getStringF(size_t tabs = 0, const JsonFormattingOptions &options = defaultJsonFormattingOptions) const override;

        ~JsonArray();
    };

}
