#ifndef SONAR_H
#define SONAR_H

#include "Commandable.h"


class Sonar :public Sense{
private:
  int pinTrig;
  int pinEcho;
  int lastDuration;
  int curDuration;
  int thr = 80;
    int getDataString(char * result){ 
        sprintf(result, "%d", curDuration - lastDuration);
        lastDuration = curDuration;
        return 0;
    }
    bool hasDataToSend(){
		digitalWrite(pinTrig, LOW); 
		delayMicroseconds(2); 
		digitalWrite(pinTrig, HIGH);
		delayMicroseconds(10); 
		digitalWrite(pinTrig, LOW); 
		curDuration = pulseIn(pinEcho, HIGH, 2000); 
		return (abs(curDuration - lastDuration) > thr);
    }

public:
    Sonar(int _pinTrig, int _pinEcho,const char * _name, ITransport* transport, bool pullup = true):
		Sense(transport,_name){ 
			pinTrig = _pinTrig;
			pinEcho = _pinEcho;
			pinMode(pinTrig, OUTPUT); 
			pinMode(pinEcho, INPUT); 
			char r[5];
			dt("100",r);
    }
        
};

#endif
