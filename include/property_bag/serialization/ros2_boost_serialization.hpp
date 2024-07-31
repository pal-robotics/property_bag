/**************************************************************************
**
**  File: ros_boost_serialization
**
**  Authors: victor, Jeremie
**  Created on: 2017/10/24
**
**  Copyright (c) 2017 PAL Robotics SL. All Rights Reserved
**************************************************************************/

#ifndef ROS_BOOST__SERIALIZATION
#define ROS_BOOST__SERIALIZATION
#include "boost/serialization/split_free.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/quaternion.hpp"
#include "geometry_msgs/msg/quaternion_stamped.hpp"
#include "builtin_interfaces/msg/time.hpp"
#include "builtin_interfaces/msg/duration.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/serialization.hpp"

namespace boost{
namespace serialization{

template <class Archive>
void save(Archive & ar, const rclcpp::Time & m, const unsigned int /*version*/) {
  int64_t nanoseconds = m.nanoseconds();
  ar & nanoseconds;
}
template <class Archive>
void load(Archive & ar, rclcpp::Time & m, const unsigned int /*version*/) {
  int64_t nanoseconds;
  ar & nanoseconds;
  m = rclcpp::Time(nanoseconds, RCL_SYSTEM_TIME);
}
template <class Archive>
void serialize(Archive & ar, rclcpp::Time & m, const unsigned int version) {
  split_free(ar,m,version);
}

template <class Archive>
void save(Archive & ar, const rclcpp::Duration & m, const unsigned int /*version*/) {
  int64_t nanoseconds = m.nanoseconds();
  ar & nanoseconds;
}
template <class Archive>
void load(Archive & ar, rclcpp::Duration & m, const unsigned int /*version*/) {
  int64_t nanoseconds;
  ar & nanoseconds;
  m = rclcpp::Duration::from_nanoseconds(nanoseconds);
}
template <class Archive>
void serialize(Archive & ar, rclcpp::Duration & m, const unsigned int version) {
  split_free(ar,m,version);
}

#define ROS_BOOST_SERIALIZE_MSG(Msg) \
template<class Archive>\
void save(Archive & ar, const Msg &m, const unsigned int /*version*/) \
{\
  rclcpp::Serialization<Msg> serializer; \
  rclcpp::SerializedMessage serialized_msg; \
  serializer.serialize_message(&m, &serialized_msg); \
  rcl_serialized_message_t & rcl_sm = \
    serialized_msg.get_rcl_serialized_message(); \
  ar & rcl_sm.buffer_length; \
  for(std::size_t i = 0 ; i < rcl_sm.buffer_length ; ++i) \
  { \
    ar & rcl_sm.buffer[i]; \
  } \
\
}\
\
template<class Archive>\
void load(Archive & ar, Msg &m, const unsigned int /*version*/)\
{\
  std::size_t buffer_length; \
  ar & buffer_length; \
  \
  rclcpp::SerializedMessage serialized_msg(buffer_length, rcl_get_default_allocator()); \
  serialized_msg.get_rcl_serialized_message().buffer_length = buffer_length; \
  rcl_serialized_message_t & rcl_sm = \
    serialized_msg.get_rcl_serialized_message(); \
  for(std::size_t i = 0 ; i < buffer_length ; ++i) \
  { \
    ar & rcl_sm.buffer[i]; \
  } \
  rclcpp::Serialization<Msg> serializer; \
  serializer.deserialize_message(&serialized_msg, &m); \
}\
\
template <class Archive>\
void serialize(Archive & ar, Msg &m, const unsigned int version) {\
  split_free(ar,m,version);\
}

ROS_BOOST_SERIALIZE_MSG(geometry_msgs::msg::Pose);
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::msg::PoseStamped);
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::msg::Point);
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::msg::PointStamped);
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::msg::Quaternion);
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::msg::QuaternionStamped);
ROS_BOOST_SERIALIZE_MSG(builtin_interfaces::msg::Time);
ROS_BOOST_SERIALIZE_MSG(builtin_interfaces::msg::Duration);

} /* namespace serialization */
} /* namespace boost */
#endif /* ROS_BOOST__SERIALIZATION */
