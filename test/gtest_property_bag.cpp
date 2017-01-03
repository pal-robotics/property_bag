#include "utils_gtest.h"
#include <Eigen/Dense>
#include <property_bag/property_bag.h>

EXPORT_PROPERTY_NAMED_TYPE(test::Dummy, test__Dummy)

TEST(PropertyBagTest, PropertyBag)
{
  Eigen::Vector3d eigen_vector(1., 2., 3.);
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_string", std::string("my_string"),
                               "my_vector", eigen_vector};

  bool my_bool(false);
  int  my_int(55);
  std::string my_string = "hellooo";
  Eigen::Vector3d my_vector;

  ASSERT_TRUE(bag.getPropertyValue<bool>("my_bool", my_bool));
  ASSERT_TRUE(my_bool);

  ASSERT_TRUE(bag.getPropertyValue<int>("my_int", my_int));
  ASSERT_EQ(my_int, 5);

  ASSERT_TRUE(bag.getPropertyValue<std::string>("my_string", my_string));
  ASSERT_EQ(my_string, "my_string");


  ASSERT_TRUE(bag.getPropertyValue<Eigen::Vector3d>("my_vector", my_vector));
  ASSERT_EQ(my_vector(0), eigen_vector(0));
  ASSERT_EQ(my_vector(1), eigen_vector(1));
  ASSERT_EQ(my_vector(2), eigen_vector(2));

  bool my_other_bool(false);
  int  my_other_int(55);

  ASSERT_FALSE(bag.getPropertyValue<bool>("my_other_bool", my_other_bool));
  ASSERT_FALSE(my_other_bool);

  ASSERT_FALSE(bag.getPropertyValue<int>("my_other_int", my_other_int));
  ASSERT_NE(my_other_int, 5);

  auto list_properties = bag.listProperties();

  ASSERT_EQ(list_properties, (std::list<std::string>{"my_bool", "my_int", "my_string", "my_vector"}));

  PRINTF("All good at PropertyBagTest::PropertyBag !\n");
}

TEST(PropertyBagTest, PropertyBagThrow)
{
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_dummy", test::Dummy()};

  bool my_bool(false);
  int  my_int(55);
  test::Dummy my_dummy;

  ASSERT_NO_THROW(bag.getPropertyValue<bool>("my_bool", my_bool););
  ASSERT_NO_THROW(bag.getPropertyValue<int>("my_int", my_int));
  ASSERT_NO_THROW(bag.getPropertyValue<test::Dummy>("my_dummy", my_dummy));

  ASSERT_THROW(bag.getPropertyValue<int>("my_bool", my_int),          property_bag::PropertyException);
  ASSERT_THROW(bag.getPropertyValue<test::Dummy>("my_int", my_dummy), property_bag::PropertyException);
  ASSERT_THROW(bag.getPropertyValue<int>("my_dummy", my_int),         property_bag::PropertyException);

  PRINTF("All good at PropertyTest::PropertyBagThrow !\n");
}

TEST(PropertyBagTest, PropertyBagProperty)
{
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_dummy", test::Dummy()};

  ASSERT_NO_THROW(auto p        = bag.getProperty("my_bool"); UNUSED(p));
  ASSERT_NO_THROW(auto& p       = bag.getProperty("my_bool"); UNUSED(p));
  ASSERT_NO_THROW(const auto& p = bag.getProperty("my_bool"); UNUSED(p));

  ASSERT_NO_THROW(auto p        = bag.getProperty("my_int"); UNUSED(p));
  ASSERT_NO_THROW(auto& p       = bag.getProperty("my_int"); UNUSED(p));
  ASSERT_NO_THROW(const auto& p = bag.getProperty("my_int"); UNUSED(p));

  ASSERT_NO_THROW(auto p        = bag.getProperty("my_dummy"); UNUSED(p));
  ASSERT_NO_THROW(auto& p       = bag.getProperty("my_dummy"); UNUSED(p));
  ASSERT_NO_THROW(const auto& p = bag.getProperty("my_dummy"); UNUSED(p));

  PRINTF("All good at PropertyTest::PropertyBagProperty !\n");
}

TEST(PropertyBagTest, PropertyBagProperty2)
{
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_dummy", test::Dummy{2, 6.28, "ok"}};

  const auto& prop_bool  = bag.getProperty("my_bool");
  const auto& prop_int   = bag.getProperty("my_int");
  const auto& prop_dummy = bag.getProperty("my_dummy");

  ASSERT_TRUE(prop_bool.get<bool>());

  ASSERT_EQ(prop_int.get<int>(), 5);
  ASSERT_EQ(prop_dummy.get<test::Dummy>(), test::Dummy(2, 6.28, "ok"));

  PRINTF("All good at PropertyTest::PropertyBagProperty2 !\n");
}

TEST(PropertyBagTest, PropertyBagAddWithDoc)
{
  property_bag::PropertyBag bag;



  bag.addPropertiesWithDoc("my_bool", true, "my_bool_doc",
                           "my_int", 5, "my_int_doc",
                           "my_dummy", test::Dummy{2, 6.28, "ok"}, "my_dummy_doc");

  const auto& prop_bool  = bag.getProperty("my_bool");
  const auto& prop_int   = bag.getProperty("my_int");
  const auto& prop_dummy = bag.getProperty("my_dummy");

  ASSERT_TRUE(prop_bool.get<bool>());

  ASSERT_EQ(prop_int.get<int>(), 5);
  ASSERT_EQ(prop_dummy.get<test::Dummy>(), test::Dummy(2, 6.28, "ok"));

  ASSERT_EQ(prop_bool.description(),  "my_bool_doc");
  ASSERT_EQ(prop_int.description(),   "my_int_doc");
  ASSERT_EQ(prop_dummy.description(), "my_dummy_doc");

  PRINTF("All good at PropertyTest::PropertyBagAddWithDoc !\n");
}

TEST(PropertyBagTest, PropertyBagSerialization)
{

  std::string string;
  {

    std::stringstream ss;

    boost::archive::text_oarchive oa(ss);

    property_bag::PropertyBag bag;

    Eigen::Vector3d eigen_vector(1., 2., 3.);

    bag.addPropertiesWithDoc("my_bool", true, "my_bool_doc",
                             "my_int", 5, "my_int_doc",
                             "my_dummy", test::Dummy{2, 6.28, "ok"}, "my_dummy_doc");
    bag.addProperty("eigen_vector", eigen_vector);

    ASSERT_NO_THROW(oa << bag);

    string = ss.str();
  }

  PRINTF("PropertyBagTest::PropertyBagSerialization Saved !\n");

  std::cerr<<string<<std::endl;
  {

    std::stringstream ss;

    ss<<string;

    boost::archive::text_iarchive ia(ss);

    property_bag::PropertyBag bag;

    ASSERT_NO_THROW(ia >> bag);

    ASSERT_FALSE(bag.empty());
    ASSERT_EQ(bag.size(), 4);

    ASSERT_TRUE(bag.exists("my_bool"));
    ASSERT_TRUE(bag.exists("my_int"));
    ASSERT_TRUE(bag.exists("my_dummy"));

    const auto& prop_bool  = bag.getProperty("my_bool");
    const auto& prop_int   = bag.getProperty("my_int");
    const auto& prop_dummy = bag.getProperty("my_dummy");

    ASSERT_TRUE(prop_bool.get<bool>());

    ASSERT_EQ(prop_int.get<int>(), 5);
    ASSERT_EQ(prop_dummy.get<test::Dummy>(), test::Dummy(2, 6.28, "ok"));

    ASSERT_EQ(prop_bool.description(),  "my_bool_doc");
    ASSERT_EQ(prop_int.description(),   "my_int_doc");
    ASSERT_EQ(prop_dummy.description(), "my_dummy_doc");
  }

  PRINTF("All good at PropertyBagTest::PropertyBagSerialization !\n");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
