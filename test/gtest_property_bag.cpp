#include "utils_gtest.h"
#include <Eigen/Dense>
#include <property_bag/property_bag.h>

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

TEST(PropertyBagTest, PropertyBagDefaultValue)
{
  property_bag::PropertyBag bag;

  bool my_bool(false);
  int  my_int(55);
  test::Dummy my_dummy;

  ASSERT_NO_THROW(bag.getPropertyValue<bool>("my_bool", my_bool, true););
  ASSERT_NO_THROW(bag.getPropertyValue<int>("my_int", my_int, 15));
  ASSERT_NO_THROW(bag.getPropertyValue<test::Dummy>("my_dummy", my_dummy, test::Dummy{2, 6.28, "ok"}));

  ASSERT_TRUE(my_bool);
  ASSERT_EQ(my_int, 15);
  ASSERT_EQ(my_dummy, test::Dummy(2, 6.28, "ok"));

  PRINTF("All good at PropertyTest::PropertyBagDefaultValue !\n");
}

TEST(PropertyBagTest, PropertyBagExist)
{
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_dummy", test::Dummy()};

  ASSERT_TRUE(bag.exists("my_bool"));
  ASSERT_TRUE(bag.exists("my_int"));
  ASSERT_TRUE(bag.exists("my_dummy"));

  ASSERT_FALSE(bag.exists("my_other"));

  PRINTF("All good at PropertyTest::PropertyBagExist !\n");
}

TEST(PropertyBagTest, PropertyBagSize)
{
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_dummy", test::Dummy()};

  ASSERT_EQ(bag.size(), 3);

  bag.addProperty("my_other_int", 5);

  ASSERT_EQ(bag.size(), 4);

  ASSERT_TRUE(bag.removeProperty("my_other_int"));
  ASSERT_FALSE(bag.removeProperty("my_other_int"));

  ASSERT_EQ(bag.size(), 3);

  PRINTF("All good at PropertyTest::PropertyBagSize !\n");
}

TEST(PropertyBagTest, PropertyBagPropertyHandling)
{
  property_bag::PropertyBag bag;

  ASSERT_EQ(bag.getRetrievalHandling(), property_bag::RetrievalHandling::QUIET);

  bag.setRetrievalHandling(property_bag::RetrievalHandling::THROW);

  ASSERT_EQ(bag.getRetrievalHandling(), property_bag::RetrievalHandling::THROW);

  PRINTF("All good at PropertyTest::PropertyBagPropertyHandling !\n");
}

TEST(PropertyBagTest, PropertyBagThrow)
{
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_dummy", test::Dummy()};

  bool my_bool(false);
  int  my_int(55);
  test::Dummy my_dummy;

  bool retrieved = true;

  ASSERT_NO_THROW(retrieved &= bag.getPropertyValue<bool>("my_bool", my_bool););
  ASSERT_NO_THROW(retrieved &= bag.getPropertyValue<int>("my_int", my_int));
  ASSERT_NO_THROW(retrieved &= bag.getPropertyValue<test::Dummy>("my_dummy", my_dummy));

  ASSERT_TRUE(retrieved);

  retrieved = false;

  const property_bag::RetrievalHandling QUIET = property_bag::RetrievalHandling::QUIET;
  const property_bag::RetrievalHandling THROW = property_bag::RetrievalHandling::THROW;

  ASSERT_NO_THROW(retrieved |= bag.getPropertyValue<int>("my_bool", my_int,          QUIET));
  ASSERT_NO_THROW(retrieved |= bag.getPropertyValue<test::Dummy>("my_int", my_dummy, QUIET));
  ASSERT_NO_THROW(retrieved |= bag.getPropertyValue<int>("my_dummy", my_int,         QUIET));

  ASSERT_FALSE(retrieved);

  ASSERT_THROW(bag.getPropertyValue<bool>("my_other_bool", my_bool, THROW),  property_bag::PropertyException);

  ASSERT_THROW(bag.getPropertyValue<int>("my_bool", my_int, THROW),          property_bag::PropertyException);
  ASSERT_THROW(bag.getPropertyValue<test::Dummy>("my_int", my_dummy, THROW), property_bag::PropertyException);
  ASSERT_THROW(bag.getPropertyValue<int>("my_dummy", my_int, THROW),         property_bag::PropertyException);

  bag.setRetrievalHandling(property_bag::RetrievalHandling::THROW);

  ASSERT_THROW(bag.getPropertyValue<int>("my_bool", my_int),          property_bag::PropertyException);
  ASSERT_THROW(bag.getPropertyValue<test::Dummy>("my_int", my_dummy), property_bag::PropertyException);
  ASSERT_THROW(bag.getPropertyValue<int>("my_dummy", my_int),         property_bag::PropertyException);

  ASSERT_NO_THROW(retrieved |= bag.getPropertyValue<int>("my_bool", my_int, QUIET));
  ASSERT_NO_THROW(retrieved |= bag.getPropertyValue<test::Dummy>("my_int", my_dummy, QUIET));
  ASSERT_NO_THROW(retrieved |= bag.getPropertyValue<int>("my_dummy", my_int, QUIET));

  ASSERT_FALSE(retrieved);

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

TEST(PropertyBagTest, PropertyBagConstructorDoc)
{
  property_bag::PropertyBag bag{property_bag::PropertyBag::WithDoc{},
                                "my_bool", true, "my_bool_doc",
                                "my_int", 5, "my_int_doc",
                                "my_dummy", test::Dummy{2, 6.28, "ok"}, "my_dummy_doc"};

  const auto& prop_bool  = bag.getProperty("my_bool");
  const auto& prop_int   = bag.getProperty("my_int");
  const auto& prop_dummy = bag.getProperty("my_dummy");

  ASSERT_TRUE(prop_bool.get<bool>());

  ASSERT_EQ(prop_int.get<int>(), 5);
  ASSERT_EQ(prop_dummy.get<test::Dummy>(), test::Dummy(2, 6.28, "ok"));

  ASSERT_EQ(prop_bool.description(),  "my_bool_doc");
  ASSERT_EQ(prop_int.description(),   "my_int_doc");
  ASSERT_EQ(prop_dummy.description(), "my_dummy_doc");

  PRINTF("All good at PropertyTest::PropertyBagConstructorDoc !\n");
}

TEST(PropertyBagTest, PropertyPropertyBag)
{
  property_bag::Property property{
    property_bag::PropertyBag{property_bag::PropertyBag::WithDoc{},
                              "my_bool", true, "my_bool_doc",
                              "my_int", 5, "my_int_doc",
                              "my_dummy", test::Dummy{2, 6.28, "ok"}, "my_dummy_doc"},
      "my_prop_bag"};

  ASSERT_TRUE(property.is_same<property_bag::PropertyBag>());

  ASSERT_TRUE(property.is_defined());
  ASSERT_TRUE(property.is_default());
  ASSERT_FALSE(property.is_modified());

  ASSERT_EQ(property.description(), "my_prop_bag");

  ASSERT_TRUE(property.is_compatible<property_bag::PropertyBag>());
  ASSERT_FALSE(property.is_compatible<int>());

  ASSERT_NO_THROW(property.enforce_type<property_bag::PropertyBag>());
  ASSERT_THROW(property.enforce_type<int>(), property_bag::PropertyException);

  const property_bag::PropertyBag* my_bag = nullptr;
  ASSERT_NO_THROW(my_bag = &property.get<property_bag::PropertyBag>(); );

  ASSERT_FALSE(my_bag == nullptr);

  if (my_bag != nullptr)
  {
    const auto& prop_bool  = my_bag->getProperty("my_bool");
    const auto& prop_int   = my_bag->getProperty("my_int");
    const auto& prop_dummy = my_bag->getProperty("my_dummy");

    ASSERT_TRUE(prop_bool.get<bool>());

    ASSERT_EQ(prop_int.get<int>(), 5);
    ASSERT_EQ(prop_dummy.get<test::Dummy>(), test::Dummy(2, 6.28, "ok"));

    ASSERT_EQ(prop_bool.description(),  "my_bool_doc");
    ASSERT_EQ(prop_int.description(),   "my_int_doc");
    ASSERT_EQ(prop_dummy.description(), "my_dummy_doc");
  }

  ASSERT_THROW(property.get<int>(), property_bag::PropertyException);

  ASSERT_NO_THROW(property.set(property_bag::PropertyBag{"my_bool", true}));

//  ASSERT_EQ(property.get<property_bag::PropertyBag>(), property_bag::PropertyBag{"my_bool", true});

  ASSERT_TRUE(property.is_defined());
  ASSERT_FALSE(property.is_default());
  ASSERT_TRUE(property.is_modified());

  property_bag::Property property_int(1, "my_int");

  ASSERT_FALSE(property.is_same(property_int));
  ASSERT_FALSE(property.is_compatible(property_int));

  ASSERT_NO_THROW(property = property_int);

  PRINTF("All good at PropertyTest::PropertyPropertyBag !\n");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
