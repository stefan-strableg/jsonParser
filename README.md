This is a simple implementation of a json parser.

Use doxygen to create the documentation.

# Features

- Pretty formatting of JSON with plenty of options
- Easy-to-use interfaces
- Reading from Files
- Writing to files
- An Interface you can implement to automatically convert your Classes to JSON

## Creating JSON

Create object from string, from values or set it later:

```cpp
    using namespace json;

    Object a = Object::fromString("{\"key\":\"value\"}");

    Object b = Object::fromKeyValuePairs("key1", "value",
                                              "key2", 5,
                                              "key3", 2.4);
    Object c;
    c.loadString("{\"key\":\"value\"}");


    Array d = Array::fromString("{1,2,3}");

    Array e = Array::fromValues("value", 5, 2.4);

    Array f;
    f.loadString("{1,2,3}");
```

## Simple Access and Modification of Values

Its easy to read values into specific Objects:

```cpp
    int i = object.get<int>("key");
    float i = object.get<float>("key");

    std::string str = object.get<std::string>("key");
    // Or simply:
    str = object.S("SomeKey");
```

As well as to modify them:

```cpp
    object.V("key1") = 2;
    object.V("key2") = 2;
    object.V("key3") = "Hello World!";
    object.V("key4") = json::Array::fromValues(1,'a',"nice");;

    object.remove("key2");

    array.V(2) = 1;
    array.push_back("Hello World!");
    array.remove(4,3); // Removes 3 elements starting with the 4th one
```

You can very easily access nested **A**rrays, **O**bjects and **V**alues per Reference:

```cpp
    json::Object object;
    object.readFromFile("data.json");

    object.A("key1").erase(1,3);
    object.O("key2").insert("keyX", "value");
    object.V("key3") = "SomeValue";

    object.O("keyA").A("keyB").push_back(5.f);
```

You can make your classes convertible to JSON by implementing one of these Methods:

```cpp
    json::Object* YourClass::toJson() const;
    // or
    json::Array* YourClass::toJson() const;
```

You can make your classes convertible from JSON by implementing one of these Methods:

```cpp
    static YourClass YourClass::fromJsonObject(const json::Object&);
    // or
    static YourClass YourClass::fromJsonArray(const json::Array&);
```

## Simple loading from and writing to files

Simply load JSON from a file:

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

Create a unformatted or formatted string from an Object or Array:

```cpp
    json::Object object;
    // unformatted
    std::cout << object.getString() << '\n';

    // unformatted
    std::cout << object << '\n';

    // formatted
    std::cout << object.getStringF() << '\n';
```

The output format of getStringF() is customizable via JsonFormattingOptions:

```cpp
    using namespace json;

    Object object;
    object.readFromFile("data.json");

    FormattingOptions options;

    options.firstBracketInNewline = true;
    options.spaceBeforeColon = true;
    options.inlineShortBottomLevelObjects = false;
    options.maxLengthToInline = 128;

    std::cout << object.getStringF(options) << "\n";
```

The formatting options are described in the documentation.

You can also format when writing to a file:

```cpp
    object.writeToFile("data.json", formattingOptions);
```
