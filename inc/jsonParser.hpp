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
        static const uint8_t _flagStringValid = 1 << 0;
        static const uint8_t _flagMapValid = 1 << 1;

        uint8_t _flags;
        std::string _string;
        std::vector<JsonItem> _data;

        void _validateMap();
        void _validateString();

        void _invalidateMap();
        void _invalidateString();

        JsonItem _at(size_t n);

    public:
        JsonArray();
        JsonArray(std::string str);

        template <typename T>
        T get(size_t n);

        JsonArray getA(size_t n);
        JsonObject getO(size_t n);
        std::string getS(size_t n);

        JsonArray &A(size_t n);
        JsonObject &O(size_t n);
        std::string &S(size_t n);

        std::string getString();
    };

    class JsonObject
    {
    private:
        static const uint8_t _flagStringValid = 1 << 0;
        static const uint8_t _flagMapValid = 1 << 1;

        uint8_t _flags;
        std::string _string;
        std::map<std::string, JsonItem> _data;

        void _validateMap();
        void _validateString();

        void _invalidateMap();
        void _invalidateString();

    public:
        void dump();

        void dumpMetadata();

        JsonObject();
        JsonObject(std::string str);

        template <typename T>
        T get(std::string key);

        void insert(std::string key, std::string item);

        JsonArray getA(std::string key);
        JsonObject getO(std::string key);
        std::string getS(std::string key);

        JsonArray &A(std::string key);
        JsonObject &O(std::string key);
        std::string &S(std::string key);

        std::string getString();
    };
}