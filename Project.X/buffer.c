/*
 * File:   buffer.c
 */
#include "buffer.h"

CircularBuffer uartBufferRx;
CircularBuffer uartBufferTx;

void initCircularBuffer(CircularBuffer *uartBuffer) {
    uartBuffer->head = 0;
    uartBuffer->tail = 0;
    uartBuffer->count = 0;
}

void addToBuffer(CircularBuffer *uartBuffer, char data) {
    if (uartBuffer->count < BUFFER_SIZE) {
        uartBuffer->data[uartBuffer->tail] = data;
        uartBuffer->tail = (uartBuffer->tail + 1) % BUFFER_SIZE;
        uartBuffer->count++;
    } else {
        // Buffer full, next received character will not be read
    }
}

void stringToBuffer(CircularBuffer *uartBuffer, char data[], int length) {
    if (uartBuffer->count < BUFFER_SIZE) {
        for (int i = 0; i < length; i++) {
            uartBuffer->data[uartBuffer->tail] = data[i];
            uartBuffer->tail = (uartBuffer->tail + 1) % BUFFER_SIZE;
            uartBuffer->count++;
        }
    } else {
        // Buffer full, next received character will not be read
    }
}

int pullFromBuffer() {
    IEC0bits.U1RXIE = 0x00; // desable UART interruption
    int c = -1;
    if (uartBufferRx.count > 0) {
        c = uartBufferRx.data[uartBufferRx.head];
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
    int msg;
    msg = U1RXREG;
    return msg;
    //return pullFromBuffer();
}
