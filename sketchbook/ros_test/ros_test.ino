#include <ros.h>
#include <ArduinoHardware.h>
#include <geometry_msgs/Twist.h>
#include <SR04.h>
#include "DualMC33926MotorShield.h"

#define ECHOPIN1 51
#define TRIGPIN1 53  
#define ECHOPIN2 47
#define TRIGPIN2 49

ros::NodeHandle nh;
DualMC33926MotorShield md;
int flag = 0;

void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}

void messageCb(const geometry_msgs::Twist& toggle_mag){
  flag = toggle_mag.linear.x;
  Serial.println(flag);
  //md.setM2Speed(100);
  switch(flag){
    case 2:{//forward cat
      md.setM1Speed(150);
      md.setM2Speed(-150);
      //delay(2000);
      //md.setSpeeds(0, 0);
      break;
    }
    case 6:{//left dog
      md.setM1Speed(150);
      md.setM2Speed(150);
      //delay(2000);
      //md.setSpeeds(0, 0);
      break;
    }
    case -6:{//right bicycle
      md.setM1Speed(-150);
      md.setM2Speed(-150);
      //delay(2000);
      //md.setSpeeds(0, 0);
      break;
    }
    case -2:{//back person
      md.setM1Speed(-150);
      md.setM2Speed(150);
      //delay(2000);
      //md.setSpeeds(0, 0);
      break;
    }
    default:{//stop chair
      md.setSpeeds(0, 0);
      break;
    }
  }
  flag = 0;
}

ros::Subscriber<geometry_msgs::Twist> sub("/turtle1/cmd_vel", &messageCb );

void setup(){
  nh.initNode();
  Serial.begin(57600);
  md.init();
  nh.subscribe(sub);
  pinMode(ECHOPIN1, INPUT);
  pinMode(ECHOPIN2, INPUT);
  pinMode(TRIGPIN1, OUTPUT);
  pinMode(TRIGPIN2, OUTPUT);
}

void loop(){
  nh.spinOnce();
  digitalWrite(TRIGPIN1, LOW);
  delay(2);
  digitalWrite(TRIGPIN1, HIGH);
  delay(10);
  digitalWrite(TRIGPIN1, LOW);
  float distance1 = pulseIn(ECHOPIN1, HIGH);
  distance1 = distance1 / 58;
  
  digitalWrite(TRIGPIN2, LOW);
  delay(2);
  digitalWrite(TRIGPIN2, HIGH);
  delay(10);
  digitalWrite(TRIGPIN2, LOW);
  float distance2 = pulseIn(ECHOPIN2, HIGH);
  distance2 = distance2 / 58;
  Serial.println(distance1);
  Serial.println(distance2);
  if(distance1 < 20 || distance2 < 20){
    md.setSpeeds(0, 0);
  }
  
}

