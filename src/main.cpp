#include <cstdlib>
#include <iostream>
#include "../inc/jsonParser.hpp"
#include "../inc/string.hpp"

std::string _string = "{ \n\
    \"Herausgeber\": \"Xema\", \n\
    \"Nummer\": \"1234-5678-9012-3456\", \n\
    \"Deckung\": 2e+6, \n\
    \"Waehrung\": \"EURO\", \n\
    \"Inhaber\": \n\
    { \n\
      \"Name\": \"Mustermann\", \n\
      \"Vorname\": \"Max\", \n\
      \"maennlich\": true, \n\
      \"Hobbys\": [\"Reiten\", \"Golfen\", \"Lesen\"], \n\
      \"Alter\": 42, \n\
      \"Kinder\": [], \n\
      \"Partner\": null \n\
    } \n\
  }";

int main(void)
{
    json::JsonObject jobj(_string);

    std::cout << jobj.getString();

    return EXIT_SUCCESS;
}

int main2(void)
{
    std::vector<std::pair<std::string, std::string>> _data;

    size_t i = 1, tokStart = 1;
    uint16_t bracketLevel = 0;
    bool first = true;
    std::pair<std::string, std::string> keyValPair;
    bool inQuote = false, backslash = false, inColonComa = false;
    for (; i < _string.size() - 1; i++)
    {
        bool resetBackslash = backslash;

        switch (_string[i])
        {
        case '\\':
            backslash = !backslash;
            break;
        case '"':
            if (!backslash && bracketLevel == 0)
            {
                inColonComa = false;
                if (!inQuote)
                    tokStart = i + 1;
                else if (first)
                {
                    keyValPair.first = _string.substr(tokStart, i - tokStart);
                    first = false;
                }
                else
                {
                    keyValPair.second = _string.substr(tokStart, i - tokStart);
                    _data.push_back(keyValPair);
                    first = true;
                }

                inQuote = !inQuote;
            }
            break;
        case '{':
            if (!inQuote)
                bracketLevel++;
            break;
        case '}':
            if (!inQuote)
                bracketLevel--;
            break;
        case ':':
            if (!inQuote && bracketLevel == 0)
            {
                inColonComa = true;
                tokStart = i + 1;
            }
            break;
        case ',':
            if (inColonComa && bracketLevel == 0)
                if (first)
                {
                    keyValPair.first = _string.substr(tokStart, i - tokStart);
                    first = false;
                }
                else
                {
                    keyValPair.second = _string.substr(tokStart, i - tokStart);
                    strn::trim(keyValPair.second);
                    _data.push_back(keyValPair);
                    first = true;
                }
        }

        if (resetBackslash)
            backslash = false;
    }

    if (inColonComa)
    {
        keyValPair.second = _string.substr(tokStart, i - tokStart - 1);
        strn::trim(keyValPair.second);
        _data.push_back(keyValPair);
    }

    for (const auto &e : _data)
    {
        std::cout << e.first << ": " << e.second << "\n";
    }

    return EXIT_SUCCESS;
}