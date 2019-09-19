#define _TASK_LTS_POINTER
#define DEBUG_INITIAL_LEVEL 5
#define DEBUG_MINIMUM true
//#include "SerialDebug.h"

#include <Arduino.h>
#include <TaskScheduler.h>
#include "GPIOSwitch.h"
#include "ExternalCommands.h"
#include "SerialTransport.h"
#include "Encoder.h"
#include "Trigger.h"
#include "Rotation.h"

//#define TESTUNO 1
#define TEST 1



const int NUM_OF_SENSES = 13;
Trigger shl(22,"shl",false); Trigger vkl(24,"vkl");      
Trigger bol(26,"bol");       Trigger bor(28,"bor");
Trigger alb(30,"alb");       Trigger bin(32,"bin",false);
#ifdef TEST
  Trigger rin(A3,"rin");
#else
  Trigger rin(36,"rin");
#endif
Trigger box(38,"box"); Trigger tel(40,"tel"); 
Trigger fot(42,"fot"); Trigger kom(23,"kom",false);
Trigger lif(25,"lif"); Trigger fan(27,"fan");

Trigger* senses[NUM_OF_SENSES] = {&shl,&vkl,&bol,&bor,&alb,&bin,&rin,&box,&kom,&tel,&lif,&fot,&fan};

//2 - 13, 44 - 46
GPIOName GPIOS[] = {
    {2,"shl"}, {3,"vkl"},
    {4,"bo"}, 
    {5,"alb"}, {6,"bin"},
    {7,"rin"},
    {8,"box"}, {9,"tel"},
    {10,"fot"}, {11,"kom"},
    {12,"lif"}, {13,"fan"},  
    {44,"vol"}, {45,"sig"},  
    {46,"rad"}
};
Scheduler scheduler;

GPIOSwitch gpio("outs",GPIOS,15);
SerialTransport transport(Serial); 

Encoder vol(A0,"vol",&transport);
Rotation rad(A1,"rad",&transport);

Commandable* coms[3] = {&gpio,&vol,&rad};//,&r,&r3,&r4,&b2,&b3,&s,&r}; 
ExternalCommands disp("cmd",&transport,coms,3);


bool prevVal = 1;

  #ifndef TEST
    int volPin = 34;
  #else
    int volPin = A0;
  #endif
void setup() {
  Serial.begin(57600);
  Serial.println("sys:1");
}

void loop() {
  #ifndef TESTUNO
  for (int i=0 ;i < NUM_OF_SENSES;i++)
    senses[i]->loop();
  #else
    rin.loop();
  #endif
  scheduler.execute();

}
