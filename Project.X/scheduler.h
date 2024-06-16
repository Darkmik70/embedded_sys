/*
 * File:   scheduler.h
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include <xc.h> // include processor files - each processor file is guarded.  

typedef struct {
    int n;
    int N;
    int enable;
    void (*f)(void *);
    void* params;
} heartbeat;

void scheduler(heartbeat schedInfo[], int nTasks);

#endif	/* SCHEDULER_H */
