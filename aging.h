/* 
 * File:   aging.h
 * Author: danni
 *
 * Created on June 4, 2010, 11:06 PM
 */

#ifndef _AGING_H
#define	_AGING_H

#include "globaldefs.h"
#include "datatypes.h"
#include <semaphore.h>
#include "mm.h"
#include "pcb.h"
#include "ipt.h"

static volatile bool AGING_ShouldClose = FALSE;
pthread_t Aging;
sem_t Aging_mutex;

unsigned int* Aging_Registers;

bool AGING_Init();
void AGING_Close();
void AGING_DeInit();
void* AGING_Main();

#endif	/* _AGING_H */

