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

static volatile bool PROCESS_ShouldClose = FALSE;
void* PROCESS_RUN(void* pcb);
int PROCESS_CREATE();
void PROCESS_STOP();
#endif	/* _PROCESS_H */

