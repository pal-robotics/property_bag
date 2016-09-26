# property_bag

##### A generic container based on type-erasure that can thus hold variables of different types.

A property is a type-erasure class that can hold a variable of any type.
In a PropertyBag, the property is associated to a `std::string` that serves as a key and another (optional) `std::string` that describes the property.

Usage :
* `PropertyBag` initialization :
    * With default constructor :

        ```c++
        property_bag::PropertyBag bag;
        // Add a single Property.
        bag.addProperty("my_float", 5.45f, "Level of stuff");
        DummyMaker dummy; // Custom classes can be held too !
        bag.addProperty("my_dummy", dummy, "A dummy class to make a point.");
        // Adding several properties at once.
        bag.addProperties("my_bool", true, "my_int", 5);
        // Adding several properties with their descriptions at once.
        bag.addPropertiesWithDoc("my_other_bool", true, "Whether to use this cool feature or not.",
                                 "my_string", "camera left", "The sensor name");
        ```
    * With properties instantiation constructor :

        ```c++
        // Create a bag with two properties.
        property_bag::PropertyBag bag{"my_bool", true, "my_int", 5};
        ```

* You can then retrieve properties from a `PropertyBag` :
    ```c++
    /*const*/ auto& prop_bool = bag.getProperty("my_bool");
    assert_true(prop_bool.get<bool>());
    const std::string& my_bool_description = prop_bool.description();
    ```

* Or you can retrieve directly the value held by the property :
    ```c++
    int a_int(41);
    bag.getPropertyValue<int>("my_int", a_int);
    asser_true(a_int == 5);
    ```
    
* And of course you can delete properties :
    ```c++
    bag.removeProperty("my_string"); // returns true/false.
    ```

* Notice that properties can not be overwriten ! 
    ```c++
    bag.addProperty("circle_radius", 2.5f, "Circle radius"); // return true
    bag.addProperty("circle_radius", 2.5f, "Circle radius"); // return false, the `Property` already exists
    ```
    In order to update a `Property` value you can either :
    * retrieve the `Property` and update its value :
        ```c++
        auto& prop_bool = bag.getProperty("my_bool");
        prop_bool.set(false);
        ```
    * or use the update value function :
        ```c++
        bag.updateProperty("my_bool", false);
        ```
        
* Finally `PropertyBag` can be serialized using `boost serialization` :
    ```c++
    std::stringstream ss;
    {
      boost::archive::text_oarchive oa(ss);
      property_bag::PropertyBag bag;
      bag.addPropertiesWithDoc("my_bool", true, "my_bool_doc",
                               "my_int", 5, "my_int_doc",
                               "my_dummy", test::Dummy{2, 6.28, "ok"}, "my_dummy_doc");
      oa << bag;
    }
    // Brackets ensure that archives dies ...
    {
      boost::archive::text_iarchive ia(ss);
      property_bag::PropertyBag bag;
      ia >> bag;
      ...
    }
    ```
    Beware that to serialize a custom class, the custom class must define the `serialize(...)` function.
    Moreover you need to export it for the boost archive to be abble to deserialize it proprely. To do so you can use the following macro:
    ```c++
    EXPORT_PROPERTY_NAMED_TYPE(test_namespace::DummyMaker, test_namespace__DummyMaker);
    ```
    See [Boost Serialization Doc](http://www.boost.org/doc/libs/1_61_0/libs/serialization/doc/) for more info.

* Some other cool features include :
    * Check if the bag holds anything :
        ```c++
        bag.empty(); // returns true/false
        ```
    * Get how many properties the bag contains :
        ```c++
        bag.size(); // returns size_t, the number of properties held.
        ```
    * `Property` existence checking :
        ```c++
        bag.exists("my_bool"); // returns true/false.
        ```
    * Get a list of properties :
        ```c++
        std::list<std::string> properties_name = bag.listProperties();
        ```

* Todo : details `Property` class. It has some cool features too you know.