#include "ros/ros.h"
#include "bir_challenges"
#include <cstdlib>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "add_two_ints_client");
    if (argc != 3)
  {
    ROS_INFO("usage: add_two_ints_client X Y");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceCliente client = n.serviceClient<bir_challenges/AddTwoInts>("add_two_ints");

  bir_challenges::AddTwoInts srv;
  srv.request.a = atoll(argv[1]);
  srv.request.b = atoll(argv[2]);

  if(client.call(srv)){
    ROS_INFO("sum= %ld",srv.response.sum);
  }
  
  else
  {
      ROS_INFO("failed to call service add_two_ints");
      return 1;
  }
  

    return 0;
}
