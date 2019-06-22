#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <map>
#include <vector>

#include <ros/ros.h>

#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float64.h>

using namespace std;

// Definition of global variables
int key(0);
float x(0),y(0),z(0),step(0.2);
std_msgs::Float64MultiArray pose;
vector<std_msgs::Float64> init = [0, 0, 0, 0, 0, 0, 0];

// Instructions of using
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

// Map of instructions
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
	std_msgs::MultiArrayDimension d;
	d->label = " ";
	d->size = 2;
	d->stride = 1;
	pose->layout.dim = d;
	pose->data
	init = [0, 0, 0, -0.5, 0, 0.5, 0.75];

	cout<<"Initialized!"<<endl;
}

void set_newGoal()
{

}
int main(int argc, char **argv)
{
	// Initializing node
	ros::init(argc, argv, "teleop_manipulator_keyboard");
	ros::NodeHandle nh;
    	ros::AsyncSpinner spin(1);
    	spin.start();

	// Set up publisher
	ros::Publisher pub = nh.advertise<std_msgs::Float64MultiArray>("/franka/gripper_position_controller/command", 1, true);

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
			// Execution of motion
			key = 0;
		}
	}
	return 0;
}
