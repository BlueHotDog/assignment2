/* 
 * File:   freelist.h
 * Author: danni
 *
 * Created on May 23, 2010, 1:10 AM
 */

#ifndef _FREELIST_H
#define	_FREELIST_H

#include "datatypes.h"
//an array which says if a page in the disk is taken or not, thus each FreeList_t i
//in the array represent the i page in the Disk.
FreeList_t_p FreeList;

//should be called only by the Disk_init routine, first sets all to not taken
bool FREELIST_Init();
void FREELIST_DeAllocate();
//returns the start of an untaken page, or -1 if none;
int FREELIST_Get();
void FREELIST_SetTaken(unsigned int index);
#endif	/* _FREELIST_H */

