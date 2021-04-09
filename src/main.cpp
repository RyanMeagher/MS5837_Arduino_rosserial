#include <Arduino.h>
#include <Wire.h>
#include "../lib/MS5837/src/MS5837.h"
#include "../lib/rosserial_arduino/src/ros.h"
#include "../lib/rosserial_arduino/src/std_msgs/Float32.h"

ros::NodeHandle  nh;
MS5837 depthSensor;

std_msgs::Float32 float32_msg;
ros::Publisher depthPublisher("depthPublisher", &float32_msg);


void setup()
{

    Wire.begin();
    pinMode(SDA,INPUT);
    pinMode(SCL,INPUT);

    digitalWrite(SDA,LOW);
    digitalWrite(SCL,LOW);

    while (!depthSensor.init()) {
        Serial.println("Init failed!");
        Serial.println("Are SDA/SCL connected correctly?");
        Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
        Serial.println("\n\n\n");
        delay(5000);
    }

    depthSensor.setModel(MS5837::MS5837_30BA);
    depthSensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)*/

    nh.initNode();
    nh.advertise(depthPublisher);


}


void loop()
{
    depthSensor.read();

    float32_msg.data = depthSensor.depth();
    //float32_msg.data = 1.0f;
    depthPublisher.publish(&float32_msg );
    nh.spinOnce();
    delay(500);
}




