#include "messagequeues.h"


QueueItem_t_p QUEUES_GetLastItem(Queue_t_p queue) {
    ASSERT_PRINT("Entering:QUEUES_GetLastItem\n");
    QueueItem_t_p toReturn = NULL;
    ASSERT(queue != NULL);
    Queue_t temp = *queue;
    while (temp.head != NULL) {
        toReturn = temp.head->next;
        temp.head = temp.head->next;
    }
    ASSERT_PRINT("Exiting:QUEUES_GetLastItem\n");
    return toReturn;
}

void QUEUES_PrintCommand(QueueCommand_t_p command) {
    printf("command:");
    switch (command->command) {
        case PRMReadAddress:
            printf("PRMReadAddress");
            break;
        case PRMWriteToAddress:
            printf("PRMWriteToAddress");
            break;
    }
    printf("\n");
    printf("paramsAmount:%d\n", command->paramsAmount);
    printf("params:\n");
    int i = 0;
    for (i = 0; i < command->paramsAmount; i++)
        printf("%d\n", command->params[i]);
}

bool QUEUES_Init() {
BufferSize = 0x10000;

    PROCESSES_mutex = calloc(MaxNumOfProcesses, sizeof (sem_t));
    PROCESSES_empty = calloc(MaxNumOfProcesses, sizeof (sem_t));
    PROCESSES_full = calloc(MaxNumOfProcesses, sizeof (sem_t));

    int i;
    for (i = 0; i < MaxNumOfProcesses; i++) {
        sem_init(&PROCESSES_mutex[i], 0, 1); // Controls access to critical section
        sem_init(&PROCESSES_empty[i], 0, BufferSize);// counts number of empty buffer slots
        sem_init(&PROCESSES_full[i],0,0); // counts number of full buffer slots
    }

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
    while (TRUE) { // loop forever
        sem_wait(&PROCESSES_empty[processID]); // decrement the empty semaphore
        sem_wait(&PROCESSES_mutex[processID]); // enter critical section
        QueueItem_t_p lastItem = QUEUES_GetLastItem(ProcessQueues[processID]);
        QueueItem_t_p toInsert = malloc(sizeof (QueueItem_t_p));

        toInsert->command = command;
        toInsert->next = NULL;
        if (lastItem != NULL)
            lastItem->next = toInsert;
        else
            ProcessQueues[processID]->head = toInsert;
        sem_post(&PROCESSES_mutex[processID]); // leave critical section
        sem_post(&PROCESSES_full[processID]); // increment the full semaphore
    }
}

bool QUEUES_WriteToPRM(QueueCommand_t_p command) //non blocking
{
    while (TRUE) { // loop forever
        sem_wait(&PRM_empty); // decrement the empty semaphore
        sem_wait(&PRM_mutex); // enter critical section
        QueueItem_t_p lastItem = QUEUES_GetLastItem(PRMQueue);
        QueueItem_t_p toInsert = malloc(sizeof (QueueItem_t_p));

        toInsert->command = command;
        toInsert->next = NULL;
        if (lastItem != NULL)
            lastItem->next = toInsert;
        else
            PRMQueue->head = toInsert;
        sem_post(&PRM_mutex); // leave critical section
        sem_post(&PRM_full); // increment the full semaphore
    }
}

QueueCommand_t_p QUEUES_ReadProcess(PID processID) //blocking if no messages
{
    QueueCommand_t_p ans;
    while (TRUE) { // loop forever
        sem_wait(&PROCESSES_full[processID]); // decrement the full semaphore
        sem_wait(&PROCESSES_mutex[processID]); // enter critical section

        //if there aren't any messages to read, wait.
        if (ProcessQueues[processID]->head == NULL)
            pthread_mutex_lock(&ProcessReader[processID]);

        //else of when a message arrived
        QueueItem_t_p pointer = ProcessQueues[processID]->head;
        ProcessQueues[processID]->head = pointer->next;
        ans = pointer->command;
        sem_post(&PROCESSES_mutex[processID]); // leave critical section
        sem_post(&PROCESSES_empty[processID]); // increment the empty semaphore
        return ans;
    }
}

QueueCommand_t_p QUEUES_ReadPRM() //blocking if no messages
{
    QueueCommand_t_p ans;
    while (TRUE) { // loop forever
        sem_wait(&PRM_full); // decrement the full semaphore
        sem_wait(&PRM_mutex); // enter critical section
        //if there aren't any messages to read, wait.
/*
        if (PRMQueue->head == NULL)
            pthread_mutex_lock(&PRMReader);
*/

        //else or when a message arrived
        QueueItem_t_p pointer = PRMQueue->head;
        PRMQueue->head = pointer->next;
        ans = pointer->command;
        ASSERT(ans != 0);

        sem_post(&PRM_mutex); // leave critical section
        sem_post(&PRM_empty); // increment the empty semaphore
        return ans; // consume the item
    }
}


/*
  QUEUE_Write() //Producer
  {
    int widget;

    while (TRUE) {                  // loop forever
      make_new(widget);             // create a new widget to put in the buffer
      down(empty);                 // decrement the empty semaphore
      down(&mutex);                 // enter critical section
      put_item(widget);             // put widget in buffer
      up(&mutex);                   // leave critical section
      up(&full);                    // increment the full semaphore
      }
  }

  QUEUE_Read() //Consumer
  {
    int widget;
      
    while (TRUE) {                  // loop forever
      down(&full);                  // decrement the full semaphore
      down(&mutex);                 // enter critical section
      remove_item(widget);          // take a widget from the buffer
      up(&mutex);                   // leave critical section
      up(empty);                   // increment the empty semaphore
      consume_item(widget);         // consume the item
      }
  }
 */
