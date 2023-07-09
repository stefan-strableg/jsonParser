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
    private:
        void _freeValuePtr();

    public:
        enum JsonItemType : uint8_t
        {
            nonString,
            string,
            object,
            array
        } type = string;

        void *valuePtr = nullptr;

        template <typename T>
        T get();

        std::string getString();

        JsonItem() = default;
        JsonItem(const std::string &str);
        JsonItem(const JsonItem &other);
        JsonItem(JsonItem &&other);

        JsonItem &operator=(const JsonItem &other);
        JsonItem &operator=(JsonItem &&other);

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

        std::string getString();

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

        bool _buildMap();
        bool _buildString();

    public:
        void dump();

        JsonObject();
        JsonObject(std::string str);

        JsonItem get(std::string key);

        std::string getString();
    };
}