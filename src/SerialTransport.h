#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H
#include "ITransport.h"

#include <Arduino.h>


class SerialTransport: public ITransport{
private:
    HardwareSerial& serialDriver;

public:
    SerialTransport(HardwareSerial& _serialDriver): serialDriver(_serialDriver){
        _serialDriver.begin(9600);
    }

    int sendString(const char* s){
        return serialDriver.print(s);
    }


    int sendLine(const char* s){
        return serialDriver.println(s);
    }


    const int receiveChar(){
        return serialDriver.read();
    }

    String receiveString(){
        return serialDriver.readString();
    }


    const int available(){
        return serialDriver.available();
    }

};

#endif 