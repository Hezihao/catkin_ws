#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <nav_msgs/Odometry.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_broadcaster.h>

#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
vector<float>current_goal = {0.0, 0.0, 0.0};

// Defining the function which fetch parameters from param_server
vector< vector<float> > read_goal_list(ros::NodeHandle name) {
	vector< vector<float> >goal_list;
	vector<float>single_goal = {0.0,0.0,0.0};

	for(int i=0; i<3; i++) {
		goal_list.push_back(single_goal);
	}
	name.getParam("move_base/goals/goal_1/x", goal_list[0][0]);
	name.getParam("move_base/goals/goal_1/y", goal_list[0][1]);
	name.getParam("move_base/goals/goal_1/w", goal_list[0][2]);

	name.getParam("move_base/goals/goal_2/x", goal_list[1][0]);
	name.getParam("move_base/goals/goal_2/y", goal_list[1][1]);
	name.getParam("move_base/goals/goal_2/w", goal_list[1][2]);

	name.getParam("move_base/goals/goal_3/x", goal_list[2][0]);
	name.getParam("move_base/goals/goal_3/y", goal_list[2][1]);
	name.getParam("move_base/goals/goal_3/w", goal_list[2][2]);
	cout<<"List size is: "<<goal_list.size()<<endl;
	return goal_list;
}

// Defining the function which provides current_pose of robot
void riding_status (const nav_msgs::Odometry::ConstPtr& odom_msg ) {
	float diff_linear(0), diff_angular(0);
	vector<float>pose = {odom_msg->pose.pose.position.x,
			     odom_msg->pose.pose.position.y,
			     odom_msg->pose.pose.orientation.w
};
	cout<<"Current pose (x y w) = ( ";
	for(int k=0; k<3; k++) {
		cout<<pose[k]<<" ";
	}
	cout<<")"<<endl;
}

// Defining the function which provides the status of MoveBase
void mb_status (const actionlib_msgs::GoalStatusArray& mb_status_msg) {
	ROS_INFO("move_base status:");
	cout<<mb_status_msg.status_list[0].text<<endl;
}

int main(int argc, char** argv){
	// EXERCISE 1.1: 1) Initialize ROS:
	ros::init(argc, argv, "navigation_goals");
	ros::NodeHandle nh;

	// EXERCISE 1.1: 2) Load a list of goals from a .yaml file:
	//		 *  Specified in the "map" coordinate system & visible in rviz;
	//		 *  Each goal is an 1x3 vector and consists of (x,y,w);
	vector< vector<float> >goal_list = read_goal_list(nh);
	const vector<float>orientation_axis = {0.0, 0.0, 1.0};
	ros::Time start, end;

	// UNCOMMENT to check the real-time pose of robot
	// ros::Subscriber sub_riding_status = nh.subscribe("odom",10, riding_status);
	// ros::Subscriber sub_mb_status = nh.subscribe("move_base/status", 10, mb_status);

	MoveBaseClient mb("move_base", true);
	while(!mb.waitForServer(ros::Duration(3.0))){
		ROS_INFO("Waiting for the move_base action server");
	}

	// EXERCISE 1.1 5) Repeat this until the robot has reached each goal once:
	//		*  To check the status of MoveBase, please uncomment marked code;
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			current_goal[j] = goal_list[i][j];
		}
		move_base_msgs::MoveBaseGoal goal;

		goal.target_pose.header.frame_id = "map";
		goal.target_pose.header.stamp = ros::Time::now();

		goal.target_pose.pose.position.x = current_goal[0];
		goal.target_pose.pose.position.y = current_goal[1];
		goal.target_pose.pose.orientation.x = orientation_axis[0];
		goal.target_pose.pose.orientation.y = orientation_axis[1];
		goal.target_pose.pose.orientation.z = orientation_axis[2];
		goal.target_pose.pose.orientation.w = current_goal[2];

		// EXERCISE 1.1 3) Use the actionlib to send a goal to MoveBase:
		//		*  And it's visualized as RED ARROW in rviz;
		mb.sendGoal(goal);
		ROS_INFO("Goal pose sent.");
		if(i == 0) {
			start = ros::Time::now();
		}
		ROS_INFO("On my way...");
		// UNCOMMENT to check the real-time pose of robot
		// ros::spinOnce();
		while(true) {
			// EXERCISE 1.1 4) Wait until the robot has reached the goal:
			if(mb.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
				// UNCOMMENT to check the status of MoveBase
				// ros::spinOnce();
				ROS_INFO("Arrived!");
				// EXERCISE 1.1 6) Print the total time needed to drive the route:
				if(i==2) {
					end = ros::Time::now();
					ROS_INFO("Total time duration is: ");
					cout<<(end-start)<<endl;
				}
				break;
			}
		}
	}
	// EXERCISE 1.1 7) Exit after missions are completed.
	return 0;
}
