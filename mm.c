#include "mm.h"
bool MM_Init()
{
    MM = calloc(NumOfPagesInDisk,sizeof(Page));
    if(MM==NULL)
        return FALSE;
    MM_Hit_Counter = 0;
    MM_Access_Counter = 0;
    pthread_mutex_init(&MM_Counter_Mutex, NULL);
    return TRUE;
}

Page MM_ReadPage(int pageNum)
{
    ASSERT_PRINT("Entering: MM_ReadPage(pageNum:%d)\n",pageNum);
    ASSERT_PRINT("Exiting: MM_ReadPage(pageNum:%d)\n",pageNum);
    IPT_UpdateReferencetyBit(pageNum, 1);
    return MM[pageNum];
}
void MM_WritePage(Page data,int pageNum, int bitsToWrite, int startingFrom, int dirtyBit)
{
    ASSERT_PRINT("Entering: MM_WritePage(pageNum:%d)\n",pageNum);
    if(MM[pageNum] == NULL)
    {
        MM[pageNum] = calloc(PageSize,sizeof(char));
        //memset(MM[pageNum],0,sizeof(MM[pageNum]));
    }
    int i=0;
    for(i=startingFrom; i<startingFrom+bitsToWrite; i++)
        MM[pageNum][i] = data[i];
    printf("%c\n\n",data[i]);
    IPT_UpdateDirtyBit(pageNum, dirtyBit);
    IPT_UpdateReferencetyBit(pageNum, 1);
    free(data);
    ASSERT_PRINT("Exiting: MM_WritePage(pageNum:%d)\n",pageNum);
}

void MM_MemoryReference()
{
    ASSERT_PRINT("Entering: MM_MemoryReference()\n");
    pthread_mutex_lock(&MM_Counter_Mutex);

    ASSERT_PRINT("=================================Counter Value:%d===============================\n",++MM_Access_Counter);
    if(MM_Access_Counter%ShiftClock==0)
        pthread_mutex_unlock(&Aging_mutex);
    else
        pthread_mutex_unlock(&MM_Counter_Mutex);
    //The Aging deamon will unlock the MM_Counter_Mutex, so that we wont have a race issue
    ASSERT_PRINT("Exiting: MM_MemoryReference()\n");
}

void MM_DeInit()
{
    int i=0;
    for(i;i<NumOfPagesInMM;i++)
        free(MM[i]);
    free(MM);
}

void MM_Hit()
{
    pthread_mutex_lock(&MM_Counter_Mutex);
        MM_Hit_Counter++;
    pthread_mutex_unlock(&MM_Counter_Mutex);
}

