/**
 * \file property_bag.hpp
 * \brief Property container holding many different type.
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_PROPERTY_BAG_HPP
#define PROPERTY_BAG_PROPERTY_BAG_HPP

#include <property_bag/property_bag.h>

namespace property_bag
{
template<typename KeyType>
AbstractPropertyBag<KeyType>::AbstractPropertyBag(const AbstractPropertyBag<KeyType>& rhs) :
  default_handling_(rhs.default_handling_),
  properties_(rhs.properties_)
{
  //
}

template<typename KeyType>
AbstractPropertyBag<KeyType>::AbstractPropertyBag(AbstractPropertyBag<KeyType>&& rhs) :
  default_handling_(rhs.default_handling_),
  properties_(std::move(rhs.properties_))
{
  //
}

template<typename KeyType>
AbstractPropertyBag<KeyType>& AbstractPropertyBag<KeyType>::operator=(const AbstractPropertyBag<KeyType>& rhs)
{
  default_handling_ = rhs.default_handling_;
  this->properties_ = rhs.properties_;
  return *this;
}

template<typename KeyType>
AbstractPropertyBag<KeyType>& AbstractPropertyBag<KeyType>::operator=(AbstractPropertyBag<KeyType>&& rhs)
{
  default_handling_ = rhs.default_handling_;
  this->properties_ = std::move(rhs.properties_);
  return *this;
}

template<typename KeyType>
void AbstractPropertyBag<KeyType>::addProperties()
{
  /* End of parameter pack expansion */
}

template<typename KeyType>
void AbstractPropertyBag<KeyType>::addPropertiesWithDoc()
{
  /* End of parameter pack expansion */
}

template<typename KeyType>
Property& AbstractPropertyBag<KeyType>::getProperty(const KeyType &name)
{
  auto it = properties_.find(name);
  return (it != properties_.end())? it->second : none_;
}

template<typename KeyType>
const Property& AbstractPropertyBag<KeyType>::getProperty(const KeyType &name) const
{
  auto it = properties_.find(name);
  return (it != properties_.end())? it->second : none_;
}

template<typename KeyType>
bool AbstractPropertyBag<KeyType>::removeProperty(const KeyType &name)
{
  return (bool)properties_.erase(name);
}

template<typename KeyType>
std::list<KeyType> AbstractPropertyBag<KeyType>::listProperties() const
{
  std::list<KeyType> list;

  for (const auto& p : properties_) list.emplace_back(p.first);

  return list;
}

template<typename KeyType>
bool AbstractPropertyBag<KeyType>::exists(const KeyType& name) const
{
  return properties_.find(name) != properties_.end();
}

template<typename KeyType>
typename AbstractPropertyBag<KeyType>::iterator AbstractPropertyBag<KeyType>::begin()
{
  return properties_.begin();
}

template<typename KeyType>
typename AbstractPropertyBag<KeyType>::const_iterator AbstractPropertyBag<KeyType>::begin() const
{
  return properties_.begin();
}

template<typename KeyType>
typename AbstractPropertyBag<KeyType>::iterator AbstractPropertyBag<KeyType>::end()
{
  return properties_.end();
}

template<typename KeyType>
typename AbstractPropertyBag<KeyType>::const_iterator AbstractPropertyBag<KeyType>::end() const
{
  return properties_.end();
}

} // namespace property_bag

#endif /* PROPERTY_BAG_PROPERTY_BAG_HPP */
