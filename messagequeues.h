/* 
 * File:   messagequeues.h
 * Author: danni
 *
 * Created on May 24, 2010, 9:06 PM
 */

#ifndef _MESSAGEQUEUES_H
#define	_MESSAGEQUEUES_H
#include "globaldefs.h"
#include "datatypes.h"
#include <semaphore.h>

#define WAIT_FOR_PRM(pid) (sem_wait(&PROCESSES_mutex[1][pid]))
#define DONE_WITH_PRM(pid) (sem_post(&PROCESSES_mutex[1][pid]))

int BufferSize;
sem_t** PROCESSES_mutex;                // Controls access to critical section
sem_t* PROCESSES_empty;                 // counts number of empty buffer slots
sem_t* PROCESSES_full;                  // counts number of full buffer slots
sem_t PRM_mutex;                        // Controls access to critical section
sem_t PRM_empty;                        // counts number of empty buffer slots
sem_t PRM_full;                         // counts number of full buffer slots

//Queues declaration
Queue_t_p* ProcessQueues;

Queue_t_p MMUQueue;
Queue_t_p PRMQueue;


/*
 * Responsible for initing the queues for all the threads - memory allocation and setting to null
 */
bool QUEUES_Init();

bool QUEUES_WriteToProcess(PID processID,QueueCommand_t_p command); //non blocking 
bool QUEUES_WriteToMMU(QueueCommand_t_p command); //non blocking
bool QUEUES_WriteToPRM(QueueCommand_t_p command); //non blocking

QueueCommand_t_p QUEUES_ReadProcess(PID processID); //blocking if no messages
QueueCommand_t_p QUEUES_ReadMMU(); //blocking if no messages
QueueCommand_t_p QUEUES_ReadPRM(); //blocking if no messages

QueueItem_t_p QUEUES_GetLastItem(Queue_t_p queue);

void QUEUES_PrintCommand(QueueCommand_t_p command);

#endif	/* _MESSAGEQUEUES_H */

