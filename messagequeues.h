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

mqd_t QUEUE_Create(string name);
mqd_t QUEUE_CreateForProcess(PID processID);

mqd_t QUEUE_Open(string name);
mqd_t QUEUE_OpenForProcess(PID processID);

int QUEUE_Send(mqd_t queue,string text);

string QUEUE_Read(mqd_t queue);

string QUEUE_GetProcessQueueName(PID processID);

int QUEUE_Close(mqd_t queue);
#endif	/* _MESSAGEQUEUES_H */

