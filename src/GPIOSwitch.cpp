#include "GPIOSwitch.h"

void faderCallback(){
    Task& T = scheduler.currentTask();
    GPIOSwitch& V = *((GPIOSwitch*) T.getLtsPointer()); 
    for (size_t i = 0; i < V.num_of_GPIO; i++)
		if(V.dBrig[i] != 0 ) 
			/*if(V.brig[i] != 0 && V.brig[i] != 255)*/{
				short newVal = V.brig[i] + V.dBrig[i];
				if(newVal <= 0){
					newVal = 0;
				}
				else if(newVal >= 255){
					newVal = 255;
				}
				if(V.brig[i] != newVal)
					analogWrite(V.GPIOs[i].GPIOnum,newVal);
				V.brig[i] = newVal;
  
			}  
}
ptrdiff_t index_of1(char *string, char search){
    const char *moved_string = strchr(string, search);
    /* If not null, return the difference. */
    if (moved_string) {
        return moved_string - string;
    }
    /* Character not found. */
    return -1;
};
