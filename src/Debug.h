#ifndef DEBUG_H
#define DEBUG_H

#include "Sense.h"
#include <ITransport.h>

#include "Command.h"

class Debug: public Sense
{
private:
    SETUPARGCOMMAND(setVerbo,Debug);
protected:
    int getDataString(char* result){ 
        return 0;
    }
    bool hasDataToSend(){
        return false;
    }

public:
    Debug(const char * name,ITransport* transport): 
                            Sense(transport, name),
                            setVerboCommand(*this){
        commands.addPointer(&setVerboCommand);
    }

    int setVerbo(const char* v, char* result){
        //debugSetLevel (std::atoi(v.c_str()));
        return 0;
    }
};

#endif 