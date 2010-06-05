/* 
 * File:   process.h
 * Author: danni
 *
 * Created on May 23, 2010, 10:06 PM
 */

#ifndef _PROCESS_H
#define	_PROCESS_H
#include "globaldefs.h"
#include "datatypes.h"
#include "messagequeues.h"
#include "pcb.h"
#include "disk.h"

static volatile bool PROCESS_ShouldClose = FALSE;
void* PROCESS_RUN(void* pcb);
int PROCESS_CREATE();
//void PROCESS_STOP();
bool PROCESS_Read(PID processID,int vAddr, int amount);
void PROCESS_DeInit(PID id);
#endif	/* _PROCESS_H */

