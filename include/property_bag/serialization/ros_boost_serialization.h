/**************************************************************************
**
**  File: ros_boost_serialization
**
**  Authors: victor, Jeremie
**  Created on: 2017/10/24
**
**  Copyright (c) 2017 PAL Robotics SL. All Rights Reserved
**************************************************************************/

#ifndef ROS_BOOST_SERIALIZATION
#define ROS_BOOST_SERIALIZATION
#include <ros/serialization.h>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/array.hpp>
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
  ar & BOOST_SERIALIZATION_NVP(t.sec);
  ar & BOOST_SERIALIZATION_NVP(t.nsec);
}

template <class Archive>
void serialize(Archive & ar, ros::Duration &d, const unsigned int version) {
  ar & BOOST_SERIALIZATION_NVP(d.sec);
  ar & BOOST_SERIALIZATION_NVP(d.nsec);
}

#define ROS_BOOST_SERIALIZE_MSG(Msg) \
template<class Archive>\
void save(Archive & ar, const Msg &m, const unsigned int version)\
{\
  ros::SerializedMessage sm = ros::serialization::serializeMessage(m);\
  ar & boost::serialization::make_nvp("num_bytes", sm.num_bytes);\
\
  ptrdiff_t start_offset = sm.message_start - sm.buf.get();\
  ar & BOOST_SERIALIZATION_NVP(start_offset);\
\
  ar & boost::serialization::make_nvp("buffer", \
        boost::serialization::make_array(&sm.buf[0], sm.num_bytes) );\
\
}\
\
template<class Archive>\
void load(Archive & ar, Msg &m, const unsigned int version)\
{\
  size_t num_bytes;\
  ar & BOOST_SERIALIZATION_NVP(num_bytes);\
\
  ptrdiff_t start_offset;\
  ar & BOOST_SERIALIZATION_NVP(start_offset);\
\
  ros::SerializedMessage sm(boost::shared_array<uint8_t>(new uint8_t[num_bytes]), num_bytes);\
  sm.message_start = sm.buf.get() + start_offset;\
\
  ar & boost::serialization::make_nvp("buffer", \
        boost::serialization::make_array(&sm.buf[0], sm.num_bytes) );\
\
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
