#ifndef ROTATION_H
#define ROTATION_H

#include "Sense.h"

//#include "SerialDebug.h"

#define AV_WIN_SIZE 30

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
    int stepVal;
    float window[AV_WIN_SIZE];

protected:
    int getDataString(char * result){ 
        short out = newVal;
        if (threshold)
            out = newVal > threshold;

        sprintf(result, "%d", out);
        oldVal = newVal;

        return 0;
    }
    bool hasDataToSend(){
        bool res = (abs(newVal - oldVal) > stepVal);
        return res;
    }

    SETUPARGCOMMAND(alpha,Rotation);
    SETUPARGCOMMAND(thr,Rotation);
    SETUPARGCOMMAND(step,Rotation);



public:
    Rotation(int _pin,const char * name,ITransport* transport): 
                            Sense(transport, name),
                            taskGetData(20, TASK_FOREVER, &rotationGetCallback, &scheduler, true),
                            alphaCommand(*this),
                            INITCOMMAND(step),
                            thrCommand(*this)
                            {
        ADDCOMMAND(step);
        pin = _pin;
        commands.addPointer(&alphaCommand);
        commands.addPointer(&thrCommand);
        alphaval = 0.0f;
        taskGetData.setLtsPointer(this);
        threshold = 0;  
        stepVal = 3;
        oldVal = analogRead(pin);
        newVal = oldVal;   
        for (int i=0;i<AV_WIN_SIZE;i++){
            window[i] = newVal;
        }

    }

    int thr(const char*  arg,char* result){
        char *err;
        int d = strtod(arg, &err);
        if (*err != 0 ) 
            return 1; 
        threshold = d;
        return 0;
    }
    
        
    int step(const char*  arg,char* result){
        char *err;
        int d = strtod(arg, &err);
        if (*err != 0 ) 
            return 1; 
        stepVal = d;
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
        int readVal = analogRead(pin);
        float sum = 0;
        for (int i= AV_WIN_SIZE - 1;i >= 0;i--){
            sum += window[i];
            window[i+1] = window[i];
        }
        window[0] = readVal;
        sum += readVal;
        float av = sum/((float) AV_WIN_SIZE);
        newVal = roundf(av);
    }


};

#endif
