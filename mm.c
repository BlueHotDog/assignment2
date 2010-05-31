
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
void MM_WritePage(Page data,int pageNum)
{
    ASSERT_PRINT("Entering: MM_WritePage(pageNum:%d)\n",pageNum);
    MM[pageNum] = data;
    ASSERT_PRINT("Exiting: MM_WritePage(pageNum:%d)\n",pageNum);
}