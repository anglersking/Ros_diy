#include "ros/ros.h"
#include "topic_test/Message.h"
void chatterCallback(const topic_test::Message::ConstPtr& msg)
{
 ROS_INFO("I heard: [%s]", msg->message.c_str());
}
int main(int argc, char **argv)
{
 ros::init(argc, argv, "listener");
 ros::NodeHandle n;
 ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
 ros::spin();
 return 0;
}
