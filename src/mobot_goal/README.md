# Multi-goal-driver
In order to test this pkg, please have:
- Ubuntu 16.04
- ROS-kinetic
- Gazebo
- pkg: h_bot

Commands needed are:
- $ roslaunch mobot_goal goal_list_sender.launch
- $ rosrun mobot_goal goal_list_sender

While running the test, following things could be observed:
- Rviz showing planned path & current goal etc.:
![image](https://github.com/Hezihao/catkin_ws/blob/master/src/mobot_goal/IMG/reaching_2nd_goal.png)

- Gazebo showing the simulation of robot:
![image](https://github.com/Hezihao/catkin_ws/blob/master/src/mobot_goal/IMG/in_gazebo.png)

- As result in terminal:
![image](https://github.com/Hezihao/catkin_ws/blob/master/src/mobot_goal/IMG/Result_of_program.png)
