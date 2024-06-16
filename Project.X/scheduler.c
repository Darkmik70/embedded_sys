/*
 * File:   scheduler.c
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
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
