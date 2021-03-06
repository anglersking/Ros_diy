#include "ros/ros.h" 
//#include "std_msgs/String.h" 
#include "topic_test/Message.h"
#include <sstream> 
int main(int argc, char **argv) 
{ 
	ros::init(argc, argv, "talker"); 
	ros::NodeHandle n; 
	ros::Publisher chatter_pub = n.advertise<topic_test::Message>("chatter", 1000); 
	ros::Rate loop_rate(10); 
	int count = 0; 
	while (ros::ok()) 
	 { 
		topic_test::Message msg; 
		std::stringstream ss; 
		ss <<"hello world "<< count; 
		msg.message = ss.str(); 
		ROS_INFO("%s", msg.message.c_str()); 
		chatter_pub.publish(msg); 
		ros::spinOnce(); 
		loop_rate.sleep(); 
		++count; 
	 } 
	 return 0; 
}
