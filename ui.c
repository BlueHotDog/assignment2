#include "ui.h"
#include "freelist.h"
#include "pcb.h"

void* UI_Main()
{

    string command = calloc(MAX_INPUT_LENGTH,sizeof(char));
    while(!UI_ShouldUIThreadClose)
    {
        printf(">");
        scanf("%s",command);
        UI_ParseCommand(&command);
    }
    pthread_exit(NULL);
}


void UI_ParseCommand(const string* const comm)
{
    if(strcmp(*comm,"createProcess")==0 || strcmp(*comm,"c")==0)
    {
        UI_HandleCreateProcess();
    }
    else if(strcmp(*comm,"exit")==0)
    {
        UI_SignalUIThreadToStop();
    }
}
bool UI_CreateUIThread()
{
    ASSERT_PRINT("Entering: UI_CreateUIThread\n");
    pthread_create(&UI_Thread, NULL, UI_Main, NULL);
    return TRUE;
}
void UI_SignalUIThreadToStop()
{
    ASSERT_PRINT("Entering:UI_SignalUIThreadToStop\n");
    UI_ShouldUIThreadClose = TRUE;
}

void UI_HandleCreateProcess() {
    ASSERT_PRINT("Entering: UI_HandleCreateProcess\n");
    do
    {
    int start = FREELIST_Get();
    if(start==-1)
    {
        printf("Error, unable to find space for new thread...\n");
        break;
    }
    DISK_AllocateSpace(start,start+NumOfProcessPages);
    int id = PCB_GetFreeProcessID();
    if(id==-1)
    {
        printf("Error, unable to find free process id...\n");
        break;
    }
    PCB_AllocateProccess(id,start,start+NumOfProcessPages);
    } while(FALSE);
    
}

void UI_HandleDelProcess(PID proccessID) {

}

void UI_HandleRead(int vAddr, PID proccessID, unsigned int amount) {

}

void UI_HandleLoopRead(int vAddr,PID proccessID,int offset,unsigned int amount) {

}