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

typedef struct iptStruct {
    PID proccessID;
    LPN pageNumber;
    bool dirtyBit; //Whenever the content of a page is changed, this bit is set (flagged true). Whenever a page is loaded to MM or saved to Disk, this bit is cleared (flagged false).
    bool referenceBit; //Whenever a page is accessed (read or write), this bit is set (flagged true). The Aging Algorithm Daemon may read / change it.
    struct iptStruct *next; // pointing to another entry in the inverted page table.
    struct iptStruct *prev;
} IPT_t,*IPT_t_p;

typedef char* Page;
typedef IPT_t_p* HAT_t; //HAT is just an array of IPT enteries

typedef Page* Disk_t;
typedef Page* MM_t;

volatile unsigned int MaxNumOfProcesses;
volatile unsigned int PageSize;
volatile unsigned int NumOfPagesInMM;
volatile unsigned int NumOfPagesInDisk;
volatile unsigned int NumOfProcessPages;
volatile unsigned int ShiftClock;


typedef struct pcbStruct {
    bool Active;
    PID proccessID;
    unsigned int start; //Start indices
    unsigned int end; //end indices
} PCB_t,*PCB_t_p;

typedef struct mmDiskMapStruct
{

} MMToDiskMap_t,*MMToDiskMap_t_p;

typedef struct freeListStruct
{
    bool isFree;
} FreeList_t,*FreeList_t_p;

typedef struct PRM_RequestQueueValueStruct
{
    unsigned int pageNumber;
    unsigned int ProccessID;

} PRM_RequestQueueValue_t,*PRM_RequestQueueValue_tp;

#endif	/* _DATATYPES_H */

