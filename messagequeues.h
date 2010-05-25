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

//Readers declarations
pthread_mutex_t* ProcessReader; //array of mutex for the reading operation index correlates to process id, i.e reading will lock on this.
pthread_mutex_t  MMUReader;
pthread_mutex_t  PRMReader;

//Writers declaration
pthread_mutex_t* ProcessWriter;
pthread_mutex_t MMUWriter;
pthread_mutex_t PRMWriter;

//Queues declaration
Queue_t_p* ProcessQueues;
Queue_t_p  MMUQueue;
Queue_t_p  PRMQueue;


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

#endif	/* _MESSAGEQUEUES_H */

