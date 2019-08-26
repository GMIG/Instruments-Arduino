#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H
#include "ITransport.h"

#include <Arduino.h>

#define MAX_COMMAND_SIZE 30

class SerialTransport: public ITransport{
private:
    HardwareSerial& serialDriver;
    char commandBuffer[MAX_COMMAND_SIZE];
public:
    SerialTransport(HardwareSerial& _serialDriver): serialDriver(_serialDriver){
        serialDriver.begin(57600);
        serialDriver.setTimeout(100);
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
    

    char* receiveString(){
        /*Serial.print("SIZE:");
        Serial.println(serialDriver.available());
        char c = serialDriver.read();
        int i =0;
        commandBuffer[i] = c;
        int startMillis = millis();
        while (c >= 0 && c != '\n' && i < MAX_COMMAND_SIZE - 1 && (millis() - startMillis) < 100)
        {   
            i++;
            c = serialDriver.read();
            commandBuffer[i] = c;
            Serial.println(c);
        }
        
        commandBuffer[i+1] = '\0';*/
        
        serialDriver.readBytesUntil('\n',commandBuffer,MAX_COMMAND_SIZE);
        return commandBuffer;
    }


    const int available(){
        int av = serialDriver.available();
        return av;
    }

};

#endif 
