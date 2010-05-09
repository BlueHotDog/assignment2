/* 
 * File:   datatypes.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 6:02 PM
 */

#ifndef _DATATYPES_H
#define	_DATATYPES_H
#include "globaldefs.h"

typedef unsigned int PID; //proccess id
typedef unsigned int LPN; //logical page number

const volatile unsigned int PageSize;
const volatile unsigned int NumOfPagesInMM;
const volatile unsigned int NumOfPagesInDisk;
const volatile unsigned int NumOfProcessPages;

typedef struct iptStruct {
    PID proccessID;
    LPN pageNumber;
    bool dirtyBit; //Whenever the content of a page is changed, this bit is set (flagged true). Whenever a page is loaded to MM or saved to Disk, this bit is cleared (flagged false).
    bool referenceBit; //Whenever a page is accessed (read or write), this bit is set (flagged true). The Aging Algorithm Daemon may read / change it.
    struct iptStruct *next; // pointing to another entry in the inverted page table.
    struct iptStruct *prev;
} IPT_t,*IPT_t_p;


typedef struct pcbStruct {
    PID proccessID;

} PCB_t,*PCB_t_p;

typedef struct mmDiskMapStruct
{

} MMToDiskMap_t,*MMToDiskMap_t_p;

typedef struct freeListStruct
{
    
} FreeList_t,*FreeList_t_p;

#endif	/* _DATATYPES_H */

