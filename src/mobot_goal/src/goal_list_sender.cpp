#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_broadcaster.h>

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
float tolerance(0.15);

vector< vector<float> > read_goal_list(ros::NodeHandle name) {
	vector< vector<float> >goal_list;
	vector<float>single_goal = {0.0,0.0,0.0};

	for(int i=0; i<3; i++) {
		goal_list.push_back(single_goal);
	}
	name.getParam("move_base/goals/goal_1/x", goal_list[0][0]);
	cout<<"Initialization of data ok!"<<endl;
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

int main(int argc, char** argv){
	ros::init(argc, argv, "navigation_goals");
	ros::NodeHandle nh;

	MoveBaseClient mb("move_base", true);
	while(!mb.waitForServer(ros::Duration(3.0))){
		ROS_INFO("Waiting for the move_base action server");
	}

	vector< vector<float> >goal_list = read_goal_list(nh);
	vector<float>single_goal = {0.0, 0.0, 0.0};
	const vector<float>orientation_axis = {0.0, 0.0, 1.0};

	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			single_goal[j] = goal_list[i][j];
		}
		move_base_msgs::MoveBaseGoal goal;

		goal.target_pose.header.frame_id = "map";
		goal.target_pose.header.stamp = ros::Time::now();

		goal.target_pose.pose.position.x = single_goal[0];
		goal.target_pose.pose.position.y = single_goal[1];
		goal.target_pose.pose.orientation.x = orientation_axis[0];
		goal.target_pose.pose.orientation.y = orientation_axis[1];
		goal.target_pose.pose.orientation.z = orientation_axis[2];
		goal.target_pose.pose.orientation.w = single_goal[2];

		mb.sendGoal(goal);
		ROS_INFO("Goal pose sent.");

		ros::Time start = ros::Time::now();

		// check current status of MoveBase while driving
		ROS_INFO("On my way...");
		while(true) {
			if(mb.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
				ROS_INFO("You have arrived to the goal position");
				break;
			}
			/*else{
				ROS_INFO("The base failed for some reason");
			}*/
		}
		if(i==2) {
			ros::Time end = ros::Time::now();
			ROS_INFO("Total time duration is: ");
			cout<<(end-start)<<endl;
		}		
	}
	return 0;
}
