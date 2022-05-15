#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>


// function declaration
std::string ball_position(int,const sensor_msgs::Image);
std::string position="";
std::string pos;
bool flag_ball_found = false;

// define a global client that can request services
ros::ServiceClient client;

/* 
this function calls the command_robot service to drive the robot in the specified direction
*/ 

void drive_robot(float lin_x, float ang_z){
    /* ToDo:
    Request a service and pass the velocities
    */
//    ROS_INFO_STREAM("Driving the Robot in the Direction of ball.");
   ball_chaser::DriveToTarget srv;
   srv.request.linear_x = lin_x;
   srv.request.angular_z = ang_z;


   /* requesting a service and passing the values*/
   if (!client.call(srv)){
       ROS_ERROR("Failed to call the service commad_robot!.");
   }
}

/*
this callback function continuously executes adn reads the 
image data
*/

void process_image_callback(const sensor_msgs::Image img){
    int white_pixel = 255;    

    /* TODO:
    (i) Loop through each pixel in the image and check if there is a bright white one.
    
    (ii) Then, Identify if this pixel falls in the left, mid, or right side of the image.
    
    (iii) Depending on the white ball position, call the drive_bot function and pass the velocities to it.

    (iv) Request a stop when there's no white ball seen by the camera.
    
    */

   //(i) looping through each pixel. and //(ii) check whether the pixel is in right, left or in the mid of the image
//    testing for rows and columns, and steps

    // std::cout<<"Rows = "<<img.height<<", Columns = "<<img.width<<", and Steps = "<<img.step<<std::endl;

//    printing number of rows

   for(int i=0; i<((img.height)*(img.step)); i+=3){
       flag_ball_found =false;
        if((img.data[i] == white_pixel) && (img.data[i+1] == white_pixel) && (img.data[i+2] == white_pixel)){
        //   std::cout<<"The value of "<<i<<" pixel is "<< (int)img.data[i]<<std::endl;
            flag_ball_found = true;
            pos = (ball_position(((int)i%img.step),img));
            std::cout<<"ball found on "<<pos<<" side. \n";
             break;
        }
    }
    
    std::string lft = "left";
    std::string rgt = "right";
    std::string cnt = "center";
    // std::string nn = "none";
    //(iii) - drive the robot in the direction of ball
    if(pos.compare(lft)==0 && flag_ball_found == true){
        ROS_INFO_STREAM("Driving Robot to Left");
        drive_robot(0.2,1.5);
    }
    else if (pos.compare(rgt)==0 && flag_ball_found == true)
    {
        ROS_INFO_STREAM("Driving Robot to Right");
        drive_robot(0.2,-1.5);
    }
    else if (pos.compare(cnt)==0 && flag_ball_found == true)
    {
        ROS_INFO_STREAM("Driving Robot to Center");
        drive_robot(0.3,0.0);
    }
    else if (flag_ball_found == false)
    {
        ROS_INFO_STREAM("Stopping Robot");
        drive_robot(0.0,0.0);
    }

}

// function to check the ball position
    std::string ball_position(int j, const sensor_msgs::Image img){
        std::cout<<"Position Function Called. \n";
        // const sensor_msgs::Image img;
        int tot_col = img.step;

        std::cout<<"Number of Columns = "<<tot_col<<std::endl;
        std::cout<<"Ball found in Column = "<<j<<std::endl;
        int sec_width_left = (int)(tot_col/3);
        int sec_width_right = (int)((tot_col*2)/3);

        if ( j>=0 && j<sec_width_left)
        {
            // position.clear();
            position="left";
            std::cout<<"Ball found in col: "<<j<<" and in "<<position<<std::endl;
        }
        else if(j>=sec_width_left && j<sec_width_right)
        {
            // position.clear();
            position="center";
            std::cout<<"Ball found in col: "<<j<<" and in "<<position<<std::endl;
        }
        else if(j>= sec_width_right && j<tot_col)
        {
            // position.clear();
            position="right";
            std::cout<<"Ball found in col: "<<j<<" and in "<<position<<std::endl;
        }
        std::cout<<"Position == "<<position<<std::endl;
        return position;
    }

int main(int argc, char** argv){
    /* Initialize teh process_image node and create a handle to it.*/
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    /* Define a client service capable of requesting services from command_robot*/
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    /*Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    */
   ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw",10,process_image_callback);


    /*Handle ros communication events*/
    ros::spin();

    return 0;

}