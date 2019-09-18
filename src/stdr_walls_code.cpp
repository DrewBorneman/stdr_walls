#include "ros/ros.h"
#include <unistd.h>
#include <stdlib.h>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Vector3.h"
#include <sstream>
#include <string>

float prevMin = 0;
geometry_msgs::Twist des_vel;
geometry_msgs::Twist cmd_vel;
ros::Publisher vel_pub;

void lidarInput(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	int rangesSize = msg->ranges.size();
	float minRange = msg->ranges[0];
	for(int i=1;i<rangesSize;i++){
		if(msg->ranges[i] < minRange){
			minRange = msg->ranges[i];		
		};
	};
	//ROS_INFO("Min Range: [%f]",minRange);
	cmd_vel = des_vel;
	if(minRange < prevMin && minRange < 0.4f){
		cmd_vel.linear.x = -cmd_vel.linear.x;
		cmd_vel.linear.y = -cmd_vel.linear.y;
		cmd_vel.linear.z = -cmd_vel.linear.z;
		ROS_WARN("within 0.4m of a wall - reversing direction"); 	
	};	
	ROS_INFO("Publishing: [%f]", cmd_vel.linear.x);
	vel_pub.publish(cmd_vel);
	prevMin = minRange;
}

void desVelInput(const geometry_msgs::Twist::ConstPtr& msg)
{
	des_vel.linear = msg->linear;
	des_vel.angular = msg->angular;
}

int main(int argc, char ** argv)
{
	std::string topicName;
	std::string nmspcName;
	int opt;
	while ((opt = getopt(argc, (argv), "n:")) != -1) {
		switch (opt) {
    			case 't':
      				topicName = strdup(optarg);
      				break;
    			case 'n':
      				nmspcName = strdup(optarg);
      				break;
    			default:
      				printf("The -%c is not a recognized parameter\n", opt);
      				break;
		
		}
	} 

	printf("topic name: %s\n",topicName.c_str());

	ros::init(argc,argv,topicName);

	ros::NodeHandle n;

	vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel",1000);
	ros::Subscriber des_vel_pub = n.subscribe<geometry_msgs::Twist>(topicName,1000,desVelInput);
	//ros::Rate loop_rate(10);

	ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("laser_1", 1000, lidarInput);

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
		//loop_rate.sleep();
	}

	return 0;
}

