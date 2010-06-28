#include "mmu.h"

bool MMU_Init() {
    ASSERT_PRINT("Entering:MMU_Create()\n");
pthread_mutex_init(&MMU_mutex_helper,NULL);
    ASSERT_PRINT("Exiting:MMU_Create()\n");
    return TRUE;
}

void MMU_Close() {
    ASSERT_PRINT("Entering:MMU_Close()\n");
    MMU_shouldClose = TRUE;
    ASSERT_PRINT("Exiting:MMU_Close()\n");
}

char MMU_ReadAddress(MemoryAddress_t address, int bitIndex) {
    ASSERT_PRINT("Entering:MMU_ReadAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
    MMFI res;
pthread_mutex_lock(&MMU_mutex_helper);
    READERSWRITERS_LockDataRead();
    IPT_t_p addr = HAT_GetEntry(address);
    int hashIndex = HAT_PRIVATE_Hash(address);
    bool wasInLoop = FALSE;
    while (IPT_FindFrame(hashIndex, address.processID, address.pageNumber, &res) == FALSE) {
        ASSERT_PRINT("segmentation fault...\n");
        if (monitor) printf("sending message to PRM: PRMSegmentationFault, page:%d, pid:%d\n",address.pageNumber, address.processID);
        wasInLoop = TRUE;
        QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));
        comm->command = PRMSegmentationFault;
        comm->params = calloc(2, sizeof (int));
        comm->params[0] = address.pageNumber;
        comm->params[1] = address.processID;
        comm->paramsAmount = 2;
        comm->stringParamsAmount = 0;
        comm->voidParamsAmount = 0;

        QUEUES_WriteToPRM(comm);
    }
    if (!wasInLoop)
        MM_Hit();
    if (monitor) printf("MMU:\tfound MM frame:%d for page:%d, pid:%d\n",res ,address.pageNumber, address.processID);
    Page toReturn = MM_ReadPage(res); //May be an error here, need some mutex to protect agains RACE conditions..
    char ret = toReturn[bitIndex];
    READERSWRITERS_UnlockDataRead();
pthread_mutex_unlock(&MMU_mutex_helper);
    return ret;
    ASSERT_PRINT("Exiting:MMU_ReadAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
}

bool MMU_WriteToAddress(MemoryAddress_t address, Page value, int bitsToWrite, int startingFrom) {
    ASSERT_PRINT("Entering:MMU_WriteToAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
    MMFI res = -1;
pthread_mutex_lock(&MMU_mutex_helper);
    READERSWRITERS_LockDataRead();
    IPT_t_p addr = HAT_GetEntry(address);
    int hashIndex = HAT_PRIVATE_Hash(address);
    bool wasInLoop = FALSE;
    while (IPT_FindFrame(hashIndex, address.processID, address.pageNumber, &res) == FALSE) {
        ASSERT_PRINT("segmentation fault...\n");
        if (monitor) printf("MMU:\tPRMSegmentationFault, page:%d, pid:%d\n",address.pageNumber, address.processID);
        wasInLoop = TRUE;
        QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));
        comm->command = PRMSegmentationFault;
        comm->params = calloc(2, sizeof (int));
        comm->params[0] = address.pageNumber;
        comm->params[1] = address.processID;
        comm->paramsAmount = 2;
        comm->stringParamsAmount = 0;
        comm->voidParamsAmount = 0;
        QUEUES_WriteToPRM(comm);
    }
    if (!wasInLoop)
        MM_Hit();
    if (monitor) printf("MMU:\tfound MM frame:%d for address page:%d, pid:%d\n",res ,address.pageNumber, address.processID);
    MM_WritePage(value, res, bitsToWrite, startingFrom, 1);
    READERSWRITERS_UnlockDataRead();
pthread_mutex_unlock(&MMU_mutex_helper);
    return TRUE;
    ASSERT_PRINT("Exiting:MMU_WriteToAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
}

void* MMU_Main() {
    ASSERT_PRINT("Entering:MMU_Main()\n");

    while (!MMU_shouldClose) {
        ASSERT_PRINT("MMU trying to read from queue /MMU\n");
        //        QueueCommand_t_p command = QUEUES_ReadMMU();
        //        QUEUES_PrintCommand(command);
        //        free(command);
    }
    ASSERT_PRINT("Exiting:MMU_Main()\n");

}

