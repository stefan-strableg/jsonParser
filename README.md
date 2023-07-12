This is a simple implementation of a json parser.

# Features

This project features a wide variety of tools to 
* Read .json files and JSON-strings
* Easily Modify values, objects and arrays
* Create .json files or JSON-strings with highly customizable formatting

## Parsing JSON-strings

Create object from string or set it later:
```cpp
    json::JsonObject object("{"Key": "Value"}");

    object.setString("{"AnotherKey": "AnotherValue"}");
```

Create a unformatted or formatted string from a JsonObject:
```cpp
    // unformatted
    std::cout << object.getString() << "\n";
    
    // unformatted
    std::cout << object << "\n";

    // formatted
    std::cout << object.getStringF() << "\n";
```
## Simple loading from and writing to files

Simply load JSON from a filepath as string:
```cpp
    if (!object.readFromFile("data.json"))
        std::cout << "Failed to load file\n";
```
Or write to a file:
```cpp
    if (!object.writeToFile("data.json"))
        std::cout << "Failed to write to file\n";
```
## Highly customizable JSON-formatting

The output format of getStringF() is customizable via JsonFormattingOptions:
```cpp
    json::JsonObject object;
    object.readFromFile("data.json");

    json::JsonFormattingOptions options;

    options.firstBracketInNewline = true;   
    options.spaceBeforeColon = true; 
    options.inlineShortBottomLevelObjects = false;
    options.maxLengthToInline = 128;

    std::cout << object.getStringF(options) << "\n";
```
The formatting options are described in the documentation. 

## Simple Access and Modification of Values

Simply access (nested) Arrays, Objects and Values:
```cpp
    json::JsonObject object;
    object.readFromFile("data.json");

    object.A("SomeArrayKey").erase(1,3);
    object.O("SomeObjectKey").insert("Key", "\"Value\"");
    object.V("SomeKey") = "\"SomeValue\"";

    object.O("SomeObjectKey").A("SomeNestedArray").push_back(5.f);

    // Convenience for getting the string of objects
    std::cout << object.S("SomeKey")
```
### Getting and Setting

Easily convert the JSON-values to other data types.  
```cpp
    json::JsonObject object;
    object.readFromFile("data.json");

    int i = object.get<int>("SomeNumber");
    float f = object.get<float>("SomeFloatingPointNumber");
    CustomType t = object.get<CustomType>("SomeFloatingPointNumber");

    int j = object.A("SomeArray").get<int>("SomeOtherNumber");
```
Own conversions can be added by overloading the ostream operator.
```cpp
    std::ostream& operator<<(std::ostream&, YourType)
```
