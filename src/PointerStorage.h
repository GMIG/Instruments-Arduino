#ifndef POINTER_STORAGE_H
#define POINTER_STORAGE_H

#define MAX_CAPACITY 10
#define INITIAL_CAPACITY 1
#include <stdlib.h>

template <typename T> 
class PointerStorage { 
    short size = 0;
    T** pointers;
public:
    PointerStorage(){
        pointers = (T**) malloc(INITIAL_CAPACITY * sizeof(void*));
    
    }
    const short getSize (){
        return size;
    }
    
    T* getPointer(short i){
        return pointers[i];
    }

    int addPointer(T* ptr){
        if(size + 1 > MAX_CAPACITY)
            return 1;
        if(size >= INITIAL_CAPACITY)
            pointers = (T**) realloc(pointers, (INITIAL_CAPACITY + size)*sizeof(void*));
        pointers[size] = ptr;
        size ++;

        return 0;
    }

};

#endif 
