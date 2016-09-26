#include "property_bag/property_bag.h"

#include <map>

namespace property_bag
{

PropertyBag::PropertyBag(const PropertyBag& rhs) :
  properties_(rhs.properties_)
{
  //
}

PropertyBag& PropertyBag::operator=(const PropertyBag& rhs)
{
  this->properties_ = rhs.properties_;
  return *this;
}

void PropertyBag::addProperties()
{
  /* End of parameter pack expansion */
}

void PropertyBag::addPropertiesWithDoc()
{
  /* End of parameter pack expansion */
}

Property& PropertyBag::getProperty(const std::string &name)
{
  auto it = properties_.find(name);
  return (it != properties_.end())? it->second : none_;
}

const Property& PropertyBag::getProperty(const std::string &name) const
{
  auto it = properties_.find(name);
  return (it != properties_.end())? it->second : none_;
}

bool PropertyBag::removeProperty(const std::string &name)
{
  return (bool)properties_.erase(name);
}

std::list<std::string> PropertyBag::listProperties() const
{
  std::list<std::string> list;

  for (const auto& p : properties_) list.emplace_back(p.first);

  return list;
}

bool PropertyBag::exists(const std::string& name)
{
  return properties_.find(name) != properties_.end();
}

} // namespace property_bag
