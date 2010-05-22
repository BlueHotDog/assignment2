#include "freelist.h"
bool FREELIST_Init()
{
    ASSERT_PRINT("Entering:FREELIST_Init\n");
    FreeList = calloc(NumOfPagesInDisk,sizeof(FreeList_t));
    if(FreeList==NULL)
        return FALSE;
    int i=0;
    for(i=0;i<NumOfPagesInDisk;i++)
        FreeList[i].isFree = TRUE;
    return TRUE;
}
int FREELIST_Get()
{
    ASSERT_PRINT("Entering:FREELIST_Get\n");
    int i=0;
    for(i=0;i<NumOfPagesInDisk;i++)
        if(FreeList[i].isFree==TRUE)
        {
            ASSERT_PRINT("FREELIST_Get found Page %d as free,checking next %d pages\n",i,NumOfProcessPages);
            int ii=i;
            for(ii=i;ii<(i+NumOfProcessPages);ii++)
                if(ii>NumOfPagesInDisk || FreeList[ii].isFree==FALSE)
                    return -1;
            return i;
        }
    return -1;
}
void FREELIST_DeAllocate()
{
    ASSERT_PRINT("Entering:FREELIST_DeAllocate\n");
    free(FreeList);
}
void FREELIST_SetTaken(unsigned int index)
{
    ASSERT_PRINT("Entering:FREELIST_SetTaken(%d)\n",index);
    int i=index;
    for(i=index;i<(index+NumOfProcessPages);i++)
        FreeList[i].isFree = FALSE;
}

