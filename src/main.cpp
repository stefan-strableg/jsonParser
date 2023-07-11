#include <cstdlib>
#include <iostream>
#include "../inc/Json.hpp"

std::string _string = "{ \n\
    \"Herausgeber\": \"Xema\", \n\
    \"Hobbys1\": [\"Reiten\", \"Golfen\", \"Lesen\"], \n\
    \"Nummer\": \"1234-5678-9012-3456\", \n\
    \"Deckung\": 2e+6, \n\
    \"Inhaber\": \n\
    { \n\
      \"Name\": \"Mustermann\", \n\
      \"Vorname\": \"Max\", \n\
      \"maennlich\": true, \n\
      \"Hobbys2\": [\"Reiten\", \"Golfen\", \"Lesen\"], \n\
      \"Alter\": 42, \n\
      \"Kinder\": [], \n\
      \"Partner\": null \n\
    }, \n\
    \"Waehrung\": \"EURO\" \n\
  }";

std::string jsonArrayString = "[\"Ch [] [ [] late\"    ,        42, 6.9, [true,   false, true],  {},\"Fourty-Seven\"]";

int main(void)
{
  // json::JsonObject obj("{\"Hello\":4, \"obj\": { \"Hello2\": \"World2\"}}");

  json::JsonObject obj("Hello", "\"World\"",
                       "Number", 420,
                       "Float", 7.2f);

  std::cout << obj.getString() << "\n";

  obj.V("Number").set(69);

  std::cout << obj.getString() << "\n";

  obj.remove("Hello");

  std::cout << obj.getString() << "\n";

  obj.insert("HelloObject", "null");

  std::cout << obj.getString() << "\n";

  obj.insert("HelloObject", json::JsonObject("Hello", "\"From inside Object\""));

  std::cout << obj.getString() << "\n";

  obj.O("HelloObject").insert("HelloArray", json::JsonArray("\"yarrA\"", 7.5f, 351, "\"Cheese\""));

  json::JsonFormattingOptions options;
  options.firstArrayBracketInNewLine = true;
  options.firstObjectBraceInNewLine = true;
  options.spaceAfterColon = false;
  options.spaceBeforeColon = false;

  std::cout << "Heres the object:\n"
            << obj.getStringF(0, options) << "\n";

  return EXIT_SUCCESS;
}