#ifndef COMMANDDISPATCHER_H
#define COMMANDDISPATCHER_H
#define _TASK_LTS_POINTER


#include <TaskSchedulerDeclarations.h>
#include <Arduino.h>
#include <ITransport.h>
#include <Sense.h>


//#include "SerialDebug.h"

void ExternalCommandsCallback();



class ExternalCommands: public Sense
{
private:
    Task taskGetData;
    ITransport* transport;
    bool hasDataToSendvar = false;
    PointerStorage<Sense> senses;

protected:
    int getDataString( char* result){ 
        return readTransport( result);
    }
    bool hasDataToSend(){
        return transport->available() > 0;
    }
    SETUPCOMMAND(runall,ExternalCommands);

public:
    ExternalCommands(const char * name,ITransport* _transport, Sense** _senses, const int numOfSenses): 
                                                                Sense(_transport, name),
                                                                //taskGetData(20, TASK_FOREVER, &ExternalCommandsCallback, &scheduler, true),
                                                                transport(_transport),
                                                                runallCommand(*this){
        commands.addPointer(&runallCommand);

         for(int i = 0;i < numOfSenses;i++)
            senses.addPointer(_senses[i]);
        senses.addPointer(this);
        //taskGetData.setLtsPointer(this);
    }
    
    int runall(char * result){
        for (int i = 0; i < senses.getSize() ;i++) {
            senses.getPointer(i)->start("");
        }
        return 0;
    }

    int readTransport( char* result);
};

#endif 