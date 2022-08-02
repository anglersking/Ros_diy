#include "ros/ros.h" 
#include "std_msgs/String.h" 
#include <sstream> 
#include<geometry_msgs/Twist.h>



#include <iostream>

#include<fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sqlite3.h>

#include<jsoncpp/json/json.h>
#define KGRN "\033[0;32;32m"
#define KCYN "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[1;33m"
#define KMAG "\033[0;35m"
#define KBLU "\033[0;32;34m"
#define KCYN_L "\033[1;36m"
#define RESET "\033[0m"


int dataToFile(const char* name,const char* data){
	//unsigned char* buffer= data;
 	int fd = open(name,O_RDWR|O_CREAT,0666);
 	write(fd,data,strlen(data));
 	std::cout << "data size: " << strlen(data) << std::endl;
 	close(fd);
 	return 0;
}
 std::string readFile(const char* name){
	int fd = open(name,O_RDWR|O_CREAT,0666);
	unsigned char buffer[1024];
    std::string res;
	int readbytes = read(fd,(void*)buffer,1024);
	if(readbytes > 0){
		
    close(fd);
    res=(char*)buffer;
    return  res;
	}
 }

void ws_ros_adaptor_Callback(const geometry_msgs::Twist::ConstPtr& twist_data)
{
      
      std::string jsonstr;
	  jsonstr="{+\"linear:\"+{\"x\":"+std::to_string(twist_data->linear.x)+",\"y\":"+std::to_string(twist_data->linear.y) +",\"z\":"+std::to_string(twist_data->linear.y)+"},\"angular:\"{\"x\":"+ std::to_string(twist_data->angular.x)+",\"y\":"+std::to_string(twist_data->angular.y)+",\"z\":"+std::to_string(twist_data->angular.y)+"}}";
	  /*
	  twist_data_json["linear"]["x"]=Json::Value(twist_data->linear.x );
	  twist_data_json["linear"]["y"]=Json::Value(twist_data->linear.y) ; 
	  twist_data_json["linear"]["z"]=Json::Value(twist_data->linear.z) ; 

	  twist_data_json["angular"]["x"]=Json::Value(twist_data->angular.x) ; 
	  twist_data_json["angular"]["y"]=Json::Value(twist_data->angular.y) ; 
	  twist_data_json["angular"]["z"]=Json::Value(twist_data->angular.z); 
	  root.append(twist_data_json);
*/

	  ROS_INFO("%f", twist_data->linear.x); 
	  ROS_INFO("%f", twist_data->linear.y); 
	  ROS_INFO("%f", twist_data->linear.z); 
	  ROS_INFO("%f", twist_data->angular.x); 
	  ROS_INFO("%f", twist_data->angular.y); 
	  ROS_INFO("%f", twist_data->angular.z); 
	  
	  dataToFile("/home/yu/docker_ubuntu16.04/dockerdata/Ros_diy/src/ws_ros_adaptor/src/feedback.json",jsonstr.c_str());
	


}

int main(int argc, char **argv) 
{ 
	ros::init(argc, argv, "ws_ros_adaptor"); 
	ros::NodeHandle n; 
	ros::Publisher ws_ros_adaptor_pub = n.advertise<geometry_msgs::Twist>("ws_ros_adaptor_data", 1000); 
    ros::Subscriber ws_ros_adaptor_sub = n.subscribe("ws_ros_adaptor_data", 1000, ws_ros_adaptor_Callback);
	ros::Rate loop_rate(10); 
	int count = 0; 

	while (ros::ok()) 
	 { 
		if ( readFile("/home/yu/docker_ubuntu16.04/dockerdata/Ros_diy/src/ws_ros_adaptor/src/control.json")!="error")
		{

		std::string cmd=readFile("/home/yu/docker_ubuntu16.04/dockerdata/Ros_diy/src/ws_ros_adaptor/src/control.json");
		//std::cout<<"jsonjsonjosn"<<std::endl;
        //std::cout<<cmd<<std::endl;
	    //std::cout<<"----------------------------"<<std::endl;
		Json::Reader Reader;
        Json::Value DevJson;
		Reader.parse(cmd,DevJson);

		//std::cout<<DevJson["linear"]["x"]<<std::endl;
     //twistms.available=0;
     //Brakes.available=DevJson["available"].asInt();
     //Brakes.brake_command_data.brake_command=DevJson["brake_command"].asFloat();


		geometry_msgs::Twist twist_data; 
		
		twist_data.linear.x = DevJson["linear"]["x"].asFloat(); 
		twist_data.linear.y = DevJson["linear"]["y"].asFloat(); 
		twist_data.linear.z = DevJson["linear"]["z"].asFloat(); 

		twist_data.angular.x = DevJson["angular"]["x"].asFloat(); 
		twist_data.angular.y = DevJson["angular"]["y"].asFloat(); 
		twist_data.angular.z = DevJson["angular"]["z"].asFloat(); 


		//ROS_INFO("%f", twist_data.linear.x); 
		ws_ros_adaptor_pub.publish(twist_data); 
		std::fstream file("/home/yu/docker_ubuntu16.04/dockerdata/Ros_diy/src/ws_ros_adaptor/src/control.json", std::ios::out);
 
        readFile("/home/yu/docker_ubuntu16.04/dockerdata/Ros_diy/src/ws_ros_adaptor/src/control.json");

		ros::spinOnce(); 
		loop_rate.sleep();
		++count; 
	   } 
	 }
	 return 0; 
}
