#include "process.h"
#include "pcb.h"
#include "disk.h"
#include "messagequeues.h"

void* PROCESS_RUN(void* pcb) {
    PCB_t_p local_pcb = (PCB_t_p)pcb;
   /*  string name = calloc(MAX_MESSAGE_SIZE,sizeof(char));
    sprintf(name,"/%d",local_pcb->proccessID);
   printf("Opening message queue %s for process %d\n",name,local_pcb->proccessID);

    struct mq_attr attr;
    memset(&attr,0,sizeof(struct mq_attr));
    attr.mq_maxmsg= MAX_MESSAGE_SIZE;
    //attr.mq_msgsize= MAX_MESSAGE_SIZE;
*/

     //mqd_t queue = QUEUE_OpenForProcess(local_pcb->proccessID);
    mqd_t queue = QUEUE_OpenForProcess(local_pcb->proccessID);
    while (!PROCESS_ShouldClose) {
        //QUEUE_Send(queue,"123");
        string read = QUEUE_Read(queue);
        printf("READ:%s\n",read);
        free(read);
    }
    QUEUE_Close(queue);
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
        mqd_t ret = QUEUE_CreateForProcess(id);
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
bool PROCESS_Read(PID processID,int vAddr, int amount)
{
    int i=vAddr;
    for(i;i<(vAddr+amount);i++)
    {
        
    }
}