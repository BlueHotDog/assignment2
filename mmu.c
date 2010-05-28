#include "mmu.h"

bool MMU_Init() {
    ASSERT_PRINT("Entering:MMU_Create()\n");
    if (pthread_create(&MMU, NULL, MMU_Main, NULL) != 0)
        return FALSE;
    ASSERT_PRINT("Exiting:MMU_Create()\n");
    return TRUE;
}

void MMU_Close() {
    ASSERT_PRINT("Entering:MMU_Close()\n");
    MMU_shouldClose = TRUE;
    ASSERT_PRINT("Exiting:MMU_Close()\n");
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
