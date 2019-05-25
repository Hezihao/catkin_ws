#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, char **argv)
{
	// initializing the node as talker
	ros::init(argc,argv,"talker");
	// setup a session here
	ros::NodeHandle n;
	// declair that it's about topic "chatter"
	// 1000 is the size of queue
	// Nodehandle::advertise() returns a ros::Publisher, which can be used when publishing later
	// and the type of msg is defined here as <std_msgs::string>
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
	// controlling the time length of a period, and it's cooperating with loop_rate.sleep()	
	ros::Rate loop_rate(10); // 10 Hz as refreshing rate

	int count(0);
	// ros::ok() is detecting keyboard input, where none=true, any=false and breaks the loop
	while(ros::ok())
	{
		std_msgs::String msg;
		
		std::stringstream ss;
		ss<<"hello,world"<<count;
		// msg only has one member "data"
		msg.data = ss.str();
		// same as printf()
		ROS_INFO("%s", msg.data.c_str());
		// publishing msg to all subscribers
		chatter_pub.publish(msg);
		
		// it's a function reminding core to deal with the msg just recieved
		// and then get back here and continue
		ros::spinOnce();
		
		// length of sleep() is automatically defined, in order to maintain
		// the loop frequency
		loop_rate.sleep();
		++count;
	}
	return 0;
}
