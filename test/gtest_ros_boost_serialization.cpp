#include "utils_gtest.h"

#include "property_bag/serialization/ros_boost_serialization.hpp"

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

namespace property_bag {
namespace test {

class RosSerializationTest : public testing::Test {
protected:
  void SetUp() override {
    pose_stamped.header.frame_id = "foo";
    pose_stamped.header.stamp = rclcpp::Time(1234, 5678);
    pose_stamped.pose.position.x = 1.0;
    pose_stamped.pose.position.y = 0.0;
    pose_stamped.pose.position.z = -3.0;
    point_stamped.header = pose_stamped.header;
    point_stamped.point = pose_stamped.pose.position;
    quaternion_stamped.header = pose_stamped.header;
    quaternion_stamped.quaternion = pose_stamped.pose.orientation;
    time_msg = time;
    duration_msg = duration;
  }

  template <typename T>
  void deserializeAndCompare(T &expected, boost::archive::text_iarchive &archive)
  {
    T actual;
    ASSERT_NO_THROW_PRINT(archive >> actual);
    EXPECT_EQ(expected, actual);
  }

  void deserializeAndCompare(rclcpp::Duration &expected, boost::archive::text_iarchive &archive)
  {
    rclcpp::Duration actual = rclcpp::Duration::from_nanoseconds(0);
    ASSERT_NO_THROW_PRINT(archive >> actual);
    EXPECT_EQ(expected.nanoseconds(), actual.nanoseconds());
  }

  void deserializeAndCompare(rclcpp::Time &expected, boost::archive::text_iarchive &archive)
  {
    rclcpp::Time actual;
    ASSERT_NO_THROW_PRINT(archive >> actual);
    EXPECT_EQ(expected.nanoseconds(), actual.nanoseconds());
  }

  std::stringstream ss;
  geometry_msgs::msg::PoseStamped pose_stamped;
  geometry_msgs::msg::PointStamped point_stamped;
  geometry_msgs::msg::QuaternionStamped quaternion_stamped;
  rclcpp::Time time = rclcpp::Time(3333, 11231);
  builtin_interfaces::msg::Time time_msg;
  rclcpp::Duration duration = rclcpp::Duration(12314, 123123);
  builtin_interfaces::msg::Duration duration_msg;
};

TEST_F(RosSerializationTest, TestPoseStamped)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << pose_stamped);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(pose_stamped, ia);
	}
}

TEST_F(RosSerializationTest, TestPointStamped)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << point_stamped);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(point_stamped, ia);
  }
}

TEST_F(RosSerializationTest, TestQuaternionStamped)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << quaternion_stamped);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(quaternion_stamped, ia);
	}
}

TEST_F(RosSerializationTest, TestPose)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << pose_stamped.pose);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(pose_stamped.pose, ia);
	}
}

TEST_F(RosSerializationTest, TestPoint)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << point_stamped.point);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(point_stamped.point, ia);
	}
}

TEST_F(RosSerializationTest, TestQuaternion)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << quaternion_stamped.quaternion);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(quaternion_stamped.quaternion, ia);
	}
}

TEST_F(RosSerializationTest, TestTime)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << time);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(time, ia);
	}
}

TEST_F(RosSerializationTest, TestTimeMsg)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << time_msg);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(time_msg, ia);
	}
}

TEST_F(RosSerializationTest, TestDuration)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << duration);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(duration, ia);
	}
}

TEST_F(RosSerializationTest, TestDurationMsg)
{
	{
		boost::archive::text_oarchive oa(ss);
		ASSERT_NO_THROW(oa << duration_msg);
	}
	{
		boost::archive::text_iarchive ia(ss);
		deserializeAndCompare(duration_msg, ia);
	}
}

TEST_F(RosSerializationTest, TestALL)
{
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

}  // namespace test
}  // namespace property_bag

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
