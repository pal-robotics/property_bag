#include "utils_gtest.h"

#include "property_bag/property.h"

EXPORT_PROPERTY_NAMED_TYPE(test::Dummy, test__Dummy)

TEST(PropertyTest, PropertyDefault)
{
  property_bag::Property property;

  ASSERT_FALSE(property.is_defined());
  ASSERT_FALSE(property.is_default());
  ASSERT_FALSE(property.is_modified());

  ASSERT_EQ(property.type(), typeid(property_bag::Property::none));

  ASSERT_TRUE(property.is_same<property_bag::Property::none>());

//  EXPECT_EQ(property.type_name(), "property_bag::Property::none");
  EXPECT_EQ(property.type_name(), "N12property_bag8Property4noneE");

  ASSERT_EQ(property.description(), "");

  ASSERT_FALSE(property.is_same<bool>());
  ASSERT_FALSE(property.is_same<int>());
  ASSERT_FALSE(property.is_same<float>());
  ASSERT_FALSE(property.is_same<double>());
  ASSERT_FALSE(property.is_same<test::Dummy>());

  ASSERT_TRUE(property.is_compatible<bool>());
  ASSERT_TRUE(property.is_compatible<int>());
  ASSERT_TRUE(property.is_compatible<float>());
  ASSERT_TRUE(property.is_compatible<double>());
  ASSERT_TRUE(property.is_compatible<test::Dummy>());

  ASSERT_NO_THROW(property.enforce_type<property_bag::Property::none>());

  ASSERT_THROW(property.enforce_type<bool>(),   property_bag::PropertyException);
  ASSERT_THROW(property.enforce_type<int>(),    property_bag::PropertyException);
  ASSERT_THROW(property.enforce_type<float>(),  property_bag::PropertyException);
  ASSERT_THROW(property.enforce_type<double>(), property_bag::PropertyException);
  ASSERT_THROW(property.enforce_type<test::Dummy>(), property_bag::PropertyException);

  ASSERT_NO_THROW(auto& my_none = property.get<property_bag::Property::none>(); (void)my_none;);
  ASSERT_NO_THROW(const auto& my_none = property.get<property_bag::Property::none>(); (void)my_none;);

  ASSERT_THROW(property.get<int>(), property_bag::PropertyException);

  ASSERT_NO_THROW(property.set(5));
  ASSERT_NO_THROW(property.get<int>());

  ASSERT_FALSE(property.is_same<property_bag::Property::none>());
  ASSERT_TRUE(property.is_same<int>());

  ASSERT_THROW(property.enforce_type<property_bag::Property::none>(),
               property_bag::PropertyException);

  ASSERT_NO_THROW(property.enforce_type<int>());

//  EXPECT_EQ(property.type_name(), "int");
  EXPECT_EQ(property.type_name(), "i");

  ASSERT_EQ(property.get<int>(), 5);

  ASSERT_TRUE(property.is_defined());
  ASSERT_TRUE(property.is_default());
  ASSERT_FALSE(property.is_modified());

  ASSERT_NO_THROW(property.set(6));

  ASSERT_EQ(property.get<int>(), 6);

  ASSERT_TRUE(property.is_defined());
  ASSERT_FALSE(property.is_default());
  ASSERT_TRUE(property.is_modified());

  PRINTF("All good at PropertyTest::PropertyDefault !\n");
}

TEST(PropertyTest, PropertyBool)
{
  property_bag::Property property_bool(true, "my_bool");

  ASSERT_TRUE(property_bool.is_same<bool>());

  ASSERT_TRUE(property_bool.is_defined());
  ASSERT_TRUE(property_bool.is_default());
  ASSERT_FALSE(property_bool.is_modified());

  ASSERT_EQ(property_bool.description(), "my_bool");

  ASSERT_TRUE(property_bool.is_compatible<bool>());
  ASSERT_FALSE(property_bool.is_compatible<int>());

  ASSERT_NO_THROW(property_bool.enforce_type<bool>());
  ASSERT_THROW(property_bool.enforce_type<int>(), property_bag::PropertyException);

  ASSERT_NO_THROW(auto& my_bool = property_bool.get<bool>(); UNUSED(my_bool););

  ASSERT_THROW(property_bool.get<int>(), property_bag::PropertyException);

  ASSERT_EQ(property_bool.get<bool>(), true);

  ASSERT_NO_THROW(property_bool.set(false));

  ASSERT_EQ(property_bool.get<bool>(), false);

  ASSERT_TRUE(property_bool.is_defined());
  ASSERT_FALSE(property_bool.is_default());
  ASSERT_TRUE(property_bool.is_modified());

  property_bag::Property property_int(1, "my_int");

  ASSERT_FALSE(property_bool.is_same(property_int));
  ASSERT_FALSE(property_bool.is_compatible(property_int));

  ASSERT_NO_THROW(property_bool = property_int);

  PRINTF("All good at PropertyTest::PropertyBool !\n");

//  Property property_float(1.1f, "my_float");
//  Property property_double(double(1.2), "my_double");
//  Property property_dummy(test::Dummy(), "my_dummy");
}

TEST(PropertyTest, PropertySerialization)
{
  std::stringstream ss;

  {
    boost::archive::text_oarchive oa(ss);

    property_bag::Property property(test::Dummy(2, 6.28, "ok"), "my_dummy_description");
    ASSERT_NO_THROW(oa << property);

    property_bag::Property property_int(5, "my_int_description");
    ASSERT_NO_THROW(oa << property_int);
  }

  PRINTF("PropertyTest::PropertySerializationBin Saved !\n");

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
