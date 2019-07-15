#ifndef SENSE_H
#define SENSE_H
#define _TASK_LTS_POINTER

#include "PointerStorage.h"


#include <TaskSchedulerDeclarations.h>
#include <Arduino.h>

#include <ITransport.h>
#include "Command.h"
#include "Commandable.h"

#define COMMAND_NOT_FOUND 10
#define MAX_SENSE_RESULT MAX_COMMAND_RESULT + 40

extern Scheduler scheduler;

void sendCallback();

class Sense : public Commandable
{
protected: 
    virtual int getDataString( char * ) = 0;
    virtual bool hasDataToSend() = 0;

private:
    PointerStorage<ITransport> transports;
    Task taskSendData;
public:
    SETUPCOMMAND(stop,Sense);
    SETUPCOMMAND(start,Sense);
    SETUPCOMMAND(is,Sense);
    SETUPARGCOMMAND(dt,Sense);

    Sense(ITransport* transport, const char *__name):
                Commandable(__name),
                taskSendData(50, TASK_FOREVER, &sendCallback, &scheduler, false),
                startCommand(*this),
                stopCommand(*this),
                isCommand(*this),
                dtCommand(*this)
        {
        transports.addPointer(transport);
        commands.addPointer(&stopCommand);
        commands.addPointer(&startCommand);
        commands.addPointer(&isCommand);
        commands.addPointer(&dtCommand);
        taskSendData.setLtsPointer(this);
    }

    void manageTimerTask(){
        if(hasDataToSend())
            for (short i = 0; i < transports.getSize(); i++){ 
                ITransport* tr = transports.getPointer(i);
                tr->sendString(_name);
                tr->sendString(":");
                char result[MAX_SENSE_RESULT];
                memset(result, '\0', sizeof(result));
                getDataString(result);
                tr->sendLine((char*)result);

            }
    }
    int dt(const char* arg, char * result){
        char *err;
        unsigned int d = strtoul(arg, &err, 10);
        if (*err != 0 ) 
            return 1; 
        taskSendData.setInterval(d);
        return 0;
    }



    int is(char * result){
        sprintf(result,"%d", taskSendData.isEnabled());        
        return 0;
    }

    int stop(char * result){
        return !taskSendData.disable();
    }

    int start(char * result){
        taskSendData.enable();
        return 0;
    }

};





#endif