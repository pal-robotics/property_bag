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
#include "rclcpp/rclcpp.hpp"
#include "boost/serialization/serialization.hpp"
#include "boost/serialization/split_free.hpp"
#include "boost/serialization/export.hpp"
#include "boost/serialization/access.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/quaternion.hpp"
#include "geometry_msgs/msg/quaternion_stamped.hpp"
#include "builtin_interfaces/msg/time.hpp"
#include "builtin_interfaces/msg/duration.hpp"
#include "rclcpp/serialization.hpp"
#include "property_bag/property.hpp"

namespace property_bag {
namespace details {

template<>
class PlaceHolderImpl<rclcpp::Duration> : public PlaceHolder
{
public:

/**
 * @brief 'pimpl' struct to enable access to
 * private members during serialization
 */
struct serialization_accessor
{
	template <typename Archive>
	static void serialize(Archive& ar, PlaceHolderImpl<rclcpp::Duration>& pl,
                        const unsigned int /*file_version*/)
	{
		// Serialize the value_
		ar & pl.value_;
	}
};

/**
 * @brief PlaceHolderImpl. Default constructor
 */
PlaceHolderImpl() : value_(std::chrono::nanoseconds(0)){
}

/**
 * @brief PlaceHolderImpl
 * copy (T) constructor
 * @param value
 */
explicit PlaceHolderImpl(const rclcpp::Duration& value) :
	PlaceHolder(),
	value_(value) {
}

/**
 * @brief PlaceHolderImpl
 * move (rclcpp::Duration) constructor
 * @param value
 */
explicit PlaceHolderImpl(rclcpp::Duration&& value) :
	PlaceHolder(),
	value_(std::move(value)) {
}

/**
 * @brief PlaceHolderImpl
 * copy (PlaceHolderImpl<T>) constructor
 * @param o
 */
explicit PlaceHolderImpl(const PlaceHolderImpl<rclcpp::Duration>& o) :
	PlaceHolder(),
	value_(o.value_) {
}

/**
 * @brief PlaceHolderImpl
 * move (PlaceHolderImpl<T>) constructor
 * @param o
 */
explicit PlaceHolderImpl(PlaceHolderImpl<rclcpp::Duration>&& o) :
	PlaceHolder(),
	value_(std::move(o.value_)) {
}

/**
 * @brief operator =
 * copy (PlaceHolderImpl<T>) assignment
 * @param o. Another const PlaceHolderImpl<T>
 * @return this PlaceHolderImpl<T>
 */
PlaceHolderImpl<rclcpp::Duration>& operator=(const PlaceHolderImpl<rclcpp::Duration>& o)
{
	value_ = o.value_;
	return *this;
}

/**
 * @brief operator =
 * move (PlaceHolderImpl<T>) assignment
 * @param o. Another movable PlaceHolderImpl<T>
 * @return this PlaceHolderImpl<T>
 */
PlaceHolderImpl<rclcpp::Duration>& operator=(PlaceHolderImpl<rclcpp::Duration>&& o)
{
	value_ = std::move(o.value_);
	return *this;
}

/**
 * @brief ~PlaceHolderImpl. Default destructor.
 */
~PlaceHolderImpl() = default;

/**
 * @brief type
 * @return std::type_info. typeid(T)
 */
inline const std::type_info& type() override {
	return typeid(rclcpp::Duration);
}

protected:

rclcpp::Duration value_;

template<typename TT>
friend TT& anycast(Any &val);

template<typename TT>
friend const TT& anycast(const Any &val);
};

}  // namespace details
}  // namespace property_bag


namespace boost {
namespace serialization {

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
	template<class Archive> \
	void save(Archive & ar, const Msg &m, const unsigned int /*version*/) \
	{ \
		rclcpp::Serialization<Msg> serializer; \
		rclcpp::SerializedMessage serialized_msg; \
		serializer.serialize_message(&m, &serialized_msg); \
		rcl_serialized_message_t & rcl_sm = \
			serialized_msg.get_rcl_serialized_message(); \
		ar & rcl_sm.buffer_length; \
		for(std::size_t i = 0; i < rcl_sm.buffer_length; ++i) \
		{ \
			ar & rcl_sm.buffer[i]; \
		} \
\
	} \
\
	template<class Archive> \
	void load(Archive & ar, Msg &m, const unsigned int /*version*/) \
	{ \
		std::size_t buffer_length; \
		ar & buffer_length; \
  \
		rclcpp::SerializedMessage serialized_msg(buffer_length, rcl_get_default_allocator()); \
		serialized_msg.get_rcl_serialized_message().buffer_length = buffer_length; \
		rcl_serialized_message_t & rcl_sm = \
			serialized_msg.get_rcl_serialized_message(); \
		for(std::size_t i = 0; i < buffer_length; ++i) \
		{ \
			ar & rcl_sm.buffer[i]; \
		} \
		rclcpp::Serialization<Msg> serializer; \
		serializer.deserialize_message(&serialized_msg, &m); \
	} \
\
	template <class Archive> \
	void serialize(Archive & ar, Msg &m, const unsigned int version) { \
		split_free(ar,m,version); \
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
