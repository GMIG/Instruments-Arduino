#define _TASK_LTS_POINTER
#define DEBUG_INITIAL_LEVEL 5
#define DEBUG_MINIMUM true
//#include "SerialDebug.h"

#include <Arduino.h>
#include <TaskScheduler.h>

#include "Presence.h"
#include "Rotation.h"
#include "Debug.h"
#include "Button.h"
#include "GPIOSwitch.h"

#include "SerialTransport.h"
#include "ExternalCommands.h"

Scheduler scheduler;
SerialTransport transport(Serial); 

const int NUM_OF_SENSES = 6;
//Presence s(5,"pres",&transport);
//Presence s2(2,"pres2",&transport);
//Presence s3(3,"pres3",&transport);

Rotation joy(A0,"joy",&transport);
Rotation rot(A1,"rot",&transport);
Rotation ang(A2,"ang",&transport);
//Rotation r5(14,"rot4",&transport);

Debug d("debug",&transport);
Button but(A3,"but",&transport);
Button but2(10,"but2",&transport);
Button but3(10,"but3",&transport);
Button but4(11,"but4",&transport);

GPIOName GPIOS[] = {
  {O0,"red"},
  {O1,"rel"},
  {O2,"grn"}
};

GPIOSwitch gpio("outs",GPIOS,3);

Commandable* senses[NUM_OF_SENSES] = {&joy,&rot,&ang,&but,&but4,&gpio};//,&r,&r3,&r4,&b2,&b3,&s,&r}; 

//Sense* senses[2] = {&r3,&r}; 

ExternalCommands disp("cmd",&transport,senses,NUM_OF_SENSES);

void setup() {
  //debugSetLevel(5);
  Serial.begin(57600);
    char buf[1] ;

  //b.start(buf);
  //s2.start(buf);
  //r.start(buf);
  //b2.start();
  disp.start(buf);
  disp.runall(buf);
  Serial.println("sys:1");
}

void loop() {
	//debugHandle();

  // put your main code here, to run repeatedly:
  //s.sendAll();
  scheduler.execute();

}
