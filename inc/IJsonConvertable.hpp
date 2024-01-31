#pragma once

namespace json
{
    class JsonEntity;

    struct IJsonConvertable
    {
    public:
        virtual JsonEntity *getNewJsonEntity() const = 0;
    };
}