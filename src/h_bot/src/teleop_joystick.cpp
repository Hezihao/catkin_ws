// this node provides the joystick an interface to controll the mobile robot.
//
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

#include <iostream>

using namespace std;

float x_limit(0.5);
float theta_limit(3.1416);

void callBack(const sensor_msgs::Joy::ConstPtr& joy) {
	// here we finish the tasks of reading velocity from joystick and 
	// publishing it to robot after rescaling it.
	geometry_msgs::Twist vel;
	vel.linear.x = x_limit * joy->axes[0];
	vel.angular.z = theta_limit * joy->axes[1];
}

int main(int argc, char** argv) {
	ros::init(argc, argv, "teleop_joystick");
	ros::NodeHandle nh;
	ros::Rate loop_rate(10);
	// this node publishes msgs of type: geometry_msgs/Twist
	ros::Publisher pub;
	// this node subscribes msgs of type: sensor_msgs/Joy
	ros::Subscriber sub;
	while(ros::ok()) {
		pub = nh.advertise<geometry_msgs/Twist>("/cmd_vel", 1);
		// Need to figure out: what does this 1 mean?
		sub = nh.subscribe("joy",100,callBack);
		pub.publish(vel);
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
