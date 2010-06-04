
#include "mm.h"

bool MM_Init()
{
    MM = calloc(NumOfPagesInDisk,sizeof(Page));
    if(MM==NULL)
        return FALSE;
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
    IPT_UpdateDirtyBit(pageNum, 1, dirtyBit);
    ASSERT_PRINT("Exiting: MM_WritePage(pageNum:%d)\n",pageNum);
}