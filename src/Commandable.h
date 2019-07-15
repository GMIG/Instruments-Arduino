#ifndef COMMANDABLE_H
#define COMMANDABLE_H

#include "Command.h"

#include "PointerStorage.h"
#include <string.h>


#define COMMAND_NOT_FOUND 10

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

#define INITCOMMAND(cmdname) \
    cmdname##Command(*this)\

#define ADDCOMMAND(cmdname) \
        commands.addPointer(&cmdname##Command);\


class Commandable
{
protected:
    PointerStorage<Command> commands;  
    const char* _name;

public:
    const char* namec(){
        return this->_name;
    }

    SETUPARGCOMMAND(list,Commandable);

    int decodeCommand(const char* cmd,const char* arg, char* result){
        for(short i = 0; i < commands.getSize() ; i++){
            const char* senseCmd = commands.getPointer(i)->name();
            if (strcmp(cmd,senseCmd) == 0){
                return commands.getPointer(i)->exec(arg,result);
            }
        }
        return COMMAND_NOT_FOUND;
    }
    int list(const char* arg, char * result){
        for(short i = 0; i < commands.getSize() ; i++){
            const char* senseCmd = commands.getPointer(i)->name();
            strlcat(result,senseCmd,MAX_COMMAND_RESULT);
            strlcat(result," ",MAX_COMMAND_RESULT);

        }
        return 0;
    }

    Commandable(const char *__name):_name(__name),
                    INITCOMMAND(list){
        ADDCOMMAND(list);
    };

};



#endif