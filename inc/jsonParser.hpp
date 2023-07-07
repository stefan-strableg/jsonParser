#pragma once

#include <map>
#include <string>
#include <vector>

namespace json
{

class JsonObject
{
private:

    struct JsonObjectValue
    {
        const bool isObject;
        union ptr
        {
            std::vector<std::string>* array = nullptr;
            JsonObject* object;
        };
        std::string string;
    };

    static const uint8_t stringValid = 1 << 0;
    static const uint8_t _dataValid = 1 << 1;

    uint8_t flags;
    std::string _string;
    std::map<std::string, std::string> _data;

    template <typename ...T, typename T1>
    bool _parse(T1 item, T...);

    template <typename T1>
    bool _parse(T1 item);

public:

    bool parse(const std::string& jsonString);

    template <typename ...T>
    bool parse(T... );

    std::string getJsonString();

    JsonObject getJsonObject();

    std::string getString();

    bool getBool();

    int32_t getInt();

    float_t getFloat();
};
}