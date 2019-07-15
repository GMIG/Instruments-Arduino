#include "Rotation.h"



void rotationGetCallback(){
    Task& T = scheduler.currentTask();
    Rotation& V = *((Rotation*) T.getLtsPointer()); 
    V.readValue();
}
