#ifndef ROTATION_H
#define ROTATION_H

#include "Sense.h"

//#include "SerialDebug.h"

void rotationGetCallback();

class Rotation: public Sense
{
private:
    int pin;
    Task taskGetData;
    int oldVal;
    int newVal;
    float alphaval;
    int threshold;

protected:
    int getDataString(char * result){ 
        short out = newVal;
        if (threshold)
            out = newVal > threshold;

        sprintf(result, "%d", out);
        return 0;
    }
    bool hasDataToSend(){
        bool res = newVal != oldVal;
        oldVal = newVal;
        return res;
    }

    SETUPARGCOMMAND(alpha,Rotation);
    SETUPARGCOMMAND(thr,Rotation);

public:
    Rotation(int _pin,const char * name,ITransport* transport): 
                            Sense(transport, name),
                            taskGetData(10, TASK_FOREVER, &rotationGetCallback, &scheduler, true),
                            alphaCommand(*this),
                            thrCommand(*this)
                            {
        pin = _pin;
        commands.addPointer(&alphaCommand);
        commands.addPointer(&thrCommand);
        alphaval = 0.1f;
        taskGetData.setLtsPointer(this);
        threshold = 0;  
        oldVal = analogRead(pin);
        newVal = oldVal;   

    }

    int thr(const char*  arg,char* result){
        char *err;
        int d = strtod(arg, &err);
        if (*err != 0 ) 
            return 1; 
        threshold = d;
        return 0;
    }

    int alpha(const char*  arg, char* result){
        char *err;
        float d = strtod(arg, &err);
        if (*err != 0 ) 
            return 1; 
        alphaval = d;
        return 0;
    }
    
    void readValue(){
        newVal = (1-alphaval)*analogRead(pin) + alphaval*oldVal;
    }


};

#endif
