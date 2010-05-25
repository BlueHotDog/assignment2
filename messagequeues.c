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

    pthread_mutex_lock(&ProcessReader[processID]);
    pthread_mutex_lock(&ProcessWriter[processID]);



    pthread_mutex_unlock(&ProcessReader[processID]);
    pthread_mutex_unlock(&ProcessWriter[processID]);

    ASSERT_PRINT("Exiting:QUEUES_WriteToProcess(%d,%d)\n", processID, command->t);
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