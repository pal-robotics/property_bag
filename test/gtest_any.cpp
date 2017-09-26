#include "utils_gtest.h"

#include "property_bag/property.h"

TEST(StuffTest, Stuff)
{
  property_bag::shared_ptr<bool> dummy_ptr;
  ASSERT_TRUE(property_bag::empty(dummy_ptr));

  dummy_ptr = property_bag::make_ptr<bool>(true);
  ASSERT_FALSE(property_bag::empty(dummy_ptr));

  struct A { virtual ~A() = default;};
  struct B : A {};

  property_bag::shared_ptr<A> a_ptr = property_bag::make_ptr<B>();

  ASSERT_FALSE(property_bag::empty(property_bag::dynamic_pointer_cast<B>(a_ptr)));

  property_bag::PropertyException exception("test");

  ASSERT_EQ(exception.message_, "test");
  ASSERT_EQ(std::string(exception.what()), "Property : test\n");

  PRINTF("All good at StuffTest::Stuff !\n");
}

TEST(AnyTest, AnyBool)
{
  property_bag::details::Any any_bool;

  ASSERT_TRUE(any_bool.empty());

  any_bool = true;

  ASSERT_FALSE(any_bool.empty());

  ASSERT_EQ(any_bool.type(), typeid(bool));

  bool retrieved_bool = false;

  ASSERT_NO_THROW(retrieved_bool = property_bag::details::anycast<bool>(any_bool));

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

  ASSERT_NO_THROW(retrieved_int = property_bag::details::anycast<int>(any));

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

  ASSERT_NO_THROW(retrieved_dummy = property_bag::details::anycast<test::Dummy>(any));

  ASSERT_EQ(retrieved_dummy, test::Dummy());

  PRINTF("All good at AnyTest::AnyDummy !\n");
}

TEST(AnyTest, AnyThrow)
{
  property_bag::details::Any any_bool = true;

  ASSERT_THROW(property_bag::details::anycast<int>(any_bool);,
               property_bag::PropertyException);

  const property_bag::details::Any const_any_bool = true;

  ASSERT_THROW(property_bag::details::anycast<int>(const_any_bool);,
               property_bag::PropertyException);

  PRINTF("All good at AnyTest::AnyThrow !\n");
}

TEST(AnyTest, AnyAssignement)
{
  property_bag::details::Any any = true;
  property_bag::details::Any any_bool = true;

  any_bool = false;

  any = any_bool;

  bool retrieved_bool = true;

  ASSERT_NO_THROW(retrieved_bool = property_bag::details::anycast<bool>(any));

  ASSERT_FALSE(retrieved_bool);

  const property_bag::details::Any any_int = 5;

  any = any_int;

  int retrieved_int = 0;

  ASSERT_NO_THROW(retrieved_int = property_bag::details::anycast<int>(any));

  ASSERT_EQ(retrieved_int, 5);

  any = std::string("test");

  std::string retrieved_str;

  ASSERT_NO_THROW(retrieved_str = property_bag::details::anycast<std::string>(any));

  ASSERT_EQ(retrieved_str, "test");

  const property_bag::details::Any any_bool2 = true;

  ASSERT_FALSE(any_bool2.empty());

  any = any_bool2;

  ASSERT_NO_THROW(retrieved_bool = property_bag::details::anycast<bool>(any));

  ASSERT_TRUE(retrieved_bool);

  property_bag::details::Any any_bool3 = false;

  ASSERT_FALSE(any_bool3.empty());

  any = std::move(any_bool3);

  ASSERT_NO_THROW(retrieved_bool = property_bag::details::anycast<bool>(any));

  ASSERT_FALSE(retrieved_bool);

  PRINTF("All good at AnyTest::AnyAssignement !\n");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
