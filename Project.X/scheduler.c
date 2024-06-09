/*
 * File:   scheduler.c
 * Author: RomiC
 *
 * Created on 9 giugno 2024, 22.17
 */


#include "xc.h"
#include "scheduler.h"

void scheduler(heartbeat schedInfo[], int nTasks) 
{
    int i;
    for (i = 0; i < nTasks; i++) {
        schedInfo[i].n++;
        if (schedInfo[i].enable == 1 && schedInfo[i].n >= schedInfo[i].N) {
            schedInfo[i].f(schedInfo[i].params);            
            schedInfo[i].n = 0;
        }
    }
}