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


int pullFromBuffer() {
    IEC0bits.U1RXIE = 0x00; // desable UART interruption
    int c = -1;
    if (uartBufferRx.count > 0) {
        c = uartBufferRx.buffer[uartBufferRx.head];
        uartBufferRx.head = (uartBufferRx.head + 1) % BUFFER_SIZE;
        uartBufferRx.count--;
    }
    IEC0bits.U1RXIE = 0x01; // enable UART interruption
    return c;
}

int readBuffer(char buffer[], int length) {
    int count = 0;
    if (length <= 0) {
        return 0;
    }
    while (length > 0) {
        int c = pullFromBuffer();
        if (c != -1) {
            buffer[count++] = c;
            length--;
        }
    }
    return count;
}

int readByte() {
    return pullFromBuffer();
}
