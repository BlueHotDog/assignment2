#include "process.h"
#include "mmu.h"

void* PROCESS_RUN(void* pcb) {
    PCB_t_p local_pcb = (PCB_t_p)pcb;

    while (!PROCESS_ShouldClose) {
        QueueCommand_t_p comm = QUEUES_ReadProcess(local_pcb->processID);
        switch(comm->command){
            case ProcessReadAddress:
            {
                MemoryAddress_t mem;
                mem.processID = local_pcb->processID;
                mem.pageNumber = comm->params[0];
                Page res = MMU_ReadAddress(mem);
                int i=0;
                for(i;i<comm->params[1];i++)
                    printf("%c\n\n\n",res[i]);
            }
                break;
        }
        free(comm->params);
        free(comm);
    }
}

int PROCESS_CREATE() {
    ASSERT_PRINT("Entering: PROCESS_CREATE\n");
    do {
        int start = FREELIST_Get();
        if (start == -1) {
            printf("Error, unable to find space for new thread...\n");
            break;
        }
        DISK_AllocateSpace(start, start + NumOfProcessPages);
        int id = PCB_GetFreeProcessID();
        if (id == -1) {
            DISK_DeAllocateSpace(start,start+NumOfProcessPages);
            printf("Error, unable to find free process id...\n");
            break;
        }
        PCB_t_p pcb = PCB_AllocateProcess(id, start, start + NumOfProcessPages);
        
        pthread_create(&(pcb->processThread),NULL,PROCESS_RUN,(void*)pcb);
        return id;
    } while (FALSE);
    return -1;
}

void PROCESS_STOP() {
    PROCESS_ShouldClose = TRUE;
}
bool PROCESS_Read(PID processID,int vAddr, int amount)
{
    int i=vAddr;
    for(i;i<(vAddr+amount);i++)
    {
        
    }
}
