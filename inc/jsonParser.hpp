#pragma once

#include <map>
#include <string>
#include <vector>

namespace json
{
    class JsonArray;
    class JsonObject;
    struct JsonItem
    {
        enum : uint8_t
        {
            error,
            nonJson,
            object,
            array
        } type = nonJson;

        void *valuePtr = nullptr;

        template <typename T>
        T get();

        std::string getString();

        JsonItem() = default;
        JsonItem(const std::string &str);
        JsonItem(std::string &&str);
        ~JsonItem();
    };

    class JsonArray
    {
    private:
        static const uint8_t _stringValid = 1 << 0;
        static const uint8_t _dataValid = 1 << 1;

        uint8_t _flags;
        std::string _string;
        std::vector<JsonItem> _data;

        bool _buildMap();
        bool _buildString();

    public:
        JsonArray();
        JsonArray(std::string str);

        JsonItem at(size_t n);
    };

    class JsonObject
    {
    private:
        static const uint8_t _stringValid = 1 << 0;
        static const uint8_t _dataValid = 1 << 1;

        uint8_t _flags;
        std::string _string;
        std::map<std::string, JsonItem> _data;

        template <typename... T, typename T1>
        bool _parse(T1 item, T...);

        template <typename T1>
        bool _parse(T1 item);

        bool _buildMap();
        bool _buildString();

    public:
        JsonObject();
        JsonObject(std::string str);

        bool parse(const std::string &jsonString);

        template <typename... T>
        bool parse(T...);

        JsonItem get(std::string key);

        std::string getString();
    };
}