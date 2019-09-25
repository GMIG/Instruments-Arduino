#ifndef TRIGGER_H
#define TRIGGER_H

#include "Commandable.h"


class Trigger :public Sense{
private:
  //char* name;
  int pin;
  bool lastState;
  bool currentState;
  bool change;
    int getDataString(char * result){ 
        sprintf(result, "%d", currentState);
        lastState = currentState;
        return 0;
    }
    bool hasDataToSend(){
		currentState = digitalRead(pin);
        return lastState != currentState;
    }

public:
    Trigger(int _pin,const char * _name, ITransport* transport, bool pullup = true):
		Sense(transport,_name){ 
			pin = _pin;
			if(pullup)
				pinMode(pin, INPUT_PULLUP);
			currentState = digitalRead(pin);
			lastState = currentState;
    }
        
    void loop(){                
      currentState = digitalRead(pin);
      if(lastState != currentState){
        Serial.print(_name);
        Serial.print(":");
        Serial.println(currentState);
        lastState = currentState;
      }
    }
};

#endif
