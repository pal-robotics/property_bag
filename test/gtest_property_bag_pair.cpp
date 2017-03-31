#include "utils_gtest.h"

#include "property_bag/property_bag.h"

TEST(PropertyTest, ProperStdPair)
{
  property_bag::PropertyBag bag{"my_bool", true, "my_int", 5, "my_string", std::string("my_string")};
  std::pair<int, property_bag::PropertyBag> pair;

  std::cout << "MAKE PAIR" << std::endl;

  pair = std::make_pair(0, bag);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
