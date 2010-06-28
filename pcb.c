#include "pcb.h"


bool PCB_Init()
{
    ASSERT_PRINT("Entering:PCB_Init\n");
    PCBArray = calloc(MaxNumOfProcesses,sizeof(PCB_t));
    if(PCBArray==NULL)
        return FALSE;
    int i=0;
    for(i=0;i<MaxNumOfProcesses;i++)
        PCBArray[i].active = FALSE;
    return TRUE;
}
bool PCB_Free()
{
    ASSERT_PRINT("Entering:PCB_Free\n");
    if(PCBArray==NULL)
        return TRUE;
    free(PCBArray);
    return TRUE;
}
PCB_t_p PCB_AllocateProcess(PID id,int start,int end)
{
    ASSERT_PRINT("Entering:PCB_AllocateProcess(%d,%d,%d)\n",id,start,end);
    PCBArray[id].end=end;
    PCBArray[id].processID = id;
    PCBArray[id].start = start;
    PCBArray[id].active = TRUE;
    return &PCBArray[id];
}
int PCB_GetFreeProcessID()
{
    ASSERT_PRINT("Entering:PCB_GetFreeProcessID\n");
    int i=0;
    for(i=0;i<MaxNumOfProcesses;i++)
        if(PCBArray[i].active == FALSE)
            return i;
    return -1;
}

bool PCB_DeAllocateProcess(PID id)
{
    PCB_t_p process = PCB_GetByProcessID(id);
    if(process!=NULL)
    {
        process->active = FALSE;
        return TRUE;
    }
    return FALSE;
}
PCB_t_p PCB_GetByProcessID(PID id)
{
    ASSERT_PRINT("Entering:PCB_GetByProcessID(%d)\n",id);
    int i=0;
    for(i=0;i<MaxNumOfProcesses;i++)
        if(PCBArray[i].processID == id)
        {
            ASSERT_PRINT("Exiting:PCB_GetByProcessID(%d)->%d\n",id,i);
            return &PCBArray[i];
        }
    ASSERT_PRINT("Exiting:PCB_GetByProcessID(%d)->NULL\n",id);
    return NULL;
}
