#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../inc/Json.hpp"

int main(void)
{

  json::FormattingOptions fopt;
  fopt.maxLengthToInline = 0;

  json::Object jobj("key1", "value1",
                    "key1.5", "rawr",
                    "key2", 42,
                    "key3", json::Array(1, 2, 3),
                    "key4", json::Array::createFromString("[1,2,\"lol\"]"),
                    "key5", json::Object::createFromString("{\"key\": \"val\", \"key2\": 42, \"key3\": 42}"));

  json::Array jarr("value1",
                   json::Raw("rawr"),
                   42,
                   json::Array(1, 2, 3),
                   json::Array::createFromString("[1,2,\"lol\"]"),
                   json::Object::createFromString("{\"key\": \"val\", \"key2\": 42, \"key3\": 42}"));

  std::cout << jarr.toStringF(fopt) << "\n";
  std::cout << jobj.toStringF(fopt) << "\n";

  return EXIT_SUCCESS;
}