#ifndef SENSE_H
#define SENSE_H
#define _TASK_LTS_POINTER

#include "PointerStorage.h"


#include <TaskSchedulerDeclarations.h>
#include <Arduino.h>

#include <ITransport.h>
#include "Command.h"

//#include "SerialDebug.h"
#define COMMAND_NOT_FOUND 10

#define MAX_SENSE_RESULT MAX_COMMAND_RESULT + 40
//https://godbolt.org/ -E compiler parameter
#define SETUPCOMMAND(cmdname, parentclass) \
    class  cmdname##ClassCommand: public  Command { \
        public:\
            cmdname##ClassCommand(parentclass& _parent): parent(_parent){};\
            const char* name() {\
                return #cmdname;\
            };\
        private:\
            int exec0(const char* s,  char* result){ \
                return parent.cmdname(result); \
            }; \
            parentclass& parent;\
    } cmdname##Command;




#define SETUPARGCOMMAND(cmdname, parentclass) \
    class  cmdname##ClassCommand: public  Command { \
        public:\
            cmdname##ClassCommand(parentclass & _parent): parent(_parent){};\
            const char* name() {\
                return #cmdname;\
            };\
        private:\
            int exec0(const char* s,  char* result){ \
                return parent.cmdname(s,result); \
            }; \
            parentclass & parent;\
    } cmdname##Command;\

extern Scheduler scheduler;


void sendCallback();


class Sense
{
protected: 
    virtual int getDataString( char * ) = 0;
    virtual bool hasDataToSend() = 0;

protected:
    PointerStorage<Command> commands;  
private:
    const char* _name;
    PointerStorage<ITransport> transports;
    Task taskSendData;
    //static const char* unknownCommand;
    //static const char* commandFail;

public:
    const char* namec(){
        return this->_name;
    }
    SETUPCOMMAND(stop,Sense);
    SETUPCOMMAND(start,Sense);
    SETUPCOMMAND(is,Sense);
    SETUPARGCOMMAND(dt,Sense);
    SETUPARGCOMMAND(list,Sense);

    Sense(ITransport* transport, const char *__name):
                _name(__name),
                taskSendData(50, TASK_FOREVER, &sendCallback, &scheduler, false),
                startCommand(*this),
                stopCommand(*this),
                isCommand(*this),
                dtCommand(*this),
                listCommand(*this)

        {
        transports.addPointer(transport);
        commands.addPointer(&stopCommand);
        commands.addPointer(&startCommand);
        commands.addPointer(&isCommand);
        commands.addPointer(&dtCommand);
        commands.addPointer(&listCommand);
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

    int list(const char* arg, char * result){
        for(short i = 0; i < commands.getSize() ; i++){
            const char* senseCmd = commands.getPointer(i)->name();
            strlcat(result,senseCmd,MAX_COMMAND_RESULT);
            strlcat(result," ",MAX_COMMAND_RESULT);

        }
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

    int decodeCommand(const char* cmd,const char* arg, char* result){
        for(short i = 0; i < commands.getSize() ; i++){
            const char* senseCmd = commands.getPointer(i)->name();
            if (strcmp(cmd,senseCmd) == 0){
                return commands.getPointer(i)->exec(arg,result);
            }
        }
        return COMMAND_NOT_FOUND;
    }
};





#endif