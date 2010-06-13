#include "process.h"
#include "mmu.h"

void* PROCESS_RUN(void* pcb) {
    PCB_t_p local_pcb = (PCB_t_p) pcb;
    bool close = FALSE;
    while (TRUE) {
        QueueCommand_t_p comm = QUEUES_ReadProcess(local_pcb->processID);
        switch (comm->command) {
            case ProcessReadAddress:
            {
                int vAddr = comm->params[0];
                int amount = comm->params[1];
                //string stringToWtrite = comm->stringParams[0];
                int startPageNum = vAddr / PageSize;
                int offsetFromBeginingOfPage = vAddr % PageSize;
                int timesToRun = ((offsetFromBeginingOfPage + amount) / PageSize) + (((offsetFromBeginingOfPage + amount) % PageSize > 0) ? 1 : 0);
                int i = 0;
                int bitIndex = offsetFromBeginingOfPage;
                int ansIndex = 0;
                FILE* toWrite = stdout;
                if (comm->voidParamsAmount == 1) {
                    toWrite = comm->voidParams[0];
                }
                char stringToRead[amount+1];
                for (i = 0; i < timesToRun; i++) {
                    if (startPageNum + i < NumOfProcessPages) {
                        MemoryAddress_t mem;
                        mem.processID = local_pcb->processID;
                        mem.pageNumber = startPageNum + i;
                        int bitsToRead = 0;
                        if((i + 1) * PageSize <= amount)
                            if(i == 0)
                                bitsToRead = (PageSize - offsetFromBeginingOfPage);
                            else
                                bitsToRead = PageSize;
                        else
                            if (i==0)
                                bitsToRead = amount;
                            else
                                bitsToRead = vAddr+amount - i*PageSize;
                        int startingFrom = 0;
                        if(i==0)
                            startingFrom = offsetFromBeginingOfPage;
                        else
                            startingFrom = 0;
                        int charIndex = 0;
                        for (charIndex; charIndex < bitsToRead; charIndex++) {

                            MM_MemoryReference();
                            stringToRead[ansIndex] = MMU_ReadAddress(mem,bitIndex);
                            bitIndex = (bitIndex + 1) % PageSize;
                            ansIndex++;
                        }
                    }
                }
                stringToRead[ansIndex] = 0;
                fprintf(toWrite, "%s\n", stringToRead);
                if (comm->voidParamsAmount == 1) {
                    fclose(toWrite);
                }
                //ASSERT(DISK_PrintContent());
            }
                break;
            case ProcessWriteToAddress:
            {
                int vAddr = comm->params[0];
                int amount = comm->params[1];
                string stringToWtrite = comm->stringParams[0];
                int startPageNum = vAddr / PageSize;
                int offsetFromBeginingOfPage = vAddr % PageSize;
                int timesToRun = ((offsetFromBeginingOfPage + amount) / PageSize) + (((offsetFromBeginingOfPage + amount) % PageSize > 0) ? 1 : 0);
                int i = 0;
                int stringIndex = 0;
                for (i = 0; i < timesToRun; i++) {
                    if (startPageNum + i < NumOfProcessPages) {
                        MemoryAddress_t mem;
                        mem.processID = local_pcb->processID;
                        mem.pageNumber = startPageNum + i; 
                        int bitsToWrite = 0;
                        if((i + 1) * PageSize <= amount)
                            if(i == 0)
                                bitsToWrite = (PageSize - offsetFromBeginingOfPage);
                            else
                                bitsToWrite = PageSize;
                        else
                            if (i==0)
                                bitsToWrite = amount;
                            else
                                bitsToWrite = vAddr+amount - i*PageSize;
                        char pageToWrite[PageSize];
                        int startingFrom = 0;
                        if(i==0)
                            startingFrom = offsetFromBeginingOfPage;
                        else
                            startingFrom = 0;
                        int charIndex = 0;
                        for (charIndex; charIndex < bitsToWrite; charIndex++) {
                            
                            MM_MemoryReference();
                            pageToWrite[startingFrom+charIndex] = stringToWtrite[stringIndex];
                            stringIndex++;
                        }
                        MMU_WriteToAddress(mem, pageToWrite, bitsToWrite, startingFrom);
                    }
                }
            }
                break;
            case ProcessClose:
            {
                QUEUES_FreeCommand(comm);
                close = TRUE;
                break;
            }
        }
        if (close)
            break;
        QUEUES_FreeCommand(comm);

    }
    //
    //pthread_mutex_lock(&Aging_mutex);
    //sem_wait(&PRM_mutex);
    pthread_mutex_lock(&MM_Counter_Mutex);
    PROCESS_DeInit(local_pcb->processID);
    pthread_mutex_unlock(&MM_Counter_Mutex);
    //pthread_mutex_unlock(&Aging_mutex);
    //sem_post(&PRM_mutex);
    // // leave critical section
}

void PROCESS_DeInit(PID id) {
    ASSERT_PRINT("Entering: PROCESS_DeInit(id:%d)\n", id);

    int i = 0;



    FREELIST_SetNotTaken(PCBArray[id].start);
    PCB_GetByProcessID(id)->active = FALSE;

    //should remove from memory all info..
    ASSERT_PRINT("Exiting: PROCESS_DeInit(id:%d)\n", id);
    //pthread_exit(NULL);
}

int PROCESS_CREATE() {
    ASSERT_PRINT("Entering: PROCESS_CREATE\n");
    do {
        int start = FREELIST_Get();
        if (start == -1) {
            fprintf(outFile, "Error, unable to find space for new thread...\n");
            break;
        }
        DISK_AllocateSpace(start, start + NumOfProcessPages);
        int id = PCB_GetFreeProcessID();
        if (id == -1) {
            int i = start;
            for (i; i < start + NumOfProcessPages; i++) {
                if (FreeList[i].isFree == TRUE) {
                    fprintf(outFile, "Error allocating space for process\n");
                    exit(-1);
                }
            }
            FREELIST_SetNotTaken(start);
            //DISK_DeAllocateSpace(start, start + NumOfProcessPages);
            fprintf(outFile, "Error, unable to find free process id...\n");
            break;
        }
        PCB_t_p pcb = PCB_AllocateProcess(id, start, start + NumOfProcessPages);

        pthread_create(&(pcb->processThread), NULL, PROCESS_RUN, (void*) pcb);
        return id;
    } while (FALSE);
    return -1;
}


/*
void PROCESS_STOP() {
    PROCESS_ShouldClose = TRUE;
}
 */

/*
bool PROCESS_Read(PID processID, int vAddr, int amount) {
    int i = vAddr;
    for (i; i < (vAddr + amount); i++) {

    }
}
 */
