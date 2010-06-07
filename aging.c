#include <semaphore.h>

#include "aging.h"


bool AGING_Init() {
    ASSERT_PRINT("Entering:AGING_Init()\n");
    if (pthread_create(&Aging, NULL, AGING_Main, NULL) != 0)
        return FALSE;
    Aging_Registers = calloc(NumOfPagesInMM, sizeof (unsigned int));
    int i = 0;
    for (i = 0; i < NumOfPagesInMM; i++)
        Aging_Registers[i] = 0;
    sem_init(&Aging_mutex, 0, 0);
    ASSERT_PRINT("Exiting:AGING_Init()\n");
    return TRUE;
}

void AGING_Close() {
    ASSERT_PRINT("Entering:AGING_Close()\n");
    AGING_ShouldClose = TRUE;
    ASSERT_PRINT("Exiting:AGING_Close()\n");
}

void AGING_DeInit() {
    ASSERT_PRINT("Entering:AGING_DeInit()\n");
    //TODO:Add free memory stuff here..
    ASSERT_PRINT("Exiting:AGING_DeInit()\n");
}

void* AGING_Main() {
    ASSERT_PRINT("Entering:AGING_Main()\n");
    int i = 0;
    unsigned int m = ((unsigned int) - 1 >> 1) + 1; //a number with its msb set to 1
    while (!AGING_ShouldClose) {
        sem_wait(&Aging_mutex);
        ASSERT_PRINT("Aging deamon kicks in...\n");
        for (i = 0; i < NumOfPagesInMM; i++) {
            Aging_Registers[i] >>= 1;
            if (IPT[i] != NULL && IPT[i]->referenceBit == TRUE)
                Aging_Registers[i] |= m;
            if(IPT[i]!=NULL)
                IPT_UpdateReferencetyBit(IPT[i]->frame,FALSE);
        }
        ASSERT_PRINT("Aging deamon finished...\n");
        sem_post(&MM_Counter_Mutex);
    }
    AGING_DeInit();
    ASSERT_PRINT("Exiting:AGING_Main()\n");
}
