#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../inc/Json.hpp"

int main(void)
{
  json::JsonFormattingOptions options;
  options.forceCompact = false;

  std::cout << options.getFormattingExample() << "\n";

  return EXIT_SUCCESS;
}