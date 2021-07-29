/**
 * \file utils_gtest.h
 * \brief Some utils for gtest
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_UTILS_TESTING_H
#define PROPERTY_BAG_UTILS_TESTING_H

#include <gtest/gtest.h>


// Macros for testing equalities and inequalities.
//
//    * {ASSERT|EXPECT}_EQ(expected, actual): Tests that expected == actual
//    * {ASSERT|EXPECT}_NE(v1, v2):           Tests that v1 != v2
//    * {ASSERT|EXPECT}_LT(v1, v2):           Tests that v1 < v2
//    * {ASSERT|EXPECT}_LE(v1, v2):           Tests that v1 <= v2
//    * {ASSERT|EXPECT}_GT(v1, v2):           Tests that v1 > v2
//    * {ASSERT|EXPECT}_GE(v1, v2):           Tests that v1 >= v2


// http://stackoverflow.com/a/29155677

namespace testing
{
namespace internal
{
// enum GTestColor
// {
//   COLOR_DEFAULT,
//   COLOR_RED,
//   COLOR_GREEN,
//   COLOR_YELLOW
// };

// extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
/*
// #define PRINTF(...) \
//   do { testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN,\
//   "[          ] "); \
//   testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__); } \
//   while(0)
*/
#define PRINTF(...) printf(__VA_ARGS__)

// C++ stream interface
class TestCout : public std::stringstream
{
public:
  ~TestCout()
  {
    PRINTF("%s\n", str().c_str());
  }
};

#define TEST_COUT testing::internal::TestCout()

} // namespace internal
} // namespace testing

/** Usage :

TEST(Test, Foo)
{
  // the following works but prints default stream
  EXPECT_TRUE(false) << "Testing Stream.";

  // or you can play with AINSI color code
  EXPECT_TRUE(false) << "\033[1;31m" << "Testing Stream.";

  // or use the above defined macros

  PRINTF("Hello world");

  // or

  TEST_COUT << "Hello world";
}

*/

#define UNUSED(x) (void)x

#include <boost/serialization/nvp.hpp>

namespace test
{
struct Dummy
{
  Dummy() :
    a_(1),
    b_(3.14),
    s_{"trololo"} { }

  Dummy(int a, float b, const std::string& s) :
    a_(a),
    b_(b),
    s_{s} { }

  Dummy(const Dummy& o):
    a_(o.a_),
    b_(o.b_),
    s_(o.s_)
  {
    //std::cout << "Dummy copy constructor." << std::endl;
  }

  Dummy(Dummy&& o):
    a_(std::move(o.a_)),
    b_(std::move(o.b_)),
    s_(std::move(o.s_))
  {
    //std::cout << "Dummy move constructor." << std::endl;
  }

  Dummy& operator =(const Dummy& o)
  {
    a_ = o.a_;
    b_ = o.b_;
    s_ = o.s_;

    //std::cout << "Dummy copy assignment." << std::endl;

    return *this;
  }

  Dummy& operator =(Dummy&& o)
  {
    a_ = std::move(o.a_);
    b_ = std::move(o.b_);
    s_ = std::move(o.s_);

    //std::cout << "Dummy move assignment." << std::endl;

    return *this;
  }

  bool operator ==(const Dummy& d) const
  {
    return (a_ == d.a_) & (b_ == d.b_) & (s_ == d.s_);
  }

  int         a_;
  float       b_;
  std::string s_;

protected:

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar & BOOST_SERIALIZATION_NVP(a_);
    ar & BOOST_SERIALIZATION_NVP(b_);
    ar & BOOST_SERIALIZATION_NVP(s_);
  }
};

std::ostream& operator<< (std::ostream& os, const Dummy& ts)
{
  os << "Dummy {" << ts.a_ << "," << ts.b_ << "," << ts.s_ << "}";

  return os;
}
} // namespace test

#endif /* PROPERTY_BAG_UTILS_TESTING_H */
