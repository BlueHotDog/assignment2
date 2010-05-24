
#include "mmu.h"

bool MMU_Create()
{
    ASSERT_PRINT("Entering:MMU_Create()\n");
    printf("Opening message queue /MMU for process MMU\n");
    mqd_t ret = mq_open("/MMU", O_RDWR | O_CREAT,(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH),NULL);
    pthread_create(&MMU,NULL,MMU_Main,NULL);

}

void MMU_Close()
{
    ASSERT_PRINT("Entering:MMU_Close()\n");
    MMU_shouldClose = TRUE;
}

void* MMU_Main()
{
    ASSERT_PRINT("Entering:MMU_Main()\n");
    while (!MMU_shouldClose)
    {
        //get message from message queue
        

    }
}
