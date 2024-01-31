#include <cstdlib>
#include <iostream>
#include <fstream>
#include "../inc/Json.hpp"

void test1();
void test2();

int main(void)
{
  test2();

  return EXIT_SUCCESS;
}

struct Data : json::IJsonConvertable
{
  int n = 5;
  float temperature = 2.5;
  std::string name = "Test";

  json::JsonEntity *getNewJsonEntity() const override
  {
    json::Object *ret = new json::Object();

    *ret = json::Object::fromKeyValuePairs("n", n,
                                           "temp", temperature,
                                           "name", name);

    return ret;
  }
};

void test1()
{

  json::FormattingOptions fopt;
  fopt.maxLengthToInline = 0;

  json::Object jobj = json::Object::fromKeyValuePairs("key1", "value1",
                                                      "key1.5", json::Raw("raw"),
                                                      "key2", 42,
                                                      "key3", json::Array::fromValues(1, 2, 3),
                                                      "key4", json::Array::fromString("[1,2,\"lol\"]"),
                                                      "key5", json::Object::fromString("{\"key\": \"val\", \"key2\": 42, \"key3\": 42}"));

  json::Array jarr = json::Array::fromValues("value1",
                                             json::Raw("raw"),
                                             42,
                                             json::Array::fromValues(1, 2, 3),
                                             json::Array::fromString("[1,2,\"lol\"]"),
                                             json::Object::fromString("{\"key\": \"val\", \"key2\": 42, \"key3\": 42}"));

  std::cout << jobj.toStringF(fopt) << "\n";
  std::cout << jarr.toStringF(fopt) << "\n";
}

void test2()
{
  Data d;
}