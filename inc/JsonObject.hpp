#pragma once
#include "JsonInterface.hpp"
#include "string.hpp"
#include <sstream>
#include <iostream>
#include <map>

namespace json
{
    class JsonArray;
    class JsonValue;

    class JsonObject : public JsonInterface
    {
    private:
        std::map<std::string, JsonInterface *> _data;

    public:
        JsonObject();

        JsonObject(std::string str);
        JsonObject(const JsonObject &other);
        JsonObject(JsonObject &&other);
        JsonObject &operator=(const JsonObject &other);
        JsonObject &operator=(JsonObject &&other);

        template <typename T>
        inline void insert(std::string key, T val)
        {
            remove(key);
            std::ostringstream inStr;
            inStr << val;
            _data.insert({key, JsonInterface::makeNew(inStr.str())});
        }

        template <typename T1, typename... Ts>
        inline JsonObject(std::string key, T1 value, Ts... ts)
            : JsonObject(ts...)
        {
            insert(key, value);
        }

        void setString(std::string str) override;

        std::string getString() const override;

        JsonArray &A(std::string key);
        JsonObject &O(std::string key);
        JsonValue &V(std::string key);
        std::string S(std::string key);

        std::string getType(std::string key);

        template <typename T>
        inline T get(std::string key)
        {
            if (!_data.contains(key))
                return T();
            return strn::string_to<T>(_data[key]->getString());
        }

        void remove(std::string key);

        bool isNull(std::string key);
        bool contains(std::string key);
        bool isEmpty();

        std::string getStringF(size_t tabs = 0, const JsonFormattingOptions &options = defaultJsonFormattingOptions) const override;

        ~JsonObject();
    };

}