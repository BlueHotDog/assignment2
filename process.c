#include "process.h"

void* PROCESS_RUN(void* pcb) {
    PCB_t_p local_pcb = (PCB_t_p)pcb;
    int counter = 1;
    while (!PROCESS_ShouldClose) {
        QueueCommand_t_p comm = malloc(sizeof(QueueCommand_t));
        comm->params = calloc(2,sizeof(int));
        comm->params[0] = counter++;
        comm->params[1] = 56;
        comm->paramsAmount = 2;
        
        if(local_pcb->proccessID%2==0)
            comm->command = PRMReadAddress;
        else
            comm->command = PRMWriteToAddress;

        QUEUES_WriteToPRM(comm);
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
        PCB_t_p pcb = PCB_AllocateProccess(id, start, start + NumOfProcessPages);
        
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