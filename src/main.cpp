#include <cstdlib>
#include <iostream>
#include "../inc/jsonParser.hpp"
#include "../inc/string.hpp"

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
  json::JsonObject jobj(_string);

  // std::cout << jobj.getString() << "\n";

  jobj.A("Hobbys1") = json::JsonArray("[1, 2, 3]");
  jobj.O("Inhaber").A("Hobbys2").S(1) = "\"WOOOOOOHOOOOO\"";

  std::cout << jobj.getString() << "\n";

  return EXIT_SUCCESS;
}