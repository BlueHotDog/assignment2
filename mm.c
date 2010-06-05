
#include "mm.h"
#include "aging.h"

bool MM_Init()
{
    MM = calloc(NumOfPagesInDisk,sizeof(Page));
    if(MM==NULL)
        return FALSE;
    sem_init(&MM_Counter_Mutex, 0, ShiftClock);
    return TRUE;
}

Page MM_ReadPage(int pageNum)
{
    ASSERT_PRINT("Entering: MM_ReadPage(pageNum:%d)\n",pageNum);
    ASSERT_PRINT("Exiting: MM_ReadPage(pageNum:%d)\n",pageNum);
    IPT_UpdateReferencetyBit(pageNum, 1);
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
    IPT_UpdateReferencetyBit(pageNum, 1);
    ASSERT_PRINT("Exiting: MM_WritePage(pageNum:%d)\n",pageNum);
}

void MM_MemoryReference()
{
    ASSERT_PRINT("Entering: MM_MemoryReference()\n");
    sem_wait(&MM_Counter_Mutex);
    MM_Access_Counter++;
    if(MM_Access_Counter%ShiftClock==0)
        sem_post(&Aging_mutex);
    //The Aging deamon will unlock the MM_Counter_Mutex, so that we wont have a race issue
    ASSERT_PRINT("Exiting: MM_MemoryReference()\n");
}