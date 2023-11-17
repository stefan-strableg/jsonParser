#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../inc/Json.hpp"

int main(void)
{
  json::Object jobj;
  jobj.readFromFile("../data/json1.json");

  std::cout << jobj.toStringF() << "\n";

  std::cout << jobj.get<int>("Deckung") << "\n";
  std::cout << jobj.getString("Nummer") << "\n";

  return EXIT_SUCCESS;
}