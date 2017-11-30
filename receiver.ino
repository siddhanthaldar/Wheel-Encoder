/*This is the arduino code for the nRF module at the receiving end. This modules 
  receives the velocities of the left and right wheels and publishes it on different
  ROS topics using ROS Serial.  
*/

    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    #include <ros.h>
    #include <std_msgs/Float32.h>

    ros::NodeHandle nh;

    std_msgs::Float32 left;
    std_msgs::Float32 right;
    ros::Publisher chatter1("l_vel", &left);
    ros::Publisher chatter2("r_vel", &right);
        
    RF24 radio(9,10)  ; // CNS, CE
    const byte address = 0xf0f0f0f0c3;
    
      
    void setup() 
    {
      nh.initNode();
      nh.advertise(chatter1);
      nh.advertise(chatter2);
      Serial.begin(57600);
      radio.begin();
      radio.openReadingPipe(1, address);
      radio.setPALevel(RF24_PA_MIN);
      radio.startListening();
     
    }
    
    void loop() 
    {
      if (radio.available()) 
      { 
        float data[2];
        
        radio.read(&data, sizeof(data));

        if(data[0] == 1)
        {
          left.data = data[1];
          chatter1.publish(&left);
        }
        if(data[0] ==2){
          right.data = data[1];
          chatter2.publish(&right);
        }
        
      }

      nh.spinOnce();
   
    }
