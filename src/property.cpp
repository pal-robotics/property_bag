#include "property_bag/property.h"

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
  return message_.c_str();
}

namespace details
{
Any::Any(Any&& o)
{
  placeholder_ = o.placeholder_;
  o.placeholder_.reset();
}

Any& Any::operator=(Any&& o)
{
  placeholder_ = o.placeholder_;
  o.placeholder_.reset();

  return *this;
}
}

Property::Property() :
  description_(),
  flags_()
{
  set_holder<none>(none{});

  flags_[NONE]=true;
}

Property::Property(const Property& rhs) :
  holder_(rhs.holder_),
  description_(rhs.description_),
  flags_(rhs.flags_)
{
  //
}

Property::Property(Property&& rhs) :
  holder_(std::move(rhs.holder_)),
  description_(std::move(rhs.description_)),
  flags_(std::move(rhs.flags_))
{
  //
}

Property& Property::operator=(const Property& rhs)
{
  if (this == &rhs) return *this;

  holder_      = rhs.holder_;
  description_ = rhs.description_;
  flags_       = rhs.flags_;

  return *this;
}

Property& Property::operator=(Property&& rhs)
{
  if (this == &rhs) return *this;

  holder_      = std::move(rhs.holder_);
  description_ = std::move(rhs.description_);
  flags_       = std::move(rhs.flags_);

  return *this;
}

std::string Property::type_name() const noexcept
{
  return details::name_of(holder_.type());
}

const std::type_info& Property::type() const noexcept
{
  return holder_.type();
}

void Property::description(const std::string& description_str)
{
  description_ = description_str;
}

std::string Property::description() const noexcept
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

} // namespace property_bag
