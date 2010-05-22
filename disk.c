#include "disk.h"
#include "freelist.h"
//initializes the disk data-type
//return true on success, false otherwise
bool DISK_Init()
{
    ASSERT_PRINT("Entering:DISK_Init\n");
    Disk = calloc(NumOfPagesInDisk,sizeof(Page));
    if(Disk==NULL)
        return FALSE;
    return TRUE;
}
bool DISK_AllocateSpace(unsigned int start,unsigned int end)
{
    ASSERT_PRINT("Entering:DISK_AllocateSpace(%d,%d)\n",start,end);
    int i=start;
    for(i;i<end;i++)
    {
        Disk[i] = calloc(PageSize,sizeof(char));
        if(Disk[i]==NULL)
            return FALSE;
    }
    FREELIST_SetTaken(start);
    return TRUE;

}
bool DISK_DeAllocateSpace(unsigned int start,unsigned int end)
{
    ASSERT_PRINT("Entering:DISK_DeAllocateSpace(%d,%d)\n",start,end);
    int i=start;
    for(i;i<=end;i++)
        free(Disk[i]);
}
