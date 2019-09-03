#include "Encoder.h"



void encoderGetCallback(){
    Task& T = scheduler.currentTask();
    Encoder& V = *((Encoder*) T.getLtsPointer()); 
    V.readValue();
}
