#include "ipt.h"
#include "hat.h"

bool IPT_Init()
{
    ASSERT_PRINT("Entering:IPT_Init()\n");
    IPT = calloc(SIZE_OF_IPT, sizeof (IPT_t));
    if (IPT == NULL)
        return FALSE;
    int i = 0;
    for (i = 0; i < SIZE_OF_IPT; i++) {
        IPT[i] = 0;
    }
    totalPagesInIPT = 0;
    ASSERT_PRINT("Exiting:IPT_Init()\n");
    return TRUE;
}

IPT_t_p IPT_CreateIPT_t_p(
        PID processID,
        LPN pageNumber,
        MMFI frame)
{
    ASSERT_PRINT("Entering:IPT_CreateIPPRM_ReplaceMMFrameWithDiskFrameT_t_p()\n");
    IPT_t_p newIPTLine;
    if (!(newIPTLine = malloc(sizeof (IPT_t))))
        return FALSE;

    (newIPTLine)->dirtyBit = 0;
    (newIPTLine)->frame = frame;
    (newIPTLine)->next = NULL;
    (newIPTLine)->pageNumber = pageNumber;
    (newIPTLine)->processID = processID;
    (newIPTLine)->referenceBit = 0;
    ASSERT_PRINT("Exiting:IPT_CreateIPT_t_p()\n");
    return newIPTLine;
}

bool IPT_Add(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber,
        MMFI frame)
{
    ASSERT_PRINT("Entering:IPT_Add()\n");
    IPT_t_p newIPTLine;
    newIPTLine = IPT_CreateIPT_t_p(processID, pageNumber, frame);
    IPT_t_p pointer = HAT[HATPointedIndex];
    if (pointer == NULL) //the field was never invoked. 
    {
        newIPTLine->prev = 0;
        newIPTLine->next = 0;
        int lineIndex = IPT_FindIndexByPointer(pointer);
        IPT[lineIndex] = newIPTLine;
        HAT[HATPointedIndex] = newIPTLine;
        totalPagesInIPT++;
        return TRUE;
    }

    bool foundFrame = FALSE;
    IPT_t_p temp = HAT[HATPointedIndex];
    while (temp != NULL && temp->next != NULL){
        temp = temp->next;
    }
    IPT_t_p* newPointer;
    if (newPointer = IPT_FindEmptyLine()) {
        return FALSE;
    } else
        foundFrame = TRUE;
    *newPointer = newIPTLine;

    newIPTLine->prev = temp;
    temp->next = newIPTLine;
    newIPTLine->next = 0;
    //IPT[temp] = newIPTLine;
   // HAT[HATPointedIndex] = newIPTLine;
    totalPagesInIPT++;
    ASSERT_PRINT("Exiting:IPT_Add()\n");
    return TRUE;
}

IPT_t_p* IPT_FindIPTLine(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber)
{
    ASSERT_PRINT("Entering:IPT_FindIPTLine()\n");
    int iterations = 0;
    IPT_t_p* toReturn = NULL;
    IPT_t_p pointer = HAT[HATPointedIndex];
    while (pointer != 0 && iterations <= SIZE_OF_IPT)
    {
        if (pointer->processID == processID && pointer->pageNumber == pageNumber)
        {
            toReturn = &pointer;
            ASSERT_PRINT("Exiting:IPT_FindIPTLine() with return value: TRUE\n");
        }
        INDEX_INC(HATPointedIndex);
        iterations++;
    }
    //the page is not in the IPT, i.e. not in the MM
    ASSERT_PRINT("Exiting:IPT_FindIPTLine() with return value: FALSE\n");
    return toReturn;

}

bool IPT_FindFrame(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber,
        OUT MMFI *frame)
{
    ASSERT_PRINT("Entering:IPT_FindFramPRM_ReplaceMMFrameWithDiskFramee()\n");
    IPT_t_p* line = NULL;
    if (line=IPT_FindIPTLine(HATPointedIndex,processID,pageNumber))
    {
        *frame = (*line)->frame;
        ASSERT_PRINT("Exiting:IPT_FindFrame() with return value: TRUE, frame=%d\n", *frame);
        return TRUE;
    }
    ASSERT_PRINT("Exiting:IPT_FindFrame() with return value: FALSE\n");
    return FALSE;
}

bool IPT_Remove(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber)
{
    ASSERT_PRINT("Entering:IPT_Remove()\n");
    IPT_t_p* line = NULL;
    if (line = IPT_FindIPTLine(HATPointedIndex, processID, pageNumber))
    {
        // the entry is not in the IPT.
        return FALSE;
    }

    if((*line)->prev!=NULL)
    {
        (*line)->prev = (*line)->next;
        if((*line)->next!=NULL)
            (*line)->next->prev = (*line)->prev;
    }
    else
    {
        HAT[HATPointedIndex] = (*line)->next;
    }
    IPT_t_p* temp = line;
    *line = NULL;
    //toDelete = NULL;

    free(*temp);
    totalPagesInIPT--;
    ASSERT_PRINT("Exiting:IPT_Remove() with return value: TRUE\n");
    return TRUE;
}

int IPT_FindEmptyFrame()
{
    ASSERT_PRINT("Entering:IPT_FindEmptyFrame()\n");
    int i=0;
    bool* frameArry = calloc(SIZE_OF_IPT, sizeof(bool));
    for(i;i<SIZE_OF_IPT; i++)
        frameArry[i] = FALSE;

    for (i=0;i<SIZE_OF_IPT; i++)
        if(IPT[i] != NULL && IPT[i]->frame!=-1)
            frameArry[IPT[i]->frame] = TRUE;

    i=0;
    while (frameArry[i] && i<SIZE_OF_IPT)
        i++;

    if (i>=SIZE_OF_IPT)
    {
        ASSERT_PRINT("Exiting:IPT_FindEmptyFrame() with return value: FALSE\n");
        i=-1;
    }
    free(frameArry);
    ASSERT_PRINT("Exiting:IPT_FindEmptyFrame() with return value: TRUE, frame = %d\n",i);
    return i;
}

IPT_t_p* IPT_FindEmptyLine()
{
    ASSERT_PRINT("Entering:IPT_FindEmptyLine()\n");
    int i=0;
    for (i; i<SIZE_OF_IPT; i++)
        if(IPT[i] == NULL)
            return &IPT[i];
    ASSERT_PRINT("Exiting:IPT_FindEmptyLine()\n");
    return NULL;
}

int IPT_FindIndexByPointer(IPT_t_p pointer)
{
    int i=0;
    for(i;i<SIZE_OF_IPT; i++)
        if (IPT[i] == pointer)
            return i;
    return -1;
}

int IPT_FindLineByFrame(MMFI frame)
{
    ASSERT_PRINT("Entering:IPT_FindLineByFrame()\n");
    int i=0;
    int line = -1;
    for (i;i<SIZE_OF_IPT && line==-1; i++)
        if(IPT[i] != NULL && IPT[i]->frame == frame)
        {
            ASSERT_PRINT("Exiting:IPT_FindLineByFrame() with return value: TRUE, line = %d\n",line);
            return i;
        }
    ASSERT_PRINT("Exiting:IPT_FindLineByFrame() with return value: FALSE\n");
    return line;
}

void IPT_UpdateDirtyBit(MMFI frame, bool dirtyBit)
{
    int lineIndex = -1;
    lineIndex = IPT_FindLineByFrame(frame);
    IPT[lineIndex]->dirtyBit = dirtyBit;
}

void IPT_UpdateReferencetyBit(MMFI frame, bool referenceBit)
{
    int lineIndex = -1;
    lineIndex = IPT_FindLineByFrame(frame);
    if(lineIndex != -1) //if printMM than it is possible to access empty IPT ref.
        IPT[lineIndex]->referenceBit = referenceBit;
}

bool IPT_Replace(
        PID outProcessID,
        LPN outPageNumber,
        PID inProcessID,
        LPN inPageNumber,
        MMFI inFrame)
{
    ASSERT_PRINT("Entering:IPT_Replace()\n");
    IPT_t_p* line = NULL;
    if(line=IPT_FindIPTLine(0,outProcessID,outPageNumber))
        return FALSE;

/* YANIV.. why do you do this? i couldnt understand...
    IPT_t_p newIPTLine;
    if(IPT_CreateIPT_t_p(inProcessID,inPageNumber,inFrame,&newIPTLine))
    {
        ASSERT_PRINT("Exiting:IPT_Replace() - Cannot allocate memory for IPT line\n");
        return FALSE;
    }
*/
    IPT_t_p lineToDelete = *line;
    *line = NULL;
    MemoryAddress_t mem;
    mem.pageNumber = inPageNumber;
    mem.processID = inProcessID;
    int HATStartIndex = HAT_PRIVATE_Hash(mem);
    IPT_Add(HATStartIndex,inProcessID,inPageNumber,inFrame);
    free(lineToDelete);
    ASSERT_PRINT("Exiting:IPT_Replace() with return value: TRUE\n");
    return TRUE;
}