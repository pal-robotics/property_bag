#include "property_bag/property.h"
#include <property_bag/eigen_boost_serialization.h>

namespace property_bag
{

PropertyException::PropertyException(const std::string &message) :
  std::exception(),
  message_(message)
{
  //
}

const char* PropertyException::what() const throw()
{
  std::stringstream ss;
  ss << "Property : " << message_ << std::endl;

  return ss.str().c_str();
}

Property::Property() :
  description_(),
  flags_()
{
  set_holder<none>(none());

  flags_[NONE]=true;
}

Property::Property(const Property& rhs) :
  holder_(rhs.holder_),
  description_(rhs.description_),
  flags_(rhs.flags_)
{
  //
}

Property& Property::operator=(const Property& rhs)
{
  if (this == &rhs) return *this;

  copy_holder(rhs);
  description_ = rhs.description_;
  flags_       = rhs.flags_;

  return *this;
}

const std::string Property::type_name() const
{
  //return details::name_of(holder_.type());
  return holder_.type().name();
}

const std::type_info& Property::type() const
{
  return holder_.type();
}

void Property::description(const std::string& description_str)
{
  description_ = description_str;
}

std::string Property::description() const
{
  return description_;
}

bool Property::is_same(const Property& rhs) const
{
  return rhs.type() == type();
}

bool Property::is_compatible(const Property& rhs) const
{
  if (is_same(rhs)) return true;

  return is_same<none>() || rhs.is_same<none>();
}

void Property::copy_holder(const Property& rhs)
{
  holder_ = rhs.holder_;
}

} // namespace property_bag

// Specific boost specializations for Eigen
EXPORT_PROPERTY_NAMED_TYPE(Eigen::Vector3d, eigen_vector3)
EXPORT_PROPERTY_NAMED_TYPE(Eigen::VectorXd, eigen_vectorxd)
EXPORT_PROPERTY_NAMED_TYPE(Eigen::Quaterniond, eigen_quaterniond)
