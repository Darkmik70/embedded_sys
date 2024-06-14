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




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CIRCULAR_BUFFER */

