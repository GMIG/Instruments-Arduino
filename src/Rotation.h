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
    int value1;
    bool changed;
    float alphaval;
    int threshold;

protected:
    int getDataString(char * result){ 
        short out = value1;
        if (threshold)
            out = value1 > threshold;

        sprintf(result, "%d", out);
        return 0;
    }
    bool hasDataToSend(){
        return changed;
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
        changed = false;
        alphaval = 0.1f;
        taskGetData.setLtsPointer(this);
        threshold = 0;        

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
        int filtered = (1-alphaval)*analogRead(pin) + alphaval*value1;

        if(filtered != value1){
            changed = true;
            value1 = filtered;
        }
        else
            changed = false;


    }


};

#endif