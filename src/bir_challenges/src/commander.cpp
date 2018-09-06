#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

// The setpoint coordenates
double x_pose;
double y_pose;

// Actual turtleBot pose
double x_actual;
double y_actual;
double theta_actual;
	

// The tolerance	
double error = 0.001;

void callBackFunction(const turtlesim::Pose& msg2){
	ROS_INFO("Pose x = %f", msg2.x);
	ROS_INFO("Pose y = %f", msg2.y);
	//ROS_INFO("theta = %f", msg2.theta);
	
	x_actual = msg2.x;
	y_actual = msg2.y;
	theta_actual = msg2.theta;
}

int main(int argc, char**argv){
	// Getting input for setpoint definition
	std::cin >> x_pose;
	std::cin >> y_pose;
	
	ros::init(argc, argv, "commander");
	ros::NodeHandle nh;
	
	// As you can see I'm getting position coordenate from 'turtle1/pose' topic, and moving the turtle bot by 'turtle1/cmd_vel'
	ros::Publisher pub1 = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
	ros::Subscriber sub1 = nh.subscribe("turtle1/pose", 1000, callBackFunction);
	
	// Setting the mensage rate at 50 hz
	ros::Rate loop_rate(50);
	
	int count = 0;
	
	// Creating the mensage object for Pose
	turtlesim::Pose msg2;
	
	while(ros::ok()){
		// Creating the mensage object for commanding velocity
		geometry_msgs::Twist msg1;
		
		// Linear and angular error:
		double x_error = x_pose - x_actual;
		double y_error = y_pose - y_actual;
		double theta_error = atan2 ( ( y_error ) , ( x_error ) ) - theta_actual;
		double linear_error = sqrt(pow(x_error, 2.0) + pow(y_error, 2.0));
								   
		if( linear_error >= error ){
			//msg1.linear.x = ((-x_actual + x_pose)/std::abs((x_actual - x_pose)) )* 0.5;
			//msg1.linear.y = ((-y_actual + y_pose)/std::abs((y_actual - y_pose)) )* 0.5;
			
			// The P controller for both linear and angular velocity
			double Vx= linear_error * cos(theta_error) * 0.7;
			double w = theta_error * 0.5;
			
			//Putting velocity's info inside the mensage
			msg1.linear.x = Vx;
			msg1.angular.z = w;
			
			
			ROS_INFO("error ang = %f", theta_error);
			
			ROS_INFO("Vx = %f", msg1.linear.x);
			//ROS_INFO("Vy = %f", msg1.linear.y);
			ROS_INFO("x_error = %f", x_error);
			ROS_INFO("y_error = %f", y_error);
			ROS_INFO("linear_error = %f", linear_error );
			//double k = ((-x_actual + x_pose)/std::abs((x_actual - x_pose)) );
			//ROS_INFO("%f", k );
		}
		
		else{
			// The turtleBot will stop moving as soon as it reachs the pre setted tolerance
			msg1.linear.x =0.;
			msg1.angular.z = 0.;
			ROS_INFO("linear_error = %f", linear_error );
			//ROS_INFO("else is ok");
		}
		
		// Publishing velocity's informations
		pub1.publish(msg1);
		
		ros::spinOnce();
		
		loop_rate.sleep();
		++count;
	}
	ros::spin();
	
	return 0;

}
			
		
