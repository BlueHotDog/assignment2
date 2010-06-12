#include "messagequeues.h"
#include "pcb.h"
#include "prm.h"

QueueItem_t_p QUEUES_GetLastItem(Queue_t_p queue) {
    //ASSERT_PRINT("Entering:QUEUES_GetLastItem\n");
    QueueItem_t_p toReturn = NULL;
    ASSERT(queue != NULL);
    Queue_t temp = *queue;
    if (temp.head == NULL)
        return NULL;
    toReturn = temp.head;
    while (toReturn->next != NULL) {
        toReturn = toReturn->next;
    }
    //ASSERT_PRINT("Exiting:QUEUES_GetLastItem\n");
    return toReturn;
}

void QUEUES_PrintCommand(QueueCommand_t_p command) {
    fprintf(outFile, "command:");
    switch (command->command) {
        case PRMReadAddress:
            fprintf(outFile, "PRMReadAddress");
            break;
        case PRMWriteToAddress:
            fprintf(outFile, "PRMWriteToAddress");
            break;
    }
    fprintf(outFile, "\n");
    fprintf(outFile, "paramsAmount:%d\n", command->paramsAmount);
    fprintf(outFile, "params:\n");
    int i = 0;
    for (i = 0; i < command->paramsAmount; i++)
        fprintf(outFile, "%d\n", command->params[i]);
}

bool QUEUES_Init() {
    BufferSize = 0x200;

    PROCESSES_mutex = calloc(2, sizeof (sem_t*));

    PROCESSES_mutex[0] = calloc(MaxNumOfProcesses, sizeof (sem_t));
    PROCESSES_empty = calloc(MaxNumOfProcesses, sizeof (sem_t));
    PROCESSES_full = calloc(MaxNumOfProcesses, sizeof (sem_t));

    PROCESSES_mutex[1] = calloc(MaxNumOfProcesses, sizeof (sem_t));


    int i;
    for (i = 0; i < MaxNumOfProcesses; i++) {
        sem_init(&PROCESSES_mutex[0][i], 0, 1); // Controls access to critical section
        sem_init(&PROCESSES_empty[i], 0, BufferSize); // counts number of empty buffer slots
        sem_init(&PROCESSES_full[i], 0, 0); // counts number of full buffer slots
    }

    for (i = 0; i < MaxNumOfProcesses; i++)
        sem_init(&PROCESSES_mutex[1][i], 0, 0);

    sem_init(&PRM_mutex, 0, 1); // Controls access to critical section
    sem_init(&PRM_empty, 0, BufferSize); // counts number of empty buffer slots
    sem_init(&PRM_full, 0, 0); // counts number of full buffer slots


    //init process queue - mailing box
    ProcessQueues = calloc(MaxNumOfProcesses, sizeof (Queue_t_p));
    if (ProcessQueues == 0) {
        ASSERT_PRINT("Error While creating ProcessQueues\n");
        return FALSE;
    }
    for (i = 0; i < MaxNumOfProcesses; i++) {
        ProcessQueues[i] = malloc(sizeof (Queue_t));
        ProcessQueues[i]->head = NULL;
        if (ProcessQueues[i] == 0) {
            ASSERT_PRINT("Error While creating ProcessQueues[%d]\n", i);
            return FALSE;
        }
    }

    PRMQueue = malloc(sizeof (Queue_t));
    if ((PRMQueue) == 0) {
        ASSERT_PRINT("Error While creating MMUQueue or PRMQueue\n");
        return FALSE;
    }

    PRMQueue->head = NULL;

    ASSERT_PRINT("Exiting:QUEUES_Init\n");
    return TRUE;
}

bool QUEUES_WriteToProcess(PID processID, QueueCommand_t_p command) //non blocking
{
    sem_wait(&PROCESSES_empty[processID]); // decrement the empty semaphore
    sem_wait(&PROCESSES_mutex[0][processID]); // enter critical section
    if(PCB_GetByProcessID(processID) == NULL){
        sem_post(&PROCESSES_mutex[0][processID]); // leave critical section
        sem_post(&PROCESSES_full[processID]); // increment the full semaphore
        return FALSE;
    }
    QueueItem_t_p lastItem = QUEUES_GetLastItem(ProcessQueues[processID]);
    QueueItem_t_p toInsert = malloc(sizeof (QueueItem_t));

    toInsert->command = command;
    toInsert->next = 0;
    if (lastItem != NULL)
        lastItem->next = toInsert;
    else
        ProcessQueues[processID]->head = toInsert;
    sem_post(&PROCESSES_mutex[0][processID]); // leave critical section
    sem_post(&PROCESSES_full[processID]); // increment the full semaphore
    return TRUE;
}

bool QUEUES_WriteToPRM(QueueCommand_t_p command) //non blocking
{
    //printf("writing to PRM...\n");
    sem_wait(&PRM_empty); // decrement the empty semaphore
    sem_wait(&PRM_mutex); // enter critical section
    QueueItem_t_p lastItem = QUEUES_GetLastItem(PRMQueue);

    QueueItem_t_p toInsert = malloc(sizeof (QueueItem_t));
    //sleep(1);
    toInsert->command = command;
    toInsert->next = NULL;
    if (lastItem != NULL)
        lastItem->next = toInsert;
    else
        PRMQueue->head = toInsert;
    int id = command->params[1]; //solves race
    sem_post(&PRM_mutex); // leave critical section
    sem_post(&PRM_full); // increment the full semaphore

    WAIT_FOR_PRM(id);

    //QUEUES_FreeCommand(toInsert->command);
}

QueueCommand_t_p QUEUES_ReadProcess(PID processID) //blocking if no messages
{
    QueueCommand_t_p ans;
    sem_wait(&PROCESSES_full[processID]); // decrement the full semaphore
    sem_wait(&PROCESSES_mutex[0][processID]); // enter critical section
    QueueItem_t_p pointer = ProcessQueues[processID]->head;
    ProcessQueues[processID]->head = pointer->next;
    ans = pointer->command;
    free(pointer);
    sem_post(&PROCESSES_mutex[0][processID]); // leave critical section
    sem_post(&PROCESSES_empty[processID]); // increment the empty semaphore
    return ans;
}

QueueCommand_t_p QUEUES_ReadPRM() //blocking if no messages
{
    //printf("reading PRM...\n");
    QueueCommand_t_p ans = 0;
    sem_wait(&PRM_full); // decrement the full semaphore
    sem_wait(&PRM_mutex); // enter critical section
    if (PRMQueue != NULL && PRMQueue->head != NULL) {
        QueueItem_t_p pointer = PRMQueue->head;
        PRMQueue->head = pointer->next;
        ans = pointer->command;
        free(pointer);
        ASSERT(ans != 0);
    }
    sem_post(&PRM_mutex); // leave critical section
    sem_post(&PRM_empty); // increment the empty semaphore
    return ans; // consume the item
}

void QUEUES_DeInitPRM() {
    while (PRMQueue->head != NULL) {
        QueueItem_t_p item = PRMQueue->head;
        PRMQueue->head = item->next;
        QUEUES_FreeCommand(item->command);
        free(item);
    }
    free(PRMQueue);
}

void QUEUES_FreeCommand(QueueCommand_t_p comm) {
    if (comm->stringParamsAmount > 0)
        free(comm->stringParams);
    if (comm->voidParamsAmount > 0)
        free(comm->voidParams);
    if (comm->paramsAmount > 0)
        free(comm->params);
    //printf("freeing command\n");
    free(comm);
}

void QUEUES_DeInitProcess(PID processID) {
    while (ProcessQueues[processID]->head != NULL) {
        QueueItem_t_p item = ProcessQueues[processID]->head;
        ProcessQueues[processID]->head = ProcessQueues[processID]->head->next;
        QUEUES_FreeCommand(item->command);
        free(item);
    }
    free(ProcessQueues[processID]);
}

void QUEUES_DeInitMMU() {
    /*
        while (MMUQueue->head != NULL) {
            QueueItem_t_p item = MMUQueue->head;
            MMUQueue->head = MMUQueue->head->next;
            QUEUES_FreeCommand(item->command);
            free(item);
        }
        free(MMUQueue);
     */
}

void QUEUES_DeInit() {

    //QUEUES_DeInitMMU();
    int i = 0;
    for (i = 0; i < MaxNumOfProcesses; i++) {
        QUEUES_DeInitProcess(i);
        DONE_WITH_PRM(i);
    }
    QUEUES_DeInitPRM();
    free(ProcessQueues);
    free(PROCESSES_mutex[1]);
    free(PROCESSES_mutex[0]);
    free(PROCESSES_mutex);
    free(PROCESSES_empty);
    free(PROCESSES_full);
}
