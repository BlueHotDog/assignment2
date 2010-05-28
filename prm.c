#include "prm.h"

bool PRM_Init() {
    ASSERT_PRINT("Entering:PRM_Create()\n");
    if (pthread_create(&PRM, NULL, PRM_Main, NULL) != 0)
        return FALSE;
    ASSERT_PRINT("Exiting:PRM_Create()\n");
    return TRUE;
}

void PRM_Close() {
    ASSERT_PRINT("Entering:PRM_Close()\n");
    PRM_shouldClose = TRUE;
    ASSERT_PRINT("Exiting:PRM_Close()\n");
}

void* PRM_Main() {
    ASSERT_PRINT("Entering:PRM_Main()\n");

    while (!PRM_shouldClose) {

        ASSERT_PRINT("PRM trying to read from queue /PRM\n");
        QueueCommand_t_p command = QUEUES_ReadPRM();
        QUEUES_PrintCommand(command);
        
        free(command->params);
        free(command);
    }
    ASSERT_PRINT("Exiting:PRM_Main()\n");
}
