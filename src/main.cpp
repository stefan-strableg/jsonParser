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

  return EXIT_SUCCESS;
}