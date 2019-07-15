#ifndef PRESENCE_H
#define PRESENCE_H

#include "Sense.h"
#include <ITransport.h>

class Presence: public Sense
{
private:
    int pin;
protected:
    int getDataString(char* result){ 
        int val = digitalRead(pin);
        sprintf(result, "%d", val);
        return 0;
    }
    bool hasDataToSend(){
        return true;
    }

public:
    SETUPARGCOMMAND(setEvent,Presence);

    Presence(int _pin,const char * name,ITransport* transport): 
                            Sense(transport, name),
                            setEventCommand(*this){
        commands.addPointer(&setEventCommand);
        pin = _pin;
    }

    int setEvent(const char* d, char* result){
        Serial.println(d);
        return 0;
    }
};

#endif 