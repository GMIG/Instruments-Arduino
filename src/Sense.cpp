
#include "Sense.h"

//const char Sense::cmdErr[] PROGMEM = "command error";
//char Sense::temp[20];

//const char* Sense::startClassCommand::_name = "start";

void sendCallback(){
    Task& T = scheduler.currentTask();
    Sense& V = *((Sense*) T.getLtsPointer()); 
    V.manageTimerTask();
}

