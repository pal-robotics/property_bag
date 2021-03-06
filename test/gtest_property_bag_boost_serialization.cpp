#include "utils_gtest.h"

#include "property_bag/serialization/property_bag_boost_serialization.h"
#include "property_bag/serialization/eigen_boost_serialization.h"

EXPORT_PROPERTY_NAMED_TYPE(test::Dummy, test__Dummy);

TEST(PropertySerializationTest, PropertyBagSerialization)
{
  std::string string;
  {
    std::stringstream ss;

    boost::archive::text_oarchive oa(ss);

    property_bag::PropertyBag bag;

    bag.name("test");

    // non-default value
    bag.setRetrievalHandling(property_bag::RetrievalHandling::THROW);

    Eigen::Vector3d eigen_vector(1., 2., 3.);

    bag.addPropertiesWithDoc("my_bool", true, "my_bool_doc",
                             "my_int", 5, "my_int_doc",
                             "my_string", std::string("this is a string"), "my_string_doc",
                             "my_dummy", test::Dummy{2, 6.28, "ok"}, "my_dummy_doc");
    bag.addProperty("eigen_vector", eigen_vector);

    ASSERT_NO_THROW(oa << bag);

    string = ss.str();
  }

  PRINTF("PropertyBagTest::PropertyBagSerialization Saved !\n");

  {
    std::stringstream ss;

    ss<<string;

    boost::archive::text_iarchive ia(ss);

    property_bag::PropertyBag bag;

    ASSERT_NO_THROW(ia >> bag);

    ASSERT_FALSE(bag.empty());
    ASSERT_EQ(bag.size(), 5);

    ASSERT_EQ(bag.name(), "test");

    ASSERT_EQ(bag.getRetrievalHandling(), property_bag::RetrievalHandling::THROW);

    ASSERT_TRUE(bag.exists("my_bool"));
    ASSERT_TRUE(bag.exists("my_int"));
    ASSERT_TRUE(bag.exists("my_string"));
    ASSERT_TRUE(bag.exists("my_dummy"));

    const auto& prop_bool  = bag.getProperty("my_bool");
    const auto& prop_int   = bag.getProperty("my_int");
    const auto& prop_str   = bag.getProperty("my_string");
    const auto& prop_dummy = bag.getProperty("my_dummy");

    ASSERT_TRUE(prop_bool.get<bool>());

    ASSERT_EQ(prop_int.get<int>(), 5);
    ASSERT_EQ(prop_str.get<std::string>(), "this is a string");
    ASSERT_EQ(prop_dummy.get<test::Dummy>(), test::Dummy(2, 6.28, "ok"));

    ASSERT_EQ(prop_bool.description(),  "my_bool_doc");
    ASSERT_EQ(prop_int.description(),   "my_int_doc");
    ASSERT_EQ(prop_str.description(),   "my_string_doc");
    ASSERT_EQ(prop_dummy.description(), "my_dummy_doc");
  }

  PRINTF("All good at PropertyBagTest::PropertyBagSerialization !\n");
}

TEST(PropertySerializationTest, PropertyBagToStr)
{
  std::string string;
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

  ASSERT_EQ(property_bag::to_str(bag),  string);

  PRINTF("All good at PropertyBagTest::PropertyBagToStr !\n");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
