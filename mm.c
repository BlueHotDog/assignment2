
#include "mm.h"

bool MM_Init()
{
    MM = calloc(NumOfPagesInDisk,sizeof(Page));
    if(MM==NULL)
        return FALSE;
    return TRUE;
}