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
    while (IPT_FindFrame(hashIndex, address.processID, address.pageNumber, &res) == FALSE) {
        ASSERT_PRINT("segmentation fault...\n");
        QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));
        comm->command = PRMSegmentationFault;
        comm->params = calloc(2, sizeof (int));
        comm->params[0] = address.pageNumber;
        comm->params[1] = address.processID;
        comm->paramsAmount = 2;

        QUEUES_WriteToPRM(comm);
        WAIT_FOR_PRM(address.processID);
    }
    Page toReturn = MM_ReadPage(res); //May be an error here, need some mutex to protect agains RACE conditions..
    READERSWRITERS_UnlockDataRead();
    return toReturn;
    ASSERT_PRINT("Exiting:MMU_ReadAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
}

bool MMU_WriteToAddress(MemoryAddress_t address,Page* value) {
    ASSERT_PRINT("Entering:MMU_WriteToAddress(pid:%d,addr:%d)\n", address.processID, address.pageNumber);
    MMFI res;
    READERSWRITERS_LockDataRead();
    IPT_t_p addr = HAT_GetEntry(address);
    int hashIndex = HAT_PRIVATE_Hash(address);
    while (IPT_FindFrame(hashIndex, address.processID, address.pageNumber, &res) == FALSE) {
        ASSERT_PRINT("segmentation fault...\n");
        QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));
        comm->command = PRMSegmentationFault;
        comm->params = calloc(2, sizeof (int));
        comm->params[0] = address.pageNumber;
        comm->params[1] = address.processID;
        comm->paramsAmount = 2;

        QUEUES_WriteToPRM(comm);
        WAIT_FOR_PRM(address.processID);
    }
    MM_WritePage(*value,res);
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

