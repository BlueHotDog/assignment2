/* 
 * File:   disk.h
 * Author: yanivdu
 *
 * Created on May 22, 2010, 7:45 PM
 */

#ifndef _DISK_H
#define	_DISK_H

#include "datatypes.h"
#include "globaldefs.h"
#include "freelist.h"

static Disk_t Disk;

//initializes the disk data-type
//return true on success, false otherwise
bool DISK_Init();
//Findes a free space in the Disk using the PCB and allocate the space
bool DISK_AllocateSpace(unsigned int start,unsigned int end);
//Removes the relevant PCB block
bool DISK_DeAllocateSpace(unsigned int start,unsigned int end);
//returns the Page to the PRM
void DISK_ReadPage(int pageNum, OUT Page* pageToReturn);

//Writes the data to the pageNumber returns true if all went well, false otherwise.
bool DISK_WritePage(Page data,int pageNum);
void DISK_PrintContent();
void DISK_DeInit();

#endif	/* _DISK_H */

