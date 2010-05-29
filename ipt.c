#include "ipt.h"

bool IPT_Init()
{
    ASSERT_PRINT("Entering:PRM_Init()\n");
    IPT = calloc(SIZE_OF_IPT, sizeof (IPT_t));
    if (IPT == NULL)
        return FALSE;
    int i = 0;
    for (i = 0; i < SIZE_OF_IPT; i++) {
        IPT[i] = 0;
    }
    totalPagesInIPT = 0;
    ASSERT_PRINT("Exiting:PRM_Init()\n");
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
        while (pointer != NULL || iterations <= SIZE_OF_IPT) {
            INDEX_INC(HATPointedIndex);
            iterations++;
            pointer = IPT[HATPointedIndex];
        }
        if (iterations > SIZE_OF_IPT) {
            //throw segmentation fault to the PRM
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
    pointer = newIPTLine;
    ASSERT_PRINT("Exiting:IPT_Add()\n");
    return TRUE;
}