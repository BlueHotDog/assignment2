
#include "mm.h"
#include "aging.h"

bool MM_Init()
{
    MM = calloc(NumOfPagesInDisk,sizeof(Page));
    if(MM==NULL)
        return FALSE;
    pthread_mutex_init(&MM_Counter_Mutex, NULL);
    return TRUE;
}

Page MM_ReadPage(int pageNum)
{
    ASSERT_PRINT("Entering: MM_ReadPage(pageNum:%d)\n",pageNum);
    ASSERT_PRINT("Exiting: MM_ReadPage(pageNum:%d)\n",pageNum);
    return MM[pageNum];
}
void MM_WritePage(Page data,int pageNum, int bitsToWrite, int dirtyBit)
{
    ASSERT_PRINT("Entering: MM_WritePage(pageNum:%d)\n",pageNum);
    if(MM[pageNum] == NULL)
        MM[pageNum] = calloc(PageSize,sizeof(char));
    int i=0;
    for(i=0; i<bitsToWrite; i++)
        MM[pageNum][i] = data[i];
    IPT_UpdateDirtyBit(pageNum, dirtyBit);
    ASSERT_PRINT("Exiting: MM_WritePage(pageNum:%d)\n",pageNum);
}

void MM_MemoryReference()
{
    ASSERT_PRINT("Entering: MM_MemoryReference()\n");
    pthread_mutex_lock(&MM_Counter_Mutex);
    MM_Access_Counter++;
    if(MM_Access_Counter%ShiftClock==0)
        pthread_mutex_unlock(&Aging_mutex);
    //The Aging deamon will unlock the MM_Counter_Mutex, so that we wont have a race issue
    //pthread_mutex_unlock(&MM_Counter_Mutex);
    ASSERT_PRINT("Exiting: MM_MemoryReference()\n");
}