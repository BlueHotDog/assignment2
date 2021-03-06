/* 
 * File:   mm.h
 * Author: yanivdu
 *
 * Created on May 24, 2010, 7:56 PM
 */

#ifndef _MM_H
#define	_MM_H

#include "datatypes.h"
#include "globaldefs.h"
#include "aging.h"

static MM_t     MM;
pthread_mutex_t MM_Counter_Mutex;
static int      MM_Access_Counter;
static int      MM_Hit_Counter;
//static int      MM_Miss_Counter;

bool MM_Init();

Page MM_ReadPage(int pageNum);
void MM_WritePage(Page data,int pageNum, int bitsToWrite, int startingFrom, int dirtyBit);
void MM_MemoryReference();
void MM_Hit();
//void MM_Miss();

void MM_DeInit();

#endif	/* _MM_H */

