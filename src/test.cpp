#define _TASK_LTS_POINTER
#define DEBUG_INITIAL_LEVEL 5
#define DEBUG_MINIMUM true
//#include "SerialDebug.h"

#include <Arduino.h>
#include <TaskScheduler.h>

class Trigger{
private:
  char* name;
  int pin;
  bool lastState;
  bool currentState;
  bool change;

public:
    Trigger(int _pin,const char * _name, bool pullup = true){ 
        pin = _pin;
        name = _name;
        if(pullup)
            pinMode(pin, INPUT_PULLUP);
        currentState = digitalRead(pin);
        lastState = currentState;
    }
        
    void loop(){                
      currentState = digitalRead(pin);
      if(lastState != currentState){
        Serial.print(name);
        Serial.print(":");
        Serial.println(currentState);
        lastState = currentState;
      }
    }
};

const int NUM_OF_SENSES = 13;
Trigger shl(22,"shl",false);
Trigger vkl(24,"vkl");
Trigger bol(26,"bol");
Trigger bor(28,"bor");
Trigger alb(30,"alb");
Trigger bin(32,"bin",false);

//Trigger vol(34,"vol");

Trigger rin(36,"rin");

Trigger box(38,"box");
Trigger tel(40,"tel");
Trigger fot(42,"fot");
Trigger kom(44,"kom",false);
Trigger lif(46,"lif");
Trigger fan(48,"fan");

Trigger* senses[NUM_OF_SENSES] = {&shl,&vkl,&bol,&bor,&alb,&bin,&rin,&box,&kom,&tel,&lif,&fot,&fan};


//Rotation rad(A2,"rad",&transport);


/*
#include "Presence.h"
#include "Rotation.h"
#include "Debug.h"
#include "Button.h"
#include "GPIOSwitch.h"
#include "Encoder.h"

#include "SerialTransport.h"
#include "ExternalCommands.h"

Scheduler scheduler;
SerialTransport transport(Serial); 

const int NUM_OF_SENSES = 16;
Button shl(22,"shl",&transport,false);
Button vkl(24,"vkl",&transport);
Button bol(26,"bol",&transport);
Button bor(28,"bor",&transport);
Button alb(30,"alb",&transport);
Button bin(32,"bin",&transport,false);

Encoder vol(34,"vol",&transport);

Button rin(36,"rin",&transport);

Button box(38,"box",&transport);
Button bil(40,"bil",&transport);
Button tel(42,"tel",&transport);
Button lif(44,"lif",&transport);
Button fot(46,"fot",&transport);
Button plc(44,"plc",&transport);

Rotation rad(A2,"rad",&transport);


GPIOName GPIOS[] = {
  {23,"lshl"}, {25,"lvkl"}, {27,"lbo"}, {31,"lalb"}, {33,"lbin"},
  {35,"lvol"}, {37,"lrin"}, {39,"lbox"}, {41,"lbil"}, {43,"ltel"},
  {45,"llif"}, {47,"lfot"}, {49,"lplc"},
  
  {51,"lrad"},
  {53,"lscr"},
};

GPIOSwitch gpio("outs",GPIOS,15);

Commandable* senses[NUM_OF_SENSES] = {&shl,&vkl,&bol,&bor,&alb,&bin,&vol,&rin,&box,&bil,&tel,&lif,&fot,&plc,&rad,&gpio};

ExternalCommands disp("cmd",&transport,senses,NUM_OF_SENSES);
*/
Scheduler scheduler;

bool prevVal = 1;

void cb(){
  bool val = digitalRead(34) ;
        if(val != prevVal){
          Serial.println("vol:1");
          prevVal = val;
        }
}

Task taskGetData(10, TASK_FOREVER, &cb, &scheduler, true);


void setup() {
  Serial.begin(57600);
  char buf[1] ;
  //vol.dtread("10","");
  //vol.dt("10","");
  //disp.start(buf);
  //disp.runall(buf);

  Serial.println("sys:1");
}

void loop() {
	//debugHandle();
  for (int i=0 ;i<NUM_OF_SENSES;i++)
    senses[i]->loop();
  // put your main code here, to run repeatedly:
  //s.sendAll();
  scheduler.execute();

}
