#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include<std_msgs/Float64.h>

//Ros::Publisher motor command
ros::Publisher motor_command_publisher;



/*******************************************/
/* // TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities */

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res){

    ROS_INFO("GoToPositionRequest received - linear:%1.2f, angular:%1.2f", (float)req.linear_x, (float)req.angular_z);

    //publish the values to mobile robot
    geometry_msgs::Twist values;
     values.linear.x= req.linear_x;
     values.angular.z= req.angular_z;

    motor_command_publisher.publish(values);

    res.msg_feedback="Linear and Angular values set to: " + std::to_string((float)values.linear.x)+" and " + std::to_string((float)values.angular.z);

    ROS_INFO_STREAM(res.msg_feedback);

    return true;

}

/******************************************/

int main(int argc, char* argv[]){
    // initialize ROS node
    ros::init(argc, argv, "drive_bot");

    //Ros handle
    ros::NodeHandle n;

    geometry_msgs::Twist motor_command;


    //Create a ROS nodehandle object.
    // handle created globally

    // inform ros master that we will be publishing a msg of type geometry_msgs::Twist on the robot actuation topc with a publishing queue size of 10
    motor_command_publisher =n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    /**********************************************/

    // TODO: define a drive /ball_chaser/command_robot service with a handle_drive_reuest callback function


    //publish angles to drive the robot
    motor_command_publisher.publish(motor_command);
    
    //request drive_bot service
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

    

    /***********************************************/

    /**************************************************/
    
    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the reuqested velocities instead of constant values

    
    /************************************************/

    // while (ros::ok())
    // {
    //     // crate a mmotor_command object of type geometry::Twist
    //     geometry_msgs::Twist motor_command;

    //     //set wheel velocities, forward [0.5 0.0]
    //     motor_command.linear.x = 0.5;
    //     motor_command.angular.z = 0.0;

    // }

    //TODO: Handle ROS communication events
    ros::spin();

    return 0;
    
}
