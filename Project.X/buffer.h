#ifndef BUFFER_H
#define	BUFFER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define BUFFER_SIZE 6 

typedef struct {
    char data[BUFFER_SIZE];
    int head; // Index read
    int tail; // Index write
    int count; // Number of characters written
} CircularBuffer;

void initBuffer(CircularBuffer *uartBuffer);

int is_circular_buffer_full(CircularBuffer *cb);

int is_circular_buffer_empty(CircularBuffer *cb);


void addToBuffer(CircularBuffer *uartBuffer, char data);
int readBuffer(char buffer[], int length);
int readByte();


#endif	/* BUFFER_H */

