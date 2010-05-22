/* 
 * File:   pcb.h
 * Author: yanivdu
 *
 * Created on May 22, 2010, 8:12 PM
 */

#ifndef _PCB_H
#define	_PCB_H
#include "globaldefs.h"

bool PCB_Init();
bool PCB_AllocateProccess(PID id,int start,int end);
bool PCB_DeAllocateProccess(PID id);
//Returns the start address of the free space on disk, -1 if failure
int PCB_SearchForFreeSpace();

#endif	/* _PCB_H */

