#ifndef GPIOSWITCH_H
#define GPIOSWITCH_H
#define _TASK_LTS_POINTER

//#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerDeclarations.h>
#include <Arduino.h>

#include "Commandable.h"


#define O0 11
#define O1 10
#define O2 9
#define O3 6
#define O4 5
#define O5 3
#define MAX_GPIOS
#define ON 255

extern Scheduler scheduler;


ptrdiff_t index_of1(char *string, char search);

void faderCallback();

struct GPIOName{
    const uint8_t GPIOnum;
    const char* GPIOname;
};

class GPIOSwitch: public Commandable{

public:
    const GPIOName* GPIOs;
    uint8_t num_of_GPIO;
    Task fader;
    int* brig;
    int* dBrig;

    SETUPARGCOMMAND(allOn,GPIOSwitch);
    SETUPARGCOMMAND(allOff,GPIOSwitch);
    SETUPARGCOMMAND(allSet,GPIOSwitch);
    SETUPARGCOMMAND(allFad,GPIOSwitch);
    SETUPARGCOMMAND(set,GPIOSwitch);
    SETUPARGCOMMAND(setFad,GPIOSwitch);

    SETUPARGCOMMAND(on,GPIOSwitch);
    SETUPARGCOMMAND(off,GPIOSwitch);

    GPIOSwitch(const char* name, GPIOName* _GPIOs, uint8_t _num_of_GPIO ):
                                        Commandable(name),
                                        GPIOs(_GPIOs),
                                        fader(20, TASK_FOREVER, &faderCallback, &scheduler, true),
                                        num_of_GPIO(_num_of_GPIO),
                                        INITCOMMAND(on),
                                        INITCOMMAND(off),
                                        INITCOMMAND(allOn),
                                        INITCOMMAND(allOff),
                                        INITCOMMAND(set),
                                        INITCOMMAND(allSet),
                                        INITCOMMAND(allFad),
                                        INITCOMMAND(setFad)
                                        {   
        ADDCOMMAND(set);
        ADDCOMMAND(on);
        ADDCOMMAND(off);
        ADDCOMMAND(allOn);
        ADDCOMMAND(allOff);
        ADDCOMMAND(allSet);
        ADDCOMMAND(allFad);
                ADDCOMMAND(setFad);

        fader.setLtsPointer(this);
        brig = calloc(num_of_GPIO, sizeof(int)); 
        dBrig= calloc(num_of_GPIO, sizeof(int)); 
        for (size_t i = 0; i < num_of_GPIO; i++)
            pinMode(GPIOs[i].GPIOnum, OUTPUT);
    }

    uint8_t getI(const char* name){
        for (size_t i = 0; i < num_of_GPIO; i++)
            if(strcmp(GPIOs[i].GPIOname,name) == 0)
                return i;
        return 255;
    }


    uint8_t getNum(const char* name){
        for (size_t i = 0; i < num_of_GPIO; i++)
            if(strcmp(GPIOs[i].GPIOname,name) == 0)
                return GPIOs[i].GPIOnum;
        return 255;
    }

    int allSwitch(uint8_t on){
        for (size_t i = 0; i < num_of_GPIO; i++){
            brig[i] = on;
            digitalWrite(GPIOs[i].GPIOnum,on);
        }
        return 0;
    }

    int switcher(const char* arg, uint8_t on){
        uint8_t d = getNum(arg);
        if (d == 255) 
            return 1; 
        brig[getI(arg)] = on;
        digitalWrite(d,on);
        return 0;
    }


    int on(const char* arg, char * result){
        return switcher(arg, ON);    
    }

    char name[10];
    char val[5];
    int set(const char* arg, char * result){
        int commaPosition = index_of1(arg,',');
        memset(name, '\0', 10);
        strncpy(name, arg, commaPosition);
        memset(val, '\0', 5);
        strncpy(val, arg + commaPosition + 1, strlen(arg) - commaPosition - 1);
        Serial.println(name);
        uint8_t pin = getNum(name);
        if (pin == 255) 
            return 1; 
        char *err;
        unsigned int d = strtoul(val, &err, 10);
        if (*err != 0 ) 
            return 1; 
        brig[getI(arg)] = d;
        analogWrite(pin,d);
        return 0;    
    }
    
    //char val2[5];   
    int setFad(const char* arg, char * result){
        int commaPosition = index_of1(arg,',');
        memset(name, '\0', 10);
        strncpy(name, arg, commaPosition);
        memset(val, '\0', 5);
        strncpy(val, arg + commaPosition + 1, strlen(arg) - commaPosition - 1);

        uint8_t i = getI(name);
        if (i == 255) 
            return 1; 
        char *err;
        unsigned int d = strtoul(val, &err, 10);
        if (*err != 0 ) 
            return 1; 
        dBrig[i] = d;
        return 0;    
    }
    
    int allFad(const char* arg, char * result){
        int res = 0;
        char *err;
        int d = strtoul(arg, &err, 10);
        if (*err != 0 ) 
            return 1; 
        for (size_t i = 0; i < num_of_GPIO; i++){
            dBrig[i] = d;
        }
        return 0;    
    }


    

    int off(const char* arg, char * result){
        return switcher(arg, LOW);    
    }

    int allOn(const char* arg, char * result){
        return allSwitch(ON);    
    }

    int allOff(const char* arg, char * result){
        return allSwitch(LOW);    
    }

    int allSet(const char* arg, char * result){
        int res = 0;
        char *err;

        unsigned int d = strtoul(arg, &err, 10);
        if (*err != 0 ) 
            return 1; 
        for (size_t i = 0; i < num_of_GPIO; i++){
            brig[i] = d;
            analogWrite(GPIOs[i].GPIOnum,d);
        }
        return res;
    }



};


#endif
