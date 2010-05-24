#include "process.h"
#include "pcb.h"
#include "disk.h"

void* PROCESS_RUN(void* pcb) {
    PCB_t_p local_pcb = (PCB_t_p)pcb;
    while (!PROCESS_ShouldClose) {
        //printf("runing process %d\n",local_pcb->proccessID);
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
            printf("Error, unable to find free process id...\n");
            break;
        }
        PCB_t_p pcb = PCB_AllocateProccess(id, start, start + NumOfProcessPages);

        string name = calloc(50,sizeof(char));
        sprintf(name,"/%d",id);
        printf("Opening message queue %s for process %d\n",name,id);
        mqd_t ret = mq_open(name, O_RDWR | O_CREAT,(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH),NULL);
        if(ret==-1)
        {
            printf("Error opening message queue.\n");
            PCB_DeAllocateProccess(id);
            DISK_DeAllocateSpace(start,start+NumOfProcessPages);
            break;
        }
        pthread_create(&(pcb->processThread),NULL,PROCESS_RUN,(void*)pcb);
        return id;
    } while (FALSE);
    return -1;
}

void PROCESS_STOP() {
    PROCESS_ShouldClose = TRUE;
}
