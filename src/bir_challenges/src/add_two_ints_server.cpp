#include "ros/ros.h"
#include "bir_challenges/AddTwoInts.h"

bool add(bir_challenges::AddTwoInts::Request &req,
         bir_challenges::AddTwoInts::Response &res){

    res.sum = req.a + req.b;
    ROS_INFO("request: a= %ld, b= %ld ",(long int)req.a, (long int)req.b);
    ROS_INFO("response: sum= %ld",(long int)res.sum);
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv,"add_two_ints_server" );
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("add_two_ints", add);
    ROS_INFO("Ready for services operation");
    ros::spin();

    return 0;
}
