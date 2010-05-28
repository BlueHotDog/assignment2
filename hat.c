#include "hat.h"

bool HAT_Init() {
    ASSERT_PRINT("Entering:HAT_Init()\n");
    HAT = calloc(NumOfPagesInMM, sizeof (IPT_t));
    int i = 0;
    for (i = 0; i < NumOfPagesInMM; i++) {
        HAT[i] = NULL;
    }
    if (!HAT)
        return FALSE;
    else
        return TRUE;
    ASSERT_PRINT("Exiting:HAT_Init()\n");
}

int HAT_PRIVATE_Hash(MemoryAddress_t memoryAddress) {
    ASSERT_PRINT("Entering:HAT_PRIVATE_Hash(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
    return (memoryAddress.processID * memoryAddress.pageNumber) % NumOfPagesInMM;
    ASSERT_PRINT("Exiting:HAT_PRIVATE_Hash(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
}

IPT_t_p HAT_GetEntry(MemoryAddress_t memoryAddress) {
    ASSERT_PRINT("Entering:HAT_GetEntry(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
    int index = HAT_PRIVATE_Hash(memoryAddress);
    if(HAT[index]==NULL)
    {
        HAT[index] = IPT[index];
    }
    return HAT[index];
    ASSERT_PRINT("Exiting:HAT_GetEntry(pid:%d,addr:%d)\n", memoryAddress.processID, memoryAddress.pageNumber);
}