#ifndef ITRANSPORT_H
#define ITRANSPORT_H

#include <Arduino.h>
class ITransport
{
public:
    virtual int sendString(const char* s) = 0;
    virtual int sendLine(const char* s) = 0;

    virtual const int receiveChar() = 0;
    virtual char* receiveString() = 0;

    virtual const int available() = 0;

};



#endif 
