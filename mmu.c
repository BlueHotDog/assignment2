#include "mmu.h"
#include "hat.h"

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

Page MMU_ReadAddress(MemoryAddress_t address)
{
     ASSERT_PRINT("Entering:MMU_ReadAddress(pid:%d,addr:%d)\n",address.processID,address.pageNumber);
     IPT_t_p addr = HAT_GetEntry(address);
     if(addr==NULL)
     {
         printf("segmentation fault...");
     }
     else
     {
         
     }
     ASSERT_PRINT("Exiting:MMU_ReadAddress(pid:%d,addr:%d)\n",address.processID,address.pageNumber);
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

