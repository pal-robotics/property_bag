#include "utils_gtest.h"

#include "property_bag/property_boost_serialization.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

EXPORT_PROPERTY_NAMED_TYPE(test::Dummy, test__Dummy);

TEST(PropertySerializationTest, PropertyBoostSerialization)
{
  std::stringstream ss;

  {
    boost::archive::text_oarchive oa(ss);

    property_bag::Property property(test::Dummy(2, 6.28, "ok"), "my_dummy_description");
    ASSERT_NO_THROW(oa << property);

    property_bag::Property property_int(5, "my_int_description");
    ASSERT_NO_THROW(oa << property_int);
  }

  PRINTF("PropertyTest::PropertyBoostSerialization Saved !\n");

  {
    boost::archive::text_iarchive ia(ss);

    property_bag::Property property;
    ASSERT_NO_THROW(ia >> property);
    ASSERT_EQ(property.get<test::Dummy>(), test::Dummy(2, 6.28, "ok"));
    ASSERT_EQ(property.description(), "my_dummy_description");

    property_bag::Property property_int;
    ASSERT_NO_THROW(ia >> property_int);
    ASSERT_EQ(property_int.get<int>(), 5);
    ASSERT_EQ(property_int.description(), "my_int_description");
  }

  PRINTF("All good at PropertyTest::PropertySerializationBin !\n");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
