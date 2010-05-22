/* 
 * File:   pcb.h
 * Author: yanivdu
 *
 * Created on May 22, 2010, 8:12 PM
 */

#ifndef _PCB_H
#define	_PCB_H
#include "globaldefs.h"
#include "datatypes.h"

PCB_t_p PCBArray;

bool PCB_Init();
PCB_t PCB_AllocateProccess(PID id,int start,int end);
bool PCB_DeAllocateProccess(PID id);
int PCB_GetFreeProcessID();

#endif	/* _PCB_H */

