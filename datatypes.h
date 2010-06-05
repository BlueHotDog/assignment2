/* 
 * File:   datatypes.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 6:02 PM
 */

#ifndef _DATATYPES_H
#define	_DATATYPES_H

#include "globaldefs.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define MAX_MESSAGE_SIZE 50
#define MAX_MESSAGES_IN_QUEUE 10


//bool monitor = FALSE;

FILE* outFile;
FILE* inFile;

#define MONITOR(expr...)  ((monitor==TRUE)?fprintf(outFile,expr): __ASSERT_VOID_CAST (0))
typedef unsigned int PID;   //process id
typedef unsigned int LPN;   //logical page number
typedef unsigned int MMFI;  //MM frame index
typedef unsigned int DPI;   //MM disk page index

typedef struct iptStruct {
    PID processID;
    LPN pageNumber;
    MMFI frame;
    bool dirtyBit; //Whenever the content of a page is changed, this bit is set (flagged true). Whenever a page is loaded to MM or saved to Disk, this bit is cleared (flagged false).
    bool referenceBit; //Whenever a page is accessed (read or write), this bit is set (flagged true). The Aging Algorithm Daemon may read / change it.
    struct iptStruct* next; // pointing to another entry in the inverted page table.
    struct iptStruct* prev;
} IPT_t, *IPT_t_p;


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




//=====================QUEUE====================
enum Commands {
    ProcessReadAddress,
    ProcessWriteToAddress,
    ProcessClose,
    PRMReadAddress,
    PRMWriteToAddress,
    PRMSegmentationFault,
    PRMSegmentationFaultMMIsFull
};
typedef struct queueCommandStruct {
    enum Commands command;
    int* params; //params array
    char** stringParams;
    void** voidParams;
    int paramsAmount;
    int stringParamsAmount;
    int voidParamsAmount;
} QueueCommand_t,*QueueCommand_t_p;

typedef struct queueItemStruct {
    QueueCommand_t_p command;
    struct queueItemStruct* next;
} QueueItem_t, *QueueItem_t_p;

typedef struct queueStruct {
    QueueItem_t_p head; //reading for the head
} Queue_t,*Queue_t_p;
//================================================


typedef struct pcbStruct {
    pthread_t processThread;
    bool active;
    PID processID;
    unsigned int start; //Start indices
    unsigned int end; //end indices
    Queue_t_p processQueue;
} PCB_t, *PCB_t_p;

typedef struct mmDiskMapStruct {
} MMToDiskMap_t, *MMToDiskMap_t_p;

typedef struct freeListStruct {
    bool isFree;
} FreeList_t, *FreeList_t_p;

typedef struct memoryAddressStruct {
    unsigned int pageNumber;
    unsigned int processID;
} MemoryAddress_t, *MemoryAddress_t_p;

#endif	/* _DATATYPES_H */

