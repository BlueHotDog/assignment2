#include "hat.h"

bool HAT_Init() {
    ASSERT_PRINT("Entering:HAT_Init()\n");
    HAT = calloc(NumOfPagesInMM, sizeof (IPT_t));
    int i = 0;
    for (i = 0; i < NumOfPagesInMM; i++) {
        HAT[i] = 0;
    }
    if (!HAT)
        return FALSE;
    else
        return TRUE;
    ASSERT_PRINT("Exiting:HAT_Init()\n");
}

int HAT_PRIVATE_Hash(MemoryAddress_t memoryAddress) {
    ASSERT_PRINT("Entering:HAT_PRIVATE_Hash(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
    ASSERT_PRINT("Exiting:HAT_PRIVATE_Hash(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
    return (memoryAddress.processID+1 * memoryAddress.pageNumber+1) % NumOfPagesInMM;
}

IPT_t_p HAT_GetEntry(MemoryAddress_t memoryAddress) {
    ASSERT_PRINT("Entering:HAT_GetEntry(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
    int index = HAT_PRIVATE_Hash(memoryAddress);
    if(HAT[index]==0)
    {
        HAT[index] = index;
    }
    ASSERT_PRINT("Exiting:HAT_GetEntry(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
    return IPT[index];
}

void HAT_Print()
{
    ASSERT_PRINT("Entering: HAT_Print()\n");
    int i=0;
    for (i = 0; i < NumOfPagesInMM; i++) {
        int j=0;
        
        if(HAT[i] == 0)
            printf("HAT[%d])Null\n",i);
        else
        {
            IPT_t_p entery = IPT[HAT[i]];
            printf("HAT[%d])",i);
            while(entery!=NULL)
            {
                printf("PID:%d,PageNumber:%d,Frame:%d,Dirty:%s,Reference:%s ",entery->processID,entery->pageNumber,entery->frame,(entery->dirtyBit)?"true":"false",(entery->referenceBit)?"true":"false");
                if(entery->next!=NULL)
                    printf("-->");
                entery = entery->next;
            }
        }
    }
    ASSERT_PRINT("Exiting: HAT_Print()\n");
}