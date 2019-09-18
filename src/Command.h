#ifndef COMMAND_H
#define COMMAND_H

#define MAX_COMMAND_RESULT 20

class Command
{
public:
    virtual const char* name() = 0;
    virtual int exec(const char* arg,  char* result){
        return exec0(arg, result);
    }
protected:
private:
    virtual int exec0(const char*,  char* result) = 0;
};


#endif 
