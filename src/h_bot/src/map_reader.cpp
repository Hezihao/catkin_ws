#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void map_reader(const nav_msgs::OccupancyGrid::ConstPtr& map_msg)
{
	int width(0), height(0);

	width = map_msg->info.width;
	height = map_msg->info.height;
	ROS_INFO("Map data read.");
	ROS_INFO("Size of this map is %d * %d", width, height);

	Mat map = Mat::zeros(width,height,CV_8U);	
	Mat processed_map = Mat::zeros(1000,1000,CV_8U);
	//cout<<"("<<processed_map.rows<<","<<processed_map.cols<<")"<<endl;
	for(int i=0; i<height;i++)
	{
		for(int j=0; j<width;j++)
		{
			if(map_msg->data[(i*width)+j] == -1)
			{
				map.at<uchar>(i,j) = 5;
			}
			else
			{
				// img in cv.api of Cpp & Py are different? (0-1 / 0-255)
				//map.at<uchar>(i,j) = (map_msg->data[i*width+j])*255/100;
				map.at<uchar>(i,j) = 255-(map_msg->data[i*width+j]);
			}
		}
	}
	resize(map,processed_map,Size(1000,1000),INTER_AREA);
	imshow("Recieved map", processed_map);
	waitKey(0);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "robot_map_reader");
	ros::NodeHandle n;
	while(ros::ok())
	{
		// name of topic & callback function here
		ros::Subscriber map_sub = n.subscribe("map",1000,map_reader); 
		// check if we need to call it
		ros::spin();
	}
	return 0;
}
