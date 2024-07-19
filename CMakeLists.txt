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

#
# Declare the C++ library
#
add_library(${PROJECT_NAME}
  src/property.cpp
  src/serialization/boost_serialization_registry.cpp
  src/serialization/eigen_boost_serialization_registry.cpp
  src/serialization/ros_boost_serialization_registry.cpp
)
target_include_directories(
  ${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME}
  Boost::boost Boost::serialization Eigen3::Eigen)
ament_target_dependencies(${PROJECT_NAME}
  geometry_msgs rclcpp builtin_interfaces std_msgs)

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
ament_export_targets(${PROJECT_NAME}-targets HAS_LIBRARY_TARGET)
install(DIRECTORY include/ DESTINATION include)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
ament_auto_package(CONFIG_EXTRAS "cmake/property_bagConfig-extras.cmake")
