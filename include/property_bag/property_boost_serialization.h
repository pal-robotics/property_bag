/**
 * \file property.h
 * \brief Generic Property holding any type.
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_BOOST_SERIALIZATION_PROPERTY_H
#define PROPERTY_BAG_BOOST_SERIALIZATION_PROPERTY_H

#include "property_bag/property.h"

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/bitset.hpp>

#include <boost/serialization/export.hpp>

#define EXPORT_PROPERTY_NAMED_TYPE(Type, Name) \
  BOOST_CLASS_EXPORT_GUID(property_bag::details::PlaceHolderImpl<Type>, \
    "details_PlaceHolderImpl_"#Name);

BOOST_SERIALIZATION_ASSUME_ABSTRACT(property_bag::details::PlaceHolder);

namespace property_bag {
namespace details {

template <typename T>
struct PlaceHolderImpl<T>::serialization_accessor
{
  template <class Archive>
  static void serialize(
      Archive &ar,
      property_bag::details::PlaceHolderImpl<T> &p,
      const unsigned int /*file_version*/)
  {
    ar & boost::serialization::make_nvp("property_bag::details::PlaceHolder",
              boost::serialization::base_object<property_bag::details::PlaceHolder>(p));

    ar & BOOST_SERIALIZATION_NVP(p.value_);
  }
};

struct Any::serialization_accessor
{
  template <class Archive>
  static void serialize(
      Archive &ar,
      Any &any,
      const unsigned int /*file_version*/)
  {
    ar & BOOST_SERIALIZATION_NVP(any.placeholder_);
  }
};

} /* namespace details */

struct Property::serialization_accessor
{
  template <class Archive>
  static void serialize(
      Archive &ar,
      Property &property,
      const unsigned int /*file_version*/)
  {
    ar & BOOST_SERIALIZATION_NVP(property.holder_);
    ar & BOOST_SERIALIZATION_NVP(property.description_);
    ar & BOOST_SERIALIZATION_NVP(property.flags_);
  }
};

} /* namespace property_bag */

namespace boost {
namespace serialization {

// The only necessary thing to do is to register this
// abstract class and define an empty serialize function.
template<class Archive>
inline void serialize(
    Archive &/*ar*/,
    property_bag::details::PlaceHolder &/*p*/,
    const unsigned int /*file_version*/)
{
  //
}

template <class Archive, typename T>
void serialize(
    Archive &ar,
    property_bag::details::PlaceHolderImpl<T> &pl,
    const unsigned int file_version)
{
  property_bag::details::PlaceHolderImpl<T>::serialization_accessor::serialize(ar, pl, file_version);
}

template<class Archive>
void serialize(
    Archive &ar,
    property_bag::details::Any &any,
    const unsigned int file_version)
{
  property_bag::details::Any::serialization_accessor::serialize(ar, any, file_version);
}

template<class Archive>
void serialize(
    Archive &ar,
    property_bag::Property &property,
    const unsigned int file_version)
{
  property_bag::Property::serialization_accessor::serialize(ar, property, file_version);
}

} //namespace serialization
} //namespace boost

EXPORT_PROPERTY_NAMED_TYPE(bool, bool)
EXPORT_PROPERTY_NAMED_TYPE(int, int)
EXPORT_PROPERTY_NAMED_TYPE(float, float)
EXPORT_PROPERTY_NAMED_TYPE(double, double)
EXPORT_PROPERTY_NAMED_TYPE(std::string, std__string)
/*
EXPORT_PROPERTY_NAMED_TYPE(std::vector<bool>, std__vector_bool)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<int>, std__vector_int)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<float>, std__vector_float)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<double>, std__vector_double)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<std::string>, std__vector_std__string)
*/

//#include <property_bag/eigen_boost_serialization.h>
// Specific boost specializations for Eigen
//EXPORT_PROPERTY_NAMED_TYPE(Eigen::Vector3d, eigen_vector3)
//EXPORT_PROPERTY_NAMED_TYPE(Eigen::VectorXd, eigen_vectorxd)
//EXPORT_PROPERTY_NAMED_TYPE(Eigen::Quaterniond, eigen_quaterniond)

#endif /* PROPERTY_BAG_BOOST_SERIALIZATION_PROPERTY_H */
