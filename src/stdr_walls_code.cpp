#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

void lidarInput(const std_msgs::String::ConstPtr& msg)
{
	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char ** argv)
{

	ros::init(argc,argv,"des_vel");

	ros::NodeHandle n;

	ros::Publisher vel_pub = n.advertise<std_msgs::String>("cmd_vel",1000);
	ros::Publisher des_vel_pub = n.advertise<std_msgs::String>("des_vel",1000);
	ros::Rate loop_rate(1);

	ros::Subscriber sub = n.subscribe("robot0/laser_1", 1000, lidarInput);

	while (ros::ok())
	{
		/**
		
		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();
		ROS_INFO("%s",msg.data.c_str());
		
		* The publish() function is how you send messages. The parameter
		* is the message object. The type of this object must agree with the type
		* given as a template parameter to the advertise<>() call, as was done
		* in the constructor above.
		
		chatter_pub.publish(msg);
		*/
		ros::spin();
		loop_rate.sleep();
	}

	return 0;
}

