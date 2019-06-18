#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <map>
//#include <vector>

#include <ros/ros.h>

#include <moveit/move_group_interface/move_group.h>
#include <moveit_msgs/DisplayTrajectory.h>


using namespace std;

const float pi = 3.1415926;
int key(0);
float x(0),y(0),z(0),step(0.2);
geometry_msgs::Pose goal;

const char* msg = R"(
Reading from the keyboard and Publishing.
---------------------------
Initializing:
0
---------------------------
Moving around:
        8     
   4    5    6
   1    2    3
---------------------------
Correlation with axis:
-x: 1	+x: 3
-y: 6	+y: 4
-z: 2	+z: 8
     brake: 5
---------------------------
step = 0.2;
CTRL-C to quit
)";

map<char, vector<float>> moveBindings
{
  //translation along
  //axis: x, y, z
  {48, {9, 9, 9}},
  {49, {-1, 0, 0}},
  {50, {0, 0, -1}},
  {51, {1, 0, 0}},
  {52, {0, -1, 0}},
  {53, {0, 0, 0}},
  {54, {0, 1, 0}},
  {56, {0, 0, 1}}
};

int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;

  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  // Get the current character
  ch = getchar();

  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

void init_Goal()
{
    	goal.orientation.x = -0.000764819;
    	goal.orientation.y = 0.0366097;
    	goal.orientation.z = 0.00918912;
    	goal.orientation.w = 0.999287;
    	goal.position.x = 0.775884;
    	goal.position.y = 0.43172;
    	goal.position.z = 2.71809;
	cout<<"Initialized!"<<endl;
}

void set_newGoal()
{
	goal.position.x = x + goal.position.x;
	goal.position.y = y + goal.position.y;
	goal.position.z = z + goal.position.z;

	goal.orientation.x = goal.orientation.x;
    	goal.orientation.y = goal.orientation.y;
    	goal.orientation.z = goal.orientation.z;
    	goal.orientation.w = goal.orientation.w;
}
int main(int argc, char **argv)
{
	// Initializing node
	ros::init(argc, argv, "teleop_manipulator_keyboard");
	ros::NodeHandle nh;
    	ros::AsyncSpinner spin(1);
    	spin.start();

	// Set up publisher
	ros::Publisher display_pub = nh.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);

	// Choose arm as planning group
	moveit::planning_interface::MoveGroup plan_group("arm");

	while(true)
	{
		// Get key
		key = getch();
		cout<<key<<endl;
		if(key == '\x03')
		{
			cout<<"break"<<endl;
			break;
		}
		if(moveBindings.count(key)==1)
		{
			// Instruction
			printf("%s", msg);
			if(key != 48)
			{
				x = moveBindings[key][0]*step;
				y = moveBindings[key][1]*step;
				z = moveBindings[key][2]*step;
				set_newGoal();
			}
			else if(key == 48)
			{
				init_Goal();
			}
			plan_group.setGoalTolerance(0.1);
			plan_group.setPoseTarget(goal);
			moveit::planning_interface::MoveGroup::Plan goal_plan;

			if(plan_group.plan(goal_plan))
			{
				moveit_msgs::DisplayTrajectory display_msg;
				display_msg.trajectory_start = goal_plan.start_state_;
				display_msg.trajectory.push_back(goal_plan.trajectory_);
				display_pub.publish(display_msg);
				plan_group.move();
			}
			key = 0;
		}
	}
	return 0;
}
