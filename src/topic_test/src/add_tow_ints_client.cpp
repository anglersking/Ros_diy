#include "ros/ros.h" 
#include "topic_test/AddTwoInts.h" 
#include <cstdlib> 
int main(int argc, char **argv) 
{ 
 ros::init(argc, argv, "add_tow_ints_client"); 
 if (argc != 3) 
 { 
 ROS_INFO("usage: add_tow_ints_client X Y"); 
 return 1; 
 } 
 ros::NodeHandle n; 
 ros::ServiceClient client = n.serviceClient<topic_test::AddTwoInts>("add_tow_ints"); 
 topic_test::AddTwoInts srv; 
 srv.request.a = atoll(argv[1]); 
 srv.request.b = atoll(argv[2]); 
 if (client.call(srv)) 
 { 
 ROS_INFO("Sum: %ld", (long int)srv.response.sum); 
 } 
 else 
 { 
 ROS_ERROR("Failed to call service add_tow_ints"); 
 return 1; 
 } 
 return 0; 
} 