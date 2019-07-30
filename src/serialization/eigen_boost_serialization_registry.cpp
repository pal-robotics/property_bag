#include <property_bag/serialization/property_boost_serialization.h>
#include <property_bag/serialization/eigen_boost_serialization.h>

// Specific boost specializations for Eigen
EXPORT_PROPERTY_NAMED_TYPE(Eigen::Vector3d, eigen_vector3)
EXPORT_PROPERTY_NAMED_TYPE(Eigen::VectorXd, eigen_vectorxd)
EXPORT_PROPERTY_NAMED_TYPE(Eigen::Quaterniond, eigen_quaterniond)
EXPORT_PROPERTY_NAMED_TYPE(Eigen::Isometry3d, eigen_isometry_3d)

EXPORT_PROPERTY_NAMED_TYPE(std::vector<Eigen::Vector3d>, std_vector_eigen_vector3)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<Eigen::VectorXd>, std_vector_eigen_vectorxd)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<Eigen::Quaterniond>, std_vector_eigen_quaternion)
EXPORT_PROPERTY_NAMED_TYPE(std::vector<Eigen::Isometry3d>, std_vector_eigen_isometry)


