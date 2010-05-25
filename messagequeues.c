#include "messagequeues.h"

bool QUEUES_Init() {
    ASSERT_PRINT("Entering:QUEUES_Init\n");
    int i = 0;
    ProcessQueues = calloc(MaxNumOfProcesses, sizeof (Queue_t_p));
    if (ProcessQueues == 0) {
        ASSERT_PRINT("Error While creating ProcessQueues\n");
        return FALSE;
    }
    for (i = 0; i < MaxNumOfProcesses; i++) {
        ProcessQueues[i] = malloc(sizeof (Queue_t));
        if (ProcessQueues[i] == 0) {
            ASSERT_PRINT("Error While creating ProcessQueues[%d]\n", i);
            return FALSE;
        }
    }
    ProcessReader = calloc(MaxNumOfProcesses, sizeof (pthread_mutex_t));
    ProcessWriter = calloc(MaxNumOfProcesses, sizeof (pthread_mutex_t));

    if (ProcessReader == 0 || ProcessWriter == 0) {
        ASSERT_PRINT("Error While creating ProcessReader or ProcessWriter\n");
        return FALSE;
    }

    Queue_t_p MMUQueue = malloc(sizeof (Queue_t));

    Queue_t_p PRMQueue = malloc(sizeof (Queue_t));

    if ((PRMQueue) == 0 || MMUQueue == 0) {
        ASSERT_PRINT("Error While creating MMUQueue or PRMQueue\n");
        return FALSE;
    }

    MMUQueue->head = NULL;

    PRMQueue->head = NULL;

    ASSERT_PRINT("Exiting:QUEUES_Init\n");
    return TRUE;
}

bool QUEUES_WriteToProcess(PID processID, QueueCommand_t_p command) {
    ASSERT(processID >= 0);
    ASSERT(command != NULL);
    ASSERT_PRINT("Entering:QUEUES_WriteToProcess(%d,%d)\n", processID, command->t);

    pthread_mutex_lock(&ProcessWriter[processID]);

    QueueItem_t_p lastItem = QUEUES_GetLastItem(ProcessQueues[processID]);
    QueueItem_t_p toInsert = malloc(sizeof (QueueItem_t_p));

    toInsert->command = command;
    toInsert->next = NULL;
    if (lastItem != NULL)
        lastItem->next = toInsert;
    else
        ProcessQueues[processID]->head = toInsert;

    pthread_mutex_unlock(&ProcessWriter[processID]);
    pthread_mutex_unlock(&ProcessReader[processID]);

    ASSERT_PRINT("Exiting:QUEUES_WriteToProcess(%d,%d)\n", processID, command->t);
}

bool QUEUES_WriteToMMU(QueueCommand_t_p command) {
    ASSERT(command != NULL);
    ASSERT_PRINT("Entering:QUEUES_WriteToMMU(%d)\n", command->t);

    pthread_mutex_lock(&MMUWriter);

    QueueItem_t_p lastItem = QUEUES_GetLastItem(MMUQueue);
    QueueItem_t_p toInsert = malloc(sizeof (QueueItem_t_p));

    toInsert->command = command;
    toInsert->next = NULL;
    if (lastItem != NULL)
        lastItem->next = toInsert;
    else
        MMUQueue->head = toInsert;

    pthread_mutex_unlock(&MMUWriter);
    pthread_mutex_unlock(&MMUReader);

    ASSERT_PRINT("Entering:QUEUES_WriteToMMU(%d)\n", command->t);
}

bool QUEUES_WriteToPRM(QueueCommand_t_p command) {
    ASSERT(command != NULL);
    ASSERT_PRINT("Entering:QUEUES_WriteToPRM(%d)\n", command->t);

    pthread_mutex_lock(&PRMWriter);

    QueueItem_t_p lastItem = QUEUES_GetLastItem(PRMQueue);
    QueueItem_t_p toInsert = malloc(sizeof (QueueItem_t_p));

    toInsert->command = command;
    toInsert->next = NULL;
    if (lastItem != NULL)
        lastItem->next = toInsert;
    else
        PRMQueue->head = toInsert;

    pthread_mutex_unlock(&PRMWriter);
    pthread_mutex_unlock(&PRMReader);

    ASSERT_PRINT("Entering:QUEUES_WriteToPRM(%d)\n", command->t);
}





QueueCommand_t_p QUEUES_ReadProcess(PID processID) //blocking if no messages
{
    ASSERT_PRINT("Entering:QUEUES_ReadProcess\n");
    QueueCommand_t_p queueCommand;
    pthread_mutex_lock(&ProcessWriter[processID]);

    //if there aren't any messages to read, wait.
    if (ProcessQueues[processID]->head == NULL)
        pthread_mutex_lock(&ProcessReader[processID]);

    //else of when a message arrived
    QueueItem_t_p pointer = ProcessQueues[processID]->head;
    ProcessQueues[processID]->head = pointer->next;
    queueCommand = pointer->command;

    pthread_mutex_unlock(&ProcessWriter[processID]);
    ASSERT_PRINT("Exiting:QUEUES_ReadProcess\n");
    return queueCommand;
}

QueueCommand_t_p QUEUES_ReadMMU() //blocking if no messages
{
    ASSERT_PRINT("Entering:QUEUES_ReadMMU\n");
    QueueCommand_t_p queueCommand;
    pthread_mutex_lock(&MMUWriter);

    //if there aren't any messages to read, wait.
    if (MMUQueue->head == NULL)
        pthread_mutex_lock(&MMUReader);

    //else of when a message arrived
    QueueItem_t_p pointer = MMUQueue->head;
    MMUQueue->head = pointer->next;
    queueCommand = pointer->command;

    pthread_mutex_unlock(&MMUWriter);
    ASSERT_PRINT("Exit:QUEUES_ReadMMU\n");
    return queueCommand;
}

QueueCommand_t_p QUEUES_ReadPRM() //blocking if no messages
{
    ASSERT_PRINT("Entering:QUEUES_ReadPRM\n");
    QueueCommand_t_p queueCommand;
    pthread_mutex_lock(&PRMWriter);

    //if there aren't any messages to read, wait.
    if (PRMQueue->head == NULL)
        pthread_mutex_lock(&PRMReader);

    //else of when a message arrived
    QueueItem_t_p pointer = PRMQueue->head;
    PRMQueue->head = pointer->next;
    queueCommand = pointer->command;

    pthread_mutex_unlock(&PRMWriter);
    ASSERT_PRINT("Exit:QUEUES_ReadPRM\n");
    return queueCommand;
}

QueueItem_t_p QUEUES_GetLastItem(Queue_t_p queue) {
    ASSERT_PRINT("Entering:QUEUES_GetLastItem\n");
    QueueItem_t_p toReturn = NULL;
    Queue_t temp = *queue;
    while (temp.head != NULL) {
        toReturn = temp.head->next;
        temp.head = temp.head->next;
    }
    ASSERT_PRINT("Exiting:QUEUES_GetLastItem\n");
    return toReturn;
}