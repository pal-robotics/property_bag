#include "utils_gtest.h"

#include "property_bag/serialization/ros_boost_serialization.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <geometry_msgs/PoseStamped.h>

template <typename T>
void deserializeAndCompare(T &expected, boost::archive::text_iarchive &archive)
{
  T actual;
  ASSERT_NO_THROW(archive >> actual);
  std::stringstream expected_ss, actual_ss;
  expected_ss << expected;
  actual_ss << actual;
  EXPECT_EQ(expected_ss.str(), actual_ss.str());
}

TEST(RosSerializationTest, Test)
{
  std::stringstream ss;

  geometry_msgs::PoseStamped pose_stamped;

  pose_stamped.header.frame_id = "foo";
  pose_stamped.header.stamp = ros::Time(1234, 5678);
  pose_stamped.pose.position.x = 1.0;
  pose_stamped.pose.position.y = 0.0;
  pose_stamped.pose.position.z = -3.0;

  geometry_msgs::PointStamped point_stamped;
  point_stamped.header = pose_stamped.header;
  point_stamped.point = pose_stamped.pose.position;

  geometry_msgs::QuaternionStamped quaternion_stamped;
  quaternion_stamped.header = pose_stamped.header;
  quaternion_stamped.quaternion = pose_stamped.pose.orientation;

  ros::Time time(3333, 11231);
  std_msgs::Time time_msg;
  time_msg.data = time;
  ros::Duration duration(12314, 123123);
  std_msgs::Duration duration_msg;
  duration_msg.data = duration;

  {
    boost::archive::text_oarchive oa(ss);

    ASSERT_NO_THROW(oa << pose_stamped);

    ASSERT_NO_THROW(oa << point_stamped);

    ASSERT_NO_THROW(oa << quaternion_stamped);

    ASSERT_NO_THROW(oa << pose_stamped.pose);

    ASSERT_NO_THROW(oa << point_stamped.point);

    ASSERT_NO_THROW(oa << quaternion_stamped.quaternion);

    ASSERT_NO_THROW(oa << time);

    ASSERT_NO_THROW(oa << time_msg);

    ASSERT_NO_THROW(oa << duration);

    ASSERT_NO_THROW(oa << duration_msg);
  }

  {
    geometry_msgs::PoseStamped pose_stamped_saved;
    boost::archive::text_iarchive ia(ss);

    deserializeAndCompare(pose_stamped, ia);
    deserializeAndCompare(point_stamped, ia);
    deserializeAndCompare(quaternion_stamped, ia);
    deserializeAndCompare(pose_stamped.pose, ia);
    deserializeAndCompare(point_stamped.point, ia);
    deserializeAndCompare(quaternion_stamped.quaternion, ia);
    deserializeAndCompare(time, ia);
    deserializeAndCompare(time_msg, ia);
    deserializeAndCompare(duration, ia);
    deserializeAndCompare(duration_msg, ia);
  }
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
