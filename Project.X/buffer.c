/*
 * File:   buffer.c
 */
#include "buffer.h"
#include "function.h"

CircularBuffer uartBufferRx;
CircularBuffer uartBufferTx;



void initBuffer(CircularBuffer *uartBuffer) {
    uartBuffer->head = 0;
    uartBuffer->tail = 0;
    uartBuffer->count = BUFFER_SIZE;
}


int is_circular_buffer_full(CircularBuffer *cb){
    return((cb->head+1) % cb->count)== cb->tail;
}

int is_circular_buffer_empty(CircularBuffer *cb){
    return cb->head == cb->tail;
}

int write_circular_buffer(CircularBuffer *cb, char data){
    if(is_circular_buffer_full(cb)){
        return -1;
    }
    else{
        cb->buffer[cb->head] = data;
        cb->head = (cb->head +1)% cb->count;    // increment la testa
        return 0;   // operazione riuscita
    }
}


int read_circular_buffer(CircularBuffer *cb, char *data){
    if(is_circular_buffer_empty(cb)){
        return -1;
    }
    
    *data = cb ->buffer[cb->tail];  // legge il dato dalla posizione della coda
    cb->tail = (cb->tail + 1) % cb->count;
    return 0;
}



