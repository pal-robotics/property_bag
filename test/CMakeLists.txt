include_directories(.)

catkin_add_gtest(gtest_any gtest_any.cpp)
target_link_libraries(gtest_any ${PROJECT_NAME} ${Boost_LIBRARIES})

catkin_add_gtest(gtest_property gtest_property.cpp)
target_link_libraries(gtest_property ${PROJECT_NAME} ${Boost_LIBRARIES})

catkin_add_gtest(gtest_property_bag gtest_property_bag.cpp)
target_link_libraries(gtest_property_bag ${PROJECT_NAME} ${Boost_LIBRARIES})

catkin_add_gtest(gtest_property_bag_pair gtest_property_bag_pair.cpp)
target_link_libraries(gtest_property_bag_pair ${PROJECT_NAME} ${Boost_LIBRARIES})

###################
## Serialization ##
###################

catkin_add_gtest(gtest_property_boost_serialization gtest_property_boost_serialization.cpp)
target_link_libraries(gtest_property_boost_serialization ${PROJECT_NAME} ${Boost_LIBRARIES})

catkin_add_gtest(gtest_property_bag_boost_serialization gtest_property_bag_boost_serialization.cpp)
target_link_libraries(gtest_property_bag_boost_serialization ${PROJECT_NAME} ${Boost_LIBRARIES})

catkin_add_gtest(gtest_eigen_boost_serialization gtest_eigen_boost_serialization.cpp)
target_link_libraries(gtest_eigen_boost_serialization ${PROJECT_NAME} ${Boost_LIBRARIES})

catkin_add_gtest(gtest_ros_boost_serialization gtest_ros_boost_serialization.cpp)
target_link_libraries(gtest_ros_boost_serialization ${PROJECT_NAME} ${Boost_LIBRARIES})
