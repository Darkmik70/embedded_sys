#ifndef BUFFER_H
#define	BUFFER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define BUFFER_SIZE 64 

typedef struct {
    char buffer[BUFFER_SIZE];
    int head; // Index read
    int tail; // Index write
    int count; // Number of characters written
} CircularBuffer;

void initBuffer(CircularBuffer *uartBuffer);

int is_circular_buffer_full(CircularBuffer *cb);

int is_circular_buffer_empty(CircularBuffer *cb);

int write_circular_buffer(CircularBuffer *cb, char data);

int read_circular_buffer(CircularBuffer *cb, char *data);

void addToBuffer(CircularBuffer *uartBuffer, char data);
int readBuffer(char buffer[], int length);
int readByte();


#endif	/* BUFFER_H */

