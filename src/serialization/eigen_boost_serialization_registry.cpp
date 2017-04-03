#include <property_bag/serialization/property_boost_serialization.h>
#include <property_bag/serialization/eigen_boost_serialization.h>

// Specific boost specializations for Eigen
EXPORT_PROPERTY_NAMED_TYPE(Eigen::Vector3d, eigen_vector3)
EXPORT_PROPERTY_NAMED_TYPE(Eigen::VectorXd, eigen_vectorxd)
EXPORT_PROPERTY_NAMED_TYPE(Eigen::Quaterniond, eigen_quaterniond)

