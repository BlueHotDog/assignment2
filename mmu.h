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

static bool MMU_shouldClose = FALSE;

pthread_t MMU;



bool MMU_Init();
void MMU_Close();
void* MMU_Main();

#endif	/* _MMU_H */

