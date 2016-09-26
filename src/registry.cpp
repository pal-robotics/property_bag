//#include "property_bag/property_serialization.h"
#include "../include/property_bag/property_serialization.h"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(property_bag::details::PlaceHolder)

EXPORT_PROPERTY_NAMED_TYPE(bool, bool)
EXPORT_PROPERTY_NAMED_TYPE(int, int)
EXPORT_PROPERTY_NAMED_TYPE(float, float)
EXPORT_PROPERTY_NAMED_TYPE(double, double)
EXPORT_PROPERTY_NAMED_TYPE(std::string, std__string)
//EXPORT_PROPERTY_NAMED_TYPE(std::vector<bool>, std__vector_bool)
//EXPORT_PROPERTY_NAMED_TYPE(std::vector<int>, std__vector_int)
//EXPORT_PROPERTY_NAMED_TYPE(std::vector<float>, std__vector_float)
//EXPORT_PROPERTY_NAMED_TYPE(std::vector<double>, std__vector_double)
//EXPORT_PROPERTY_NAMED_TYPE(std::vector<std::string>, std__vector_std__string)
