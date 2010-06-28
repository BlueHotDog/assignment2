#include "disk.h"

//initializes the disk data-type
//return true on success, false otherwise

bool DISK_Init() {
    ASSERT_PRINT("Entering:DISK_Init\n");
    Disk = calloc(NumOfPagesInDisk, sizeof (Page));
    int i = 0;
    int j = 0;
    for (i = 0; i < NumOfPagesInDisk; i++)
        Disk[i] = calloc(PageSize, sizeof (char));


/*
    for (i = 0; i < NumOfPagesInDisk; i++)
        for (j = 0; j < PageSize; j++) {
            Disk[i][j] = 48 + (((i + 1) * j) % 42);
        }
*/

    //ASSERT(DISK_PrintContent());
    if (Disk == NULL)
        return FALSE;
    return TRUE;
}

void DISK_PrintContent() {
    int i = 0;
    int j = 0;
    ASSERT_PRINT("Disk content:\n");
    for (i = 0; i < NumOfPagesInDisk; i++)
    {
        for (j = 0; j < PageSize; j++)
            fprintf(outFile,"%c|", Disk[i][j]);
        fprintf(outFile,"\n");
        if((i+1)%NumOfProcessPages==0)
            fprintf(outFile,"\n");
    }
    ASSERT_PRINT("End disk content\n");
}

bool DISK_AllocateSpace(unsigned int start, unsigned int end) {
    ASSERT_PRINT("Entering:DISK_AllocateSpace(%d,%d)\n", start, end);
    int i = start;
    for (i; i < end; i++) {
        if (FreeList[i].isFree == FALSE) {
            fprintf(outFile,"Error allocating space for process\n");
            return FALSE;
        }
        //Disk[i] = calloc(PageSize, sizeof (char));
        if (Disk[i] == NULL)
            return FALSE;
    }
    FREELIST_SetTaken(start);
    ASSERT_PRINT("Exiting:DISK_AllocateSpace(%d,%d)\n", start, end);
    return TRUE;

}

bool DISK_DeAllocateSpace(unsigned int start, unsigned int end) {
    ASSERT_PRINT("Entering:DISK_DeAllocateSpace(%d,%d)\n", start, end);
    int i = start;
    for (i; i < end; i++)
        free(Disk[i]);
    ASSERT_PRINT("Exiting:DISK_DeAllocateSpace(%d,%d)\n", start, end);
}

void DISK_ReadPage(int pageNum, OUT Page* pageToReturn) {
    ASSERT_PRINT("Entering:DISK_ReadPage(pageNum:%d)\n", pageNum);
    int i=0;
    for(i=0; i<PageSize; i++)
        (*pageToReturn)[i] = Disk[pageNum][i];
    ASSERT_PRINT("Exiting:DISK_ReadPage(pageNum:%d)\n", pageNum);
}

//Writes the data to the pageNumber returns true if all went well, false otherwise.

bool DISK_WritePage(Page data, int pageNum) {
    ASSERT_PRINT("Entering:DISK_WritePage(pageNum:%d)\n", pageNum);
    //ASSERT(strlen(data) == PageSize);
    int i=0;
    for(i; i<PageSize; i++)
        Disk[pageNum][i] = data[i];
    ASSERT_PRINT("Exiting:DISK_WritePage(pageNum:%d)\n", pageNum);
}

void DISK_DeInit()
{
    DISK_DeAllocateSpace(0,NumOfPagesInDisk);
    free(Disk);
}