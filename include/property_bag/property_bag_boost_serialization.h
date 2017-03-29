/**
 * \file property.h
 * \brief Generic Property holding any type.
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_BOOST_SERIALIZATION_PROPERTY_BAG_H
#define PROPERTY_BAG_BOOST_SERIALIZATION_PROPERTY_BAG_H

#include "property_bag/property_boost_serialization.h"
#include "property_bag/property_bag.h"

#include <boost/serialization/map.hpp>

namespace property_bag {

struct PropertyBag::serialization_accessor
{
  template <class Archive>
  static void serialize(
      Archive &ar,
      PropertyBag &property_bag,
      const unsigned int /*file_version*/)
  {
    ar & BOOST_SERIALIZATION_NVP(property_bag.properties_);
  }
};

} /* namespace property_bag */

namespace boost {
namespace serialization {

template<class Archive>
void serialize(
    Archive &ar,
    property_bag::PropertyBag &property_bag,
    const unsigned int file_version)
{
  property_bag::PropertyBag::serialization_accessor::serialize(ar, property_bag, file_version);
}

} //namespace serialization
} //namespace boost

#include <boost/archive/text_oarchive.hpp>

namespace property_bag {

std::string to_str(const property_bag::PropertyBag &property_bag)
{
  std::stringstream ss;
  boost::archive::text_oarchive oa(ss);
  oa << property_bag;
  return ss.str();
}

} /* namespace property_bag */

#endif /* PROPERTY_BAG_BOOST_SERIALIZATION_PROPERTY_BAG_H */
