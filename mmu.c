#include "mmu.h"

bool MMU_Init() {
    ASSERT_PRINT("Entering:MMU_Create()\n");

    ASSERT_PRINT("Exiting:MMU_Create()\n");
    return TRUE;
}

void MMU_Close() {
    ASSERT_PRINT("Entering:MMU_Close()\n");
    MMU_shouldClose = TRUE;
    ASSERT_PRINT("Exiting:MMU_Close()\n");
}

Page MMU_ReadAddress(MemoryAddress_t address) {
    ASSERT_PRINT("Entering:MMU_ReadAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
    MMFI res;

    READERSWRITERS_LockDataRead();
    IPT_t_p addr = HAT_GetEntry(address);
    int hashIndex = HAT_PRIVATE_Hash(address);
    bool wasInLoop = FALSE;
    while (IPT_FindFrame(hashIndex, address.processID, address.pageNumber, &res) == FALSE) {
        ASSERT_PRINT("segmentation fault...\n");
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
    Page toReturn = MM_ReadPage(res); //May be an error here, need some mutex to protect agains RACE conditions..
    READERSWRITERS_UnlockDataRead();
    return toReturn;
    ASSERT_PRINT("Exiting:MMU_ReadAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
}

bool MMU_WriteToAddress(MemoryAddress_t address, Page value, int bitsToWrite, int startingFrom) {
    ASSERT_PRINT("Entering:MMU_WriteToAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
    MMFI res;

    READERSWRITERS_LockDataRead();
    IPT_t_p addr = HAT_GetEntry(address);
    int hashIndex = HAT_PRIVATE_Hash(address);
    bool wasInLoop = FALSE;
    while (IPT_FindFrame(hashIndex, address.processID, address.pageNumber, &res) == FALSE) {
        ASSERT_PRINT("segmentation fault...\n");
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
    MM_WritePage(value, res, bitsToWrite, startingFrom, 1);
    READERSWRITERS_UnlockDataRead();
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

