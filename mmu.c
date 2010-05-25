#include "mmu.h"

bool MMU_Create()
{
   // ASSERT_PRINT("Entering:MMU_Create()\n");
   // printf("Opening message queue /MMU for process MMU\n");
    //if(QUEUE_Create("/MMU")==-1)
    //    return FALSE;
    if(pthread_create(&MMU,NULL,MMU_Main,NULL)!=0)
        return FALSE;
    
    return TRUE;
}

void MMU_Close()
{
    ASSERT_PRINT("Entering:MMU_Close()\n");
    MMU_shouldClose = TRUE;
}

void* MMU_Main()
{
    ASSERT_PRINT("Entering:MMU_Main()\n");
/*
    mqd_t ret = mq_open("/MMU", O_RDWR,(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH),NULL);
    string message = calloc(MAX_MESSAGE_SIZE,sizeof(char));
    while (!MMU_shouldClose)
    {
        memset(message,0,sizeof(MAX_MESSAGE_SIZE*sizeof(char)));
        ASSERT_PRINT("MMU trying to read from queue /MMU\n");
        int ret =  mq_receive(ret, message, MAX_MESSAGE_SIZE,NULL);
        ASSERT_PRINT("MMU finished reading message of size:%d\n",ret);
    }
*/
}
