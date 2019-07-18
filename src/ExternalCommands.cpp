#include "ExternalCommands.h"
const char synErr[] PROGMEM = "syntax error:";
const char unknCmd[] PROGMEM = "unknown command:";
const char cmdError[] PROGMEM = "command error:";
const char unknSense[] PROGMEM = "unknown sense:";
const char ok[] PROGMEM = "ok";
const char par[] PROGMEM = ":";


void ExternalCommandsCallback(){
    Task& T = scheduler.currentTask();
    ExternalCommands* disp = ((ExternalCommands*) T.getLtsPointer());
    //disp->readTransport();
}
int ExternalCommands::readTransport(char* result){

        if (transport->available() > 0) {
            String cmdStr = transport->receiveString();
         
            int leftBracketPosition = cmdStr.indexOf('[');
            int rightBracketPosition = cmdStr.indexOf(']');

            int colonPosition = cmdStr.indexOf('.');
            int parOpenPosition = cmdStr.indexOf('(');
            int parClosePosition = cmdStr.indexOf(')');
            int no = -1;
            
            if ( colonPosition == no || parOpenPosition == no || parClosePosition == no ||  // any of the required fields is missing
                    (leftBracketPosition == no && rightBracketPosition != no) ||            // only right bracket
                    (leftBracketPosition != no && rightBracketPosition == no)) {           // only left bracket
                strlcpy_P(result, synErr,MAX_SENSE_RESULT);
                strlcat(result, cmdStr.c_str(),MAX_SENSE_RESULT);
                return 1; // error
            }

            String addrString;
            size_t begin = 0;
            if ( rightBracketPosition != no && leftBracketPosition != no){
                addrString = cmdStr.substring(leftBracketPosition,rightBracketPosition + 1);
                strlcpy(result,addrString.c_str(),MAX_SENSE_RESULT);
                begin = rightBracketPosition+1;
            }            
            String senseString = cmdStr.substring(begin,colonPosition);
            String commandString = cmdStr.substring(colonPosition + 1,parOpenPosition);
            String paramString = cmdStr.substring(parOpenPosition + 1,parClosePosition);
            // blocking here
            for (int i = 0; i < senses.getSize() ;i++) {
                Commandable* s = senses.getPointer(i);
                if(strcmp(s->namec(),senseString.c_str()) == 0){
                    char commandResult[MAX_COMMAND_RESULT];
                    memset(commandResult, '\0', MAX_COMMAND_RESULT);
                    int error = s->decodeCommand(commandString.c_str(),paramString.c_str(),commandResult);
                    if(error == COMMAND_NOT_FOUND){
                        strlcat_P(result, unknCmd,MAX_SENSE_RESULT);
                        strlcat(result,commandString.c_str(),MAX_SENSE_RESULT);
                        return COMMAND_NOT_FOUND;
                    }
                    else if (error != 0) {
                        strlcat_P(result,cmdError,MAX_SENSE_RESULT);
                        strlcat(result,commandString.c_str(),MAX_SENSE_RESULT);
                        return 0;
                    }
                    else{
                        strlcat_P(result,ok,MAX_SENSE_RESULT);
                        if(commandResult[0] != '\0'){
                            strlcat_P(result,par,MAX_SENSE_RESULT);
                            strlcat(result,commandResult,MAX_SENSE_RESULT);
                        }
                        return 0;
                    }
                }
            }
            strlcat_P(result,unknSense,MAX_SENSE_RESULT);
            strlcat(result,senseString.c_str(),MAX_SENSE_RESULT);
            return 3;
        }
        return 6;
    }
