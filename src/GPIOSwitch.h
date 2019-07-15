#ifndef GPIOSWITCH_H
#define GPIOSWITCH_H

#include "Commandable.h"

#define O0 11
#define O1 10
#define O2 9
#define O3 6
#define O4 5
#define O5 3
#define MAX_GPIOS



struct GPIOName{
    const uint8_t GPIOnum;
    const char* GPIOname;
};

class GPIOSwitch: public Commandable{
    const GPIOName* GPIOs;
    uint8_t num_of_GPIO;
public:
    SETUPARGCOMMAND(allOn,GPIOSwitch);
    SETUPARGCOMMAND(allOff,GPIOSwitch);

    SETUPARGCOMMAND(on,GPIOSwitch);
    SETUPARGCOMMAND(off,GPIOSwitch);

    GPIOSwitch(const char* name, GPIOName* _GPIOs, uint8_t _num_of_GPIO ):
                                        Commandable(name),
                                        GPIOs(_GPIOs),
                                        num_of_GPIO(_num_of_GPIO),
                                        INITCOMMAND(on),
                                        INITCOMMAND(off),
                                        INITCOMMAND(allOn),
                                        INITCOMMAND(allOff){   
        ADDCOMMAND(on);
        ADDCOMMAND(off);
        ADDCOMMAND(allOn);
        ADDCOMMAND(allOff);
        for (size_t i = 0; i < num_of_GPIO; i++)
            pinMode(GPIOs[i].GPIOnum, INPUT);
    }

    uint8_t getNum(const char* name){
        for (size_t i = 0; i < num_of_GPIO; i++)
            if(strcmp(GPIOs[i].GPIOname,name) == 0)
                return GPIOs[i].GPIOnum;
        return 255;
    }

    int allSwitch(uint8_t on){
        for (size_t i = 0; i < num_of_GPIO; i++)
            digitalWrite(GPIOs[i].GPIOnum,on);
        return 0;
    }

    int switcher(const char* arg, uint8_t on){
        uint8_t d = getNum(arg);
        if (d == 255) 
            return 1; 
        digitalWrite(d,on);
        return 0;
    }


    int on(const char* arg, char * result){
        return switcher(arg, HIGH);    
    }

    int off(const char* arg, char * result){
        return switcher(arg, LOW);    
    }

    int allOn(const char* arg, char * result){
        return allSwitch(HIGH);    
    }

    int allOff(const char* arg, char * result){
        return allSwitch(LOW);    
    }



};


#endif