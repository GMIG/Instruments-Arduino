#ifndef TRIGGER_H
#define TRIGGER_H


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

#endif
