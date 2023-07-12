#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../inc/Json.hpp"

int main(void)
{
  json::JsonObject jobj;
  jobj.readFromFile("data/example.json");

  json::JsonFormattingOptions options;
  options.tabSpaces = 0;
  options.firstBracketInNewline = true;
  options.spaceBeforeColon = true;
  options.inlineBottomLevelArrays = false;
  options.inlineBottomLevelObjects = false;

  std::cout << jobj.getStringF(options) << "\n";
  jobj.V("Nationalitaet") = "\"US\"";
  jobj.insert("Partei", "\"Republican Party\"");
  jobj.insert("Lieblingszahlen", json::JsonArray("\"Vierzehn\"", 42, 6.9, "\"Sieben\""));
  jobj.insert("Verschachtelte Arrays", json::JsonArray(json::JsonArray(json::JsonArray("\"Lol\"").getString()).getString()));

  jobj.writeToFile("data/output.json", options);

  jobj.readFromFile("data/json1.json");
  jobj.writeToFile("data/output1.json");

  jobj.readFromFile("data/json2.json");
  jobj.writeToFile("data/output2.json");

  return EXIT_SUCCESS;
}