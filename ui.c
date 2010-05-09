#include "ui.h"

void UI_Main()
{

    string command = calloc(MAX_INPUT_LENGTH,sizeof(char));
    while(!UI_ShouldUIThreadClose && strcmp(command,"exit")!=0)
    {
        printf(">");
        scanf("%s",command);
    }
    pthread_exit(NULL);

}

bool UI_CreateUIThread()
{
    //pthread_create(&UI_Thread, NULL, UI_Main, NULL);
}
void UI_SignalUIThreadToStop()
{
    UI_ShouldUIThreadClose = TRUE;
}

void UI_HandleCreateProcess() {

}

void UI_HandleDelProcess(PID proccessID) {

}

void UI_HandleRead(int vAddr, PID proccessID, unsigned int amount) {

}

void UI_HandleLoopRead(int vAddr,PID proccessID,int offset,unsigned int amount) {

}