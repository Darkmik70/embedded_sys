/*
 * File:   circular_buffer.h
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#ifndef CIRCULAR_BUFFER_H
#define	CIRCULAR_BUFFER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define BUFFER_SIZE  128


typedef struct {
    char buffer[BUFFER_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int size;
} CircularBuffer;


/**
 * Initialize CircularBuffer
 * set tail and head to 0, and set BUFFER_SIZE as the size of buffer
 */
void init_circular_buffer(CircularBuffer *cb);

/**
 *  Checks whether CircularBuffer is Full
 * @param cb CircularBuffer
 * @return  1 when incremented position of head equals tthe tail, that will mean
 * the buffer is full. Otherwise return 0. This also  wraps around the buffer.
 */
int is_circular_buffer_full(CircularBuffer *cb);

/**
 * Checks if buffer is empty
 * @param cb CircularBuffer
 * @return 1 when head equals tails, otherwise zero.
 * if head equals tails, this means there is no unread data in the buffer, thus
 * we consider the buffer empty
 */
int is_circular_buffer_empty(CircularBuffer *cb);

/**
 * Write data to circual buffer.
 * 
 * Checks if the buffer is full, if this is not the case, it writes new data to
 * buffer and incerements the head, Modulo operation goes for wrapping the buffer
 * in circle.
 * 
 * @param cb CircularBuffer
 * @param data char to be written to Buffer
 * @return 0 if buffer is full, otherwise 1.
 */
int write_circular_buffer(CircularBuffer *cb, char data);

/**
 * Read data from Circular Buffer.
 * 
 * This function does not remove any data. It checks if the data is empty, and
 * then if not, reads one character from the buffer, 
 * and incerements the count of tail. Tail wraps around the buffer.
 * 
 * @param cb  CircularBuffer
 * @param *data  pointer to the read character
 * @return 1 if operation went successfull, 0 when buffer is empty
 */
int read_circular_buffer(CircularBuffer *cb, char *data);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CIRCULAR_BUFFER */

