#ifndef COMMANDDISPATCHER_H
#define COMMANDDISPATCHER_H
#define _TASK_LTS_POINTER


#include <TaskSchedulerDeclarations.h>
#include <Arduino.h>
#include <ITransport.h>
#include <Sense.h>

#define MAX_SYNTAX 5
//#include "SerialDebug.h"

void ExternalCommandsCallback();



class ExternalCommands: public Sense
{
private:
    Task taskGetData;
    ITransport* transport;
    bool hasDataToSendvar = false;
    PointerStorage<Commandable> senses;
    
    const char syntax[MAX_SYNTAX]={'[',']','.','(',')'};

protected:
    int getDataString( char* result){ 

        return readTransport( result);
    }
    bool hasDataToSend(){
        return transport->available() > 0;
    }
    SETUPCOMMAND(runall,ExternalCommands);
    SETUPCOMMAND(stopall,ExternalCommands);
    SETUPARGCOMMAND(getObj,ExternalCommands);
    SETUPCOMMAND(getNumObj,ExternalCommands);


public:
    ExternalCommands(const char * name,ITransport* _transport, Commandable** _senses, const int numOfSenses): 
                                                                Sense(_transport, name),
                                                                //taskGetData(20, TASK_FOREVER, &ExternalCommandsCallback, &scheduler, true),
                                                                transport(_transport),
                                                                INITCOMMAND(runall),
                                                                INITCOMMAND(stopall),
                                                                INITCOMMAND(getObj),
                                                                INITCOMMAND(getNumObj)
                                                                {
        ADDCOMMAND(runall);
        ADDCOMMAND(stopall);
        ADDCOMMAND(getObj);
        ADDCOMMAND(getNumObj);
        senses.addPointer(this);

        for(int i = 0;i < numOfSenses;i++){
            senses.addPointer(_senses[i]);
        }
        //taskGetData.setLtsPointer(this);
    }
    
    int getNumObj (char * result){
        sprintf(result, "%d", senses.getSize());
    }

    int runall(char * result){
        for (int i = 0; i < senses.getSize() ;i++) {
            char a[1];char r[1];
            senses.getPointer(i)->decodeCommand("start", a,r);
        }
        return 0;
    }

    int getObj(const char * arg,char * result){
        char *err;
        unsigned int d = strtoul(arg, &err, 10);
        if (*err != 0 ) 
            return 1; 
        if  ( d >= senses.getSize()) 
            return 2; 
        strlcat(result,senses.getPointer(d)->namec(),MAX_SENSE_RESULT);
        return 0;
    }


    int stopall(char * result){
        for (int i = 0; i < senses.getSize() ;i++) {
            char a[1];char r[1];
            senses.getPointer(i)->decodeCommand("stop", a,r);
        }
        return 0;
    }

    int readTransport( char* result);
};

#endif 
