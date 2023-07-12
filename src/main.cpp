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

  return EXIT_SUCCESS;
}