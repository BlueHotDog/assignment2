#include <semaphore.h>

#include "aging.h"

bool AGING_Init() {
    ASSERT_PRINT("Entering:AGING_Init()\n");

    Aging_Registers = calloc(NumOfPagesInMM, sizeof (unsigned int));
    int i = 0;
    for (i = 0; i < NumOfPagesInMM; i++)
        Aging_Registers[i] = 0;
    pthread_mutex_init(&Aging_mutex, 0);
    if (pthread_create(&Aging, NULL, AGING_Main, NULL) != 0)
        return FALSE;
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
    free(Aging_Registers);
    ASSERT_PRINT("Exiting:AGING_DeInit()\n");
}

void* AGING_Main() {
    ASSERT_PRINT("Entering:AGING_Main()\n");
    int i = 0;
    unsigned int m = ((unsigned int) - 1 >> 1) + 1; //a number with its msb set to 1
    while (!AGING_ShouldClose) {
        pthread_mutex_lock(&Aging_mutex);
        if (IPT != NULL) {
            ASSERT_PRINT("Aging deamon kicks in...\n");
            for (i = 0; i < NumOfPagesInMM; i++) {
                Aging_Registers[i] >>= 1;
                if (IPT[i] != NULL && IPT[i]->referenceBit == TRUE)
                {
                    Aging_Registers[IPT[i]->frame] |= m;
                }
                if (IPT[i] != NULL)
                    IPT_UpdateReferencetyBit(IPT[i]->frame, FALSE);
            }
        }
        ASSERT_PRINT("Aging deamon finished...\n");
        pthread_mutex_unlock(&MM_Counter_Mutex);
    }
    AGING_DeInit();
    ASSERT_PRINT("Exiting:AGING_Main()\n");
}
