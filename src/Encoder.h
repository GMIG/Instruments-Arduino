#ifndef ENCODER_H
#define ENCODER_H

#include "Sense.h"

//#include "SerialDebug.h"

void encoderGetCallback();

class Encoder: public Sense
{
private:
    int pin;
    Task taskGetData;
    bool oldValue;
    bool newValue;

protected:
    int getDataString(char * result){ 
        sprintf(result, "%d", 1);
        return 0;
    }
    bool hasDataToSend(){
		bool ret = oldValue != newValue;
		oldValue = newValue;
        return ret;
    }

    SETUPARGCOMMAND(dtread,Encoder);

public:
    Encoder(int _pin,const char * name,ITransport* transport): 
                            Sense(transport, name),
                            taskGetData(5, TASK_FOREVER, &encoderGetCallback, &scheduler, true),
                            dtreadCommand(*this)
                            {
        pin = _pin;
        commands.addPointer(&dtreadCommand);
        pinMode(pin, INPUT);
        taskGetData.setLtsPointer(this);
        oldValue = 0;    
        newValue = 0; 

    }

    int dtread(const char* arg, char * result){
        char *err;
        unsigned int d = strtoul(arg, &err, 10);
        if (*err != 0 ) 
            return 1; 
        taskGetData.setInterval(d);
        return 0;
    }


    void readValue(){
		newValue = digitalRead(pin);
    }


};

#endif
