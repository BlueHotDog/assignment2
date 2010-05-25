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