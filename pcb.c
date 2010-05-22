#include "pcb.h"


bool PCB_Init()
{
    ASSERT_PRINT("Entering:PCB_Init\n");
    PCBArray = calloc(MaxNumOfProcesses,sizeof(PCB_t));

    if(PCBArray==NULL)
        return FALSE;
    int i=0;
    for(i=0;i<MaxNumOfProcesses;i++)
        PCBArray[i].Active = FALSE;
    return TRUE;
}
PCB_t PCB_AllocateProccess(PID id,int start,int end)
{
    ASSERT_PRINT("Entering:PCB_AllocateProccess(%d,%d,%d)\n",id,start,end);
    PCBArray[id].end=end;
    PCBArray[id].proccessID = id;
    PCBArray[id].start = start;
    PCBArray[id].Active = TRUE;
    return PCBArray[id];
}
int PCB_GetFreeProcessID()
{
    ASSERT_PRINT("Entering:PCB_GetFreeProcessID\n");
    int i=0;
    for(i=0;i<MaxNumOfProcesses;i++)
        if(PCBArray[i].Active == FALSE)
            return i;
    return -1;
}