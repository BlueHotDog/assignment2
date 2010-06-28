/* 
 * File:   mmu.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 7:09 PM
 */

#ifndef _MMU_H
#define	_MMU_H
#include "globaldefs.h"
#include "datatypes.h"
#include "messagequeues.h"
#include "hat.h"
#include "readerswriters.h"
#include "mm.h"

//#include "ipt.h"

static bool MMU_shouldClose = FALSE;

pthread_t MMU;
pthread_mutex_t MMU_mutex_helper;

bool MMU_Init();
void MMU_Close();

char MMU_ReadAddress(MemoryAddress_t address, int bitIndex);
bool MMU_WriteToAddress(MemoryAddress_t address,Page value, int bitsToWrite, int startingFrom);


#endif	/* _MMU_H */

