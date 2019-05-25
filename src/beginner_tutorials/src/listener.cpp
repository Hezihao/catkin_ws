#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
	ROS_INFO("I heard: [%s]",msg->data.c_str());
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"listener");
	ros::NodeHandle n;
	
	// it's actually a if-sentence, not 100% executed
	ros::Subscriber sub = n.subscribe("chatter",1000,chatterCallback);

	// detecting, if ros.ok()=false, then break the loop here	
	ros::spin();
	return 0;
}
