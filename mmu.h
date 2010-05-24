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

static bool MMU_shouldClose = FALSE;

pthread_t MMU;



bool MMU_Create();
void MMU_Close();
void* MMU_Main();

#endif	/* _MMU_H */

