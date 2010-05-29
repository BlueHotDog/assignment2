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

bool MMU_Init();
void MMU_Close();

Page MMU_ReadAddress(MemoryAddress_t address);
bool MMU_WriteAddress(MemoryAddress_t address,Page* value);


#endif	/* _MMU_H */

