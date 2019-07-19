# catkin_ws
In order to run this, please have:
- Ubuntu 16.04
- ROS-kinetic
- Gazebo 7
- other packages mentioned in "CMakeLists.txt"

# h_bot
The project "h_bot" is the simulation of a mobile robot with LiDAR and camera on board.
- The agent in gazebo looks like:
![image](https://github.com/Hezihao/catkin_ws/blob/master/IMG/Screenshot%20from%202019-06-17%2001-15-08.png)
- The map built by robot with Gmapping:
![image](https://github.com/Hezihao/catkin_ws/blob/master/IMG/Screenshot%20from%202019-06-17%2001-14-19.png)
- Also, another node which provides real-time map as .png file could be found in this pkg:
![image](https://github.com/Hezihao/catkin_ws/blob/master/IMG/h_map_reader.png)

# teleop_manipulator
This pkg used MoveIt! interface to provide an UI which can manipulate the TCP of robot arm.
- The manipulated robot arm with a certain pose in Gazebo:
![image](https://github.com/Hezihao/catkin_ws/blob/master/IMG/arm_manipulated.png)
- The provided interactive UI:
![image](https://github.com/Hezihao/catkin_ws/blob/master/IMG/tcp_controll.png)
