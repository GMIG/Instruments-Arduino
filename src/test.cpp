#define _TASK_LTS_POINTER
#define DEBUG_INITIAL_LEVEL 5
#define DEBUG_MINIMUM true

#include <Arduino.h>
#include <TaskScheduler.h>
#include "ExternalCommands.h"
#include "SerialTransport.h"
#include "Encoder.h"
#include "Trigger.h"
#include "Sonar.h"

#include "Rotation.h"
#include "GPIOSwitch.h"

//#define TEST 1

SerialTransport transport(Serial); 
Scheduler scheduler;

#ifdef TEST
  const int NUM_OF_SENSES = 5;
  Encoder vol(7,"vol",&transport);
  Trigger rin(5,"rin",&transport);
  Rotation rad(A0,"rad",&transport);
  Trigger vkl(4,"vkl",&transport);    
  const int NUM_OF_GPIOS = 2;
  GPIOName GPIOS[NUM_OF_GPIOS] = {
    {2,"vol"}, {3,"rin"}};
  GPIOSwitch gpio("outs",GPIOS,NUM_OF_GPIOS);

  Commandable* senses[NUM_OF_SENSES] = {&vol,&rad,&rin,&vkl,&gpio};

#else
  const int NUM_OF_SENSES = 16;
  Sonar shl(22,36,"shl",&transport,false); 
  Trigger vkl(23,"vkl",&transport);      
  Trigger bol(24,"bol",&transport);       Trigger bor(25,"bor",&transport);
  Trigger alb(26,"alb",&transport);       Trigger bin(27,"bin",&transport,false);
  Encoder vol(28,"vol",&transport);
  Trigger rin(29,"rin",&transport);
  Rotation rad(A15,"rad",&transport);

  Trigger box(30,"box",&transport); Trigger tel(31,"tel",&transport); 
  Trigger fot(32,"fot",&transport); Sonar kom(33,38,"kom",&transport,false);
  Trigger lif(34,"lif",&transport); Trigger fan(35,"fan",&transport);
  const int NUM_OF_GPIOS = 15;
  GPIOName GPIOS[NUM_OF_GPIOS] = {
    {2,"shl"}, {3,"vkl"},
    {4,"bo"}, 
    {5,"alb"}, {6,"bin"},
    {7,"vol"},
    {8,"rin"}, {9,"box"},
    {10,"rad"}, {11,"tel"},
    {12,"fot"}, {13,"kom"},  
    {44,"lif"}, {45,"fan"},  
    {46,"sig"}
  };

  GPIOSwitch gpio("outs",GPIOS,NUM_OF_GPIOS);
  Commandable* senses[NUM_OF_SENSES] = {&shl,&vkl,&bol,&bor,&alb,&bin,&rin,&box,&kom,&tel,&lif,&fot,&fan,&gpio,&vol,&rad};
#endif

ExternalCommands disp("cmd",&transport,senses,NUM_OF_SENSES);

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("sys:1");
//          for(int i = 0;i < NUM_OF_SENSES + 1;i++){
//           Serial.println(senses[i]->namec());
//        }

}

void loop() {
  scheduler.execute();

}
