#include "process.h"
#include "mmu.h"

void* PROCESS_RUN(void* pcb) {
    PCB_t_p local_pcb = (PCB_t_p) pcb;

    while (!PROCESS_ShouldClose) {
        QueueCommand_t_p comm = QUEUES_ReadProcess(local_pcb->processID);
        switch (comm->command) {
            case ProcessReadAddress:
            {
                int vAddr = comm->params[0];
                int amount = comm->params[1];
                FILE* toWrite = stdout;
                if (comm->voidParamsAmount == 1) {
                    toWrite = comm->voidParams[0];
                }
                int timesToRun = (amount / PageSize) + ((amount % PageSize > 0) ? 1 : 0);
                int i = 0;
                int indexInRes = 0;
                string toPrint = calloc(amount + 1, sizeof (char));
                for (i = 0; i < timesToRun; i++) {
                    if (vAddr + i < NumOfProcessPages) {
                        MemoryAddress_t mem;
                        mem.processID = local_pcb->processID;
                        mem.pageNumber = vAddr + i;

                        Page res = MMU_ReadAddress(mem);
                        int ii = 0;
                        int temp = (amount < PageSize) ? amount : PageSize;
                        for (ii; ii < temp; ii++) {
                            toPrint[indexInRes] = res[ii];
                            // fprintf(outFile,"%c|", res[ii]);
                            indexInRes++;

                        }
                        amount -= PageSize;
                    }

                }
                toPrint[indexInRes] = 0;
                fprintf(toWrite, "%s\n", toPrint);
                if (comm->voidParamsAmount == 1) {
                    fclose(toWrite);
                }

                DISK_PrintContent();
            }
                break;
            case ProcessWriteToAddress:
            {

            }
            break;
        }
        free(comm->stringParams);
        free(comm->voidParams);
    }
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
            DISK_DeAllocateSpace(start, start + NumOfProcessPages);
            fprintf(outFile, "Error, unable to find free process id...\n");
            break;
        }
        PCB_t_p pcb = PCB_AllocateProcess(id, start, start + NumOfProcessPages);

        pthread_create(&(pcb->processThread), NULL, PROCESS_RUN, (void*) pcb);
        return id;
    } while (FALSE);
    return -1;
}

void PROCESS_STOP() {
    PROCESS_ShouldClose = TRUE;
}

bool PROCESS_Read(PID processID, int vAddr, int amount) {
    int i = vAddr;
    for (i; i < (vAddr + amount); i++) {

    }
}
