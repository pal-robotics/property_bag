/**************************************************************************
**
**  File: ros_boost_serialization
**
**  Author: victor
**  Created on: 2017/10/24
**
**  Copyright (c) 2017 PAL Robotics SL. All Rights Reserved
**************************************************************************/

#ifndef ROS_BOOST_SERIALIZATION
#define ROS_BOOST_SERIALIZATION
#include <ros/serialization.h>
#include <boost/serialization/split_free.hpp>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/QuaternionStamped.h>
#include <std_msgs/Time.h>
#include <std_msgs/Duration.h>

namespace boost{
namespace serialization{

template <class Archive>
void serialize(Archive & ar, ros::Time &t, const unsigned int version) {
  ar & t.sec;
  ar & t.nsec;
}

template <class Archive>
void serialize(Archive & ar, ros::Duration &d, const unsigned int version) {
  ar & d.sec;
  ar & d.nsec;
}


#define ROS_BOOST_SERIALIZE_MSG(Msg) \
template<class Archive>\
void save(Archive & ar, const Msg &m, const unsigned int version)\
{\
  ros::SerializedMessage sm = ros::serialization::serializeMessage(m);\
  ar & sm.num_bytes;\
\
  ptrdiff_t start_offset = sm.message_start - sm.buf.get();\
  ar & start_offset;\
\
  for (size_t i = 0; i < sm.num_bytes; i++)\
  {\
    ar & sm.buf[i];\
  }\
}\
\
template<class Archive>\
void load(Archive & ar, Msg &m, const unsigned int version)\
{\
  size_t num_bytes;\
  ar & num_bytes;\
\
  ptrdiff_t start_offset;\
  ar & start_offset;\
\
  ros::SerializedMessage sm(boost::shared_array<uint8_t>(new uint8_t[num_bytes]), num_bytes);\
  sm.message_start = sm.buf.get() + start_offset;\
\
  for (size_t i = 0; i < sm.num_bytes; i++)\
  {\
    ar & sm.buf[i];\
  }\
  ros::serialization::deserializeMessage(sm, m);\
}\
\
template <class Archive>\
void serialize(Archive & ar, Msg &m, const unsigned int version) {\
  split_free(ar,m,version);\
}


ROS_BOOST_SERIALIZE_MSG(geometry_msgs::Pose)
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::PoseStamped)
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::Point)
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::PointStamped)
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::Quaternion)
ROS_BOOST_SERIALIZE_MSG(geometry_msgs::QuaternionStamped)
ROS_BOOST_SERIALIZE_MSG(std_msgs::Time)
ROS_BOOST_SERIALIZE_MSG(std_msgs::Duration)

} /* namespace serialization */
} /* namespace boost */
#endif /* ROS_BOOST_SERIALIZATION */
