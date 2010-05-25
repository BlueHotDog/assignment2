#include "messagequeues.h"

QueueCommand_t_p QUEUES_ReadProcess(PID processID) //blocking if no messages
{
    QueueCommand_t_p queueCommand;
    pthread_mutex_lock(&ProcessWriter[processID]);

    //if there aren't any messages to read, wait.
    if(ProcessQueues[processID]->head == NULL)
        pthread_mutex_lock(&ProcessReader[processID]);

    //else of when a message arrived
    QueueItem_t_p pointer = ProcessQueues[processID]->head;
    ProcessQueues[processID]->head = pointer->next;
    queueCommand = pointer->command;

    pthread_mutex_unlock(&ProcessWriter[processID]);
    return queueCommand;
}

QueueCommand_t_p QUEUES_ReadMMU() //blocking if no messages
{
    QueueCommand_t_p queueCommand;
    pthread_mutex_lock(&MMUWriter);

    //if there aren't any messages to read, wait.
    if(MMUQueue->head == NULL)
        pthread_mutex_lock(&MMUReader);

    //else of when a message arrived
    QueueItem_t_p pointer = MMUQueue->head;
    MMUQueue->head = pointer->next;
    queueCommand = pointer->command;

    pthread_mutex_unlock(&MMUWriter);
    return queueCommand;
}

QueueCommand_t_p QUEUES_ReadPRM() //blocking if no messages
{
    QueueCommand_t_p queueCommand;
    pthread_mutex_lock(&PRMWriter);
    
    //if there aren't any messages to read, wait.
    if(PRMQueue->head == NULL)
        pthread_mutex_lock(&PRMReader);
    
    //else of when a message arrived
    QueueItem_t_p pointer = PRMQueue->head;
    PRMQueue->head = pointer->next;
    queueCommand = pointer->command;
   
    pthread_mutex_unlock(&PRMWriter);
    return queueCommand;
}