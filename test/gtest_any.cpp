#include "utils_gtest.h"

#include "property_bag/property.h"

TEST(AnyTest, AnyBool)
{
  property_bag::details::Any any_bool;

  ASSERT_TRUE(any_bool.empty());

  any_bool = true;

  ASSERT_FALSE(any_bool.empty());

  ASSERT_EQ(any_bool.type(), typeid(bool));

  bool retrieved_bool = false;

  retrieved_bool = property_bag::details::anycast<bool>(any_bool);

  ASSERT_EQ(retrieved_bool, true);

  PRINTF("All good at AnyTest::AnyBool !\n");
}

TEST(AnyTest, AnyInt)
{
  property_bag::details::Any any;

  ASSERT_TRUE(any.empty());

  any = 5;

  ASSERT_FALSE(any.empty());

  ASSERT_EQ(any.type(), typeid(int));

  int retrieved_int = 55;

  retrieved_int = property_bag::details::anycast<int>(any);

  ASSERT_EQ(retrieved_int, 5);

  PRINTF("All good at AnyTest::AnyInt !\n");
}

TEST(AnyTest, AnyDummy)
{
  property_bag::details::Any any;

  ASSERT_TRUE(any.empty());

  any = test::Dummy();

  ASSERT_FALSE(any.empty());

  ASSERT_EQ(any.type(), typeid(test::Dummy));

  test::Dummy retrieved_dummy(55, 55.5, "fifty-five");

  retrieved_dummy = property_bag::details::anycast<test::Dummy>(any);

  ASSERT_EQ(retrieved_dummy, test::Dummy());

  PRINTF("All good at AnyTest::AnyDummy !\n");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
