
#include "ExternalCommands.h"

#define DEF_EMPTY_STRING(name, size) char name[size];memset(name, '\0', size);

const char synErr[] PROGMEM = "syntax error:";
const char unknCmd[] PROGMEM = "unknown command:";
const char cmdError[] PROGMEM = "command error:";
const char unknSense[] PROGMEM = "unknown sense:";
const char ok[] PROGMEM = "ok";
const char par[] PROGMEM = ":";


ptrdiff_t index_of(char *string, char search) {
    const char *moved_string = strchr(string, search);
    /* If not null, return the difference. */
    if (moved_string) {
        return moved_string - string;
    }
    /* Character not found. */
    return -1;
}


void ExternalCommandsCallback(){
    Task& T = scheduler.currentTask();
    ExternalCommands* disp = ((ExternalCommands*) T.getLtsPointer());
    //disp->readTransport();
}
int ExternalCommands::readTransport(char* result){

        if (transport->available() > 0) {

            char* cmdStr = transport->receiveString();
            //String cmdStr(cmdStr1);
            //int leftBracketPosition = cmdStr.indexOf('[');
            int leftBracketPosition = index_of(cmdStr,'[');
            
            //int rightBracketPosition = cmdStr.indexOf(']');
            int rightBracketPosition = index_of(cmdStr,']');

            //int colonPosition = cmdStr.indexOf('.');
            int colonPosition = index_of(cmdStr,'.');

            //int parOpenPosition = cmdStr.indexOf('(');
            int parOpenPosition = index_of(cmdStr,'(');

            //int parClosePosition = cmdStr.indexOf(')');
            int parClosePosition = index_of(cmdStr,')');

            int no = -1;
            
            if ( colonPosition == no || parOpenPosition == no || parClosePosition == no ||  // any of the required fields is missing
                    (leftBracketPosition == no && rightBracketPosition != no) ||            // only right bracket
                    (leftBracketPosition != no && rightBracketPosition == no)) {           // only left bracket
                strlcpy_P(result, synErr,MAX_SENSE_RESULT);
                strlcat(result, cmdStr,MAX_SENSE_RESULT);
                return 1; // error
            }

            DEF_EMPTY_STRING(addrString,7)
            
            size_t begin = 0;
            if ( rightBracketPosition != no && leftBracketPosition != no){
                strncpy(addrString, cmdStr + leftBracketPosition , rightBracketPosition - leftBracketPosition +1);
                //addrString = cmdStr.substring(leftBracketPosition,rightBracketPosition + 1);
                strlcpy(result,addrString,MAX_SENSE_RESULT);
                begin = rightBracketPosition+1;
            }            
            //String senseString = cmdStr.substring(begin,colonPosition);
            DEF_EMPTY_STRING(senseString,7);
            strncpy(senseString, cmdStr + begin, colonPosition - begin);
            
            //String commandString = cmdStr.substring(colonPosition + 1,parOpenPosition);
            DEF_EMPTY_STRING(commandString,10);
            strncpy(commandString, cmdStr + colonPosition + 1, parOpenPosition - colonPosition - 1);

            
            //String paramString = cmdStr.substring(parOpenPosition + 1,parClosePosition);
            DEF_EMPTY_STRING(paramString,10);
            strncpy(paramString, cmdStr + parOpenPosition + 1, parClosePosition - parOpenPosition - 1);
            
            /*Serial.println(addrString);
            Serial.println(senseString);
            Serial.println(commandString);
            Serial.println(paramString);
*/
            
            // blocking here
            for (int i = 0; i < senses.getSize() ;i++) {
                Commandable* s = senses.getPointer(i);
                if(strcmp(s->namec(),senseString) == 0){
                    char commandResult[MAX_COMMAND_RESULT];
                    memset(commandResult, '\0', MAX_COMMAND_RESULT);
                    int error = s->decodeCommand(commandString,paramString,commandResult);
                    if(error == COMMAND_NOT_FOUND){
                        strlcat_P(result, unknCmd,MAX_SENSE_RESULT);
                        strlcat(result,commandString,MAX_SENSE_RESULT);
                        return COMMAND_NOT_FOUND;
                    }
                    else if (error != 0) {
                        strlcat_P(result,cmdError,MAX_SENSE_RESULT);
                        strlcat(result,commandString,MAX_SENSE_RESULT);
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
            strlcat(result,senseString,MAX_SENSE_RESULT);
            return 3;
        }
        return 6;
    }
