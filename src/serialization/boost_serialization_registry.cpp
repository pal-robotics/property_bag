#include <property_bag/serialization/property_boost_serialization.h>
#include <property_bag/serialization/property_bag_boost_serialization.h>
#include <boost/serialization/vector.hpp>

EXPORT_PROPERTY_NAMED_TYPE(bool, bool)
EXPORT_PROPERTY_NAMED_TYPE(int, int)
EXPORT_PROPERTY_NAMED_TYPE(float, float)
EXPORT_PROPERTY_NAMED_TYPE(double, double)
EXPORT_PROPERTY_NAMED_TYPE(std::string, std__string)
EXPORT_PROPERTY_NAMED_TYPE(property_bag::PropertyBag, PropertyBag)

EXPORT_PROPERTY_NAMED_TYPE(std::vector<int>, std_vector_int)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<double>, std_vector_double)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<std::string>, std_vector_string)
