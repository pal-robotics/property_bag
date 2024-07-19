cmake_minimum_required(VERSION 3.16)
project(property_bag)

#
# Options
#
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

#
# Search for dependencies.
#
find_package(eigen3_cmake_module REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()
find_package(Boost COMPONENTS serialization REQUIRED)

<<<<<<< Updated upstream
###################################
## catkin specific configuration ##
###################################

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp std_msgs geometry_msgs
  DEPENDS Boost Eigen
)

###########
## Build ##
###########

include_directories(
  include)

include_directories(SYSTEM
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${Eigen_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(${PROJECT_NAME}
=======
#
# Declare the C++ library
#
ament_auto_add_library(${PROJECT_NAME}
>>>>>>> Stashed changes
  src/property.cpp
  src/serialization/boost_serialization_registry.cpp
  src/serialization/eigen_boost_serialization_registry.cpp
  src/serialization/ros_boost_serialization_registry.cpp
)
target_link_libraries(
  ${PROJECT_NAME} Boost::boost Boost::serialization Eigen3::Eigen)

#
# Testing
#
if(BUILD_TESTING)
  # find_package(ament_lint_auto REQUIRED)
  # ament_auto_find_test_dependencies()
  # ament_lint_auto_find_test_dependencies()
  add_subdirectory(test)
endif()

#
# Handle install and exports.
#
ament_auto_package(CONFIG_EXTRAS "cmake/property_bagConfig-extras.cmake")
