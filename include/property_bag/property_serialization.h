/**
 * \file property_bag.h
 * \brief Boost serialization for Property & PropertyBag.
 *
 * Define also a helper macro.
 *
 * \see Property
 * \see PropertyBag
 *
 * \author Jeremie Deray
 *  Created on: 26/09/2016
 */

#ifndef PROPERTY_BAG_PROPERTY_SERIALIZATION_H_
#define PROPERTY_BAG_PROPERTY_SERIALIZATION_H_

#pragma once

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/bitset.hpp>
#include <boost/serialization/map.hpp>

#include <boost/serialization/export.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "property_bag/property.h"

#define EXPORT_PROPERTY_NAMED_TYPE(Type, Name) \
  BOOST_CLASS_EXPORT_GUID(property_bag::details::PlaceHolderImpl<Type>, \
    "details_PlaceHolderImpl_"#Name);

/*
#define EXPORT_PROPERTY_TYPE(Type) \
  BOOST_CLASS_EXPORT_GUID(property_bag::details::PlaceHolderImpl<Type>, \
   details::symbolic_char("wolf_core_details_PlaceHolderImpl_"#Type))
*/

#endif /* PROPERTY_BAG_PROPERTY_SERIALIZATION_H_ */
