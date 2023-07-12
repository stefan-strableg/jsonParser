#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../inc/Json.hpp"

int main(void)
{
  json::JsonFormattingOptions options;
  options.inlineBottomLevelArrays = false;
  options.inlineBottomLevelObjects = false;
  options.firstBracketInNewline = true;

  json::JsonObject jobj;
  jobj.readFromFile("data/example.json");

  std::cout << jobj.getStringF();

  return EXIT_SUCCESS;
}