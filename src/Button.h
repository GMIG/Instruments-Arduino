#ifndef BUTTON_H
#define BUTTON_H

#include "Sense.h"
#include <ITransport.h>

class Button: public Sense
{
private:
    int pin;
    bool lastState;
    bool currentState;

protected:
    int getDataString(char* result){ 
        sprintf(result, "%d", currentState);
        lastState = currentState;
        return 0;
    }
    bool hasDataToSend(){
        currentState = digitalRead(pin);
        return (lastState != currentState);
    }

public:
    Button(int _pin,const char * name,ITransport* transport): 
                            Sense(transport, name){
        pin = _pin;
        pinMode(pin, INPUT_PULLUP);
        currentState = digitalRead(pin);
        lastState = currentState;
    }
};

#endif 