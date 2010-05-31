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

bool IPT_CreateIPT_t_p(
        PID processID,
        LPN pageNumber,
        MMFI frame,
        OUT IPT_t_p newIPTLine)
{
    ASSERT_PRINT("Entering:IPT_CreateIPT_t_p()\n");
    if (newIPTLine = malloc(sizeof (IPT_t)))
        return FALSE;

    newIPTLine->dirtyBit = 0;
    newIPTLine->frame = frame;
    newIPTLine->next = NULL;
    newIPTLine->pageNumber = pageNumber;
    newIPTLine->processID = processID;
    newIPTLine->referenceBit = 0;
    ASSERT_PRINT("Exiting:IPT_CreateIPT_t_p()\n");
    return TRUE;
}

bool IPT_Add(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber,
        MMFI frame)
{
    ASSERT_PRINT("Entering:IPT_Add()\n");

    IPT_t_p newIPTLine;
    ASSERT(IPT_CreateIPT_t_p(processID, pageNumber, frame, newIPTLine));
    IPT_t_p pointer = IPT[HATPointedIndex];
    if (pointer == NULL) //the field was never invoked.
    {
        newIPTLine->prev = 0;
        newIPTLine->next = 0;
        pointer = newIPTLine;
        return TRUE;
    }

    int iterations = 0;
    bool foundFrame = FALSE;
    while (!foundFrame) {
        while (IPT[HATPointedIndex] != NULL || iterations <= SIZE_OF_IPT) {
            INDEX_INC(HATPointedIndex);
            iterations++;
        }
        if (iterations > SIZE_OF_IPT) {
            //throw "segmentation fault - full MM" to the PRM
            QueueCommand_t_p command = malloc(sizeof (QueueCommand_t));
            command->params = calloc(2, sizeof (int));
            command->params[0] = processID;
            command->params[1] = pageNumber;
            command->paramsAmount = 2;
            command->command = PRMSegmentationFaultMMIsFull;
            QUEUES_WriteToPRM(command);
        } else
            foundFrame = TRUE;
    }
    newIPTLine->next = pointer->next;
    pointer->next = newIPTLine;
    newIPTLine->prev = pointer;
    totalPagesInIPT++;
    ASSERT_PRINT("Exiting:IPT_Add()\n");
    return TRUE;
}

bool IPT_FindIPTLine(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber,
        OUT int *line)
{
    ASSERT_PRINT("Entering:IPT_FindIPTLine()\n");
    int iterations = 0;
    while (IPT[HATPointedIndex] != NULL || iterations <= SIZE_OF_IPT)
    {
        if (IPT[HATPointedIndex]->processID == processID && IPT[HATPointedIndex]->pageNumber == pageNumber)
        {
            *line = HATPointedIndex;
            ASSERT_PRINT("Exiting:IPT_FindIPTLine() with return value: TRUE\n");
            return TRUE;
        }
        INDEX_INC(HATPointedIndex);
        iterations++;
    }
    if (iterations > SIZE_OF_IPT)
    {
        //the page is not in the IPT, i.e. not in the MM
        ASSERT_PRINT("Exiting:IPT_FindIPTLine() with return value: FALSE\n");
        return FALSE;
    }
}

bool IPT_FindFrame(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber,
        OUT MMFI *frame)
{
    ASSERT_PRINT("Entering:IPT_FindFrame()\n");
    int line = -1;
    if (IPT_FindIPTLine(HATPointedIndex,processID,pageNumber,&line))
    {
        *frame = IPT[line]->frame;
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
    int line = -1;
    if (!IPT_FindIPTLine(HATPointedIndex, processID, pageNumber, &line))
    {
        // the entry is not in the IPT.
        return FALSE;
    }
    IPT_t_p toDelete = IPT[line];
    IPT_t_p father = IPT[line]->prev;
    IPT_t_p son = IPT[line]->next;
    if (!father)
    {
        IPT[line] = son;
    }
    else
    {
        father->next = son;
        son->prev = father;
    }
    free(toDelete);
    totalPagesInIPT--;
    ASSERT_PRINT("Exiting:IPT_Remove() with return value: TRUE\n");
    return TRUE;
}

bool IPT_FindEmptyFrame(OUT MMFI* frame)
{
    ASSERT_PRINT("Entering:IPT_FindEmptyFrame()\n");
    int i=0;
    bool* frameArry = calloc(SIZE_OF_IPT, sizeof(bool));
    for(i;i<SIZE_OF_IPT; i++)
        frameArry[i] = FALSE;
    for (i;i<SIZE_OF_IPT; i++)
        frameArry[IPT[i]->frame] = TRUE;

    i=0;
    while (!frameArry[i] || i<SIZE_OF_IPT) i++;

    if (i>=SIZE_OF_IPT)
    {
        ASSERT_PRINT("Exiting:IPT_FindEmptyFrame() with return value: FALSE\n");
        return FALSE;
    }
    *frame = i;
    ASSERT_PRINT("Exiting:IPT_FindEmptyFrame() with return value: TRUE, frame = %d\n",*frame);
    return TRUE;
}

bool IPT_FindLineByFrame(MMFI frame, OUT int *line)
{
    ASSERT_PRINT("Entering:IPT_FindLineByFrame()\n");
    int i=0;
    for (i;i<SIZE_OF_IPT; i++)
        if(IPT[i]->frame == frame)
        {
            *line = frame;
            ASSERT_PRINT("Exiting:IPT_FindLineByFrame() with return value: TRUE, line = %d\n",*line);
            return TRUE;
        }
    ASSERT_PRINT("Exiting:IPT_FindLineByFrame() with return value: FALSE\n");
    return TRUE;
}

bool IPT_Replace(
        PID outProcessID,
        LPN outPageNumber,
        PID inProcessID,
        LPN inPageNumber,
        MMFI inFrame)
{
    ASSERT_PRINT("Entering:IPT_Replace()\n");
    int line = 0;
    if(!IPT_FindIPTLine(0,outProcessID,outPageNumber, &line))
        return FALSE;

    IPT_t_p newIPTLine;
    if(IPT_CreateIPT_t_p(inProcessID,inPageNumber,inFrame,newIPTLine))
    {
        ASSERT_PRINT("Exiting:IPT_Replace() - Cannot allocate memory for IPT line\n");
        return FALSE;
    }
    IPT_t_p lineToDelete = IPT[line];
    IPT[line] = NULL;
    MemoryAddress_t mem;
    mem.pageNumber = inPageNumber;
    mem.processID = inProcessID;
    int HATStartIndex = HAT_PRIVATE_Hash(mem);
    IPT_Add(HATStartIndex,inProcessID,inPageNumber,inFrame);
    free(lineToDelete);
    ASSERT_PRINT("Exiting:IPT_Replace() with return value: TRUE\n");
    return TRUE;
}