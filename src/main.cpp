#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../inc/Json.hpp"

int main(void)
{
  json::JsonObject jobj;
  jobj.readFromFile("data/json1.json");

  std::cout << jobj.getStringF() << '\n';

  for (auto &e : jobj.O("Inhaber").A("Hobbys"))
  {
    std::cout << e << '\n';
  }

  return EXIT_SUCCESS;
}