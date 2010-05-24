#include "ui.h"


void* UI_Main() {

    string command = calloc(MAX_INPUT_LENGTH, sizeof (char));
    while (!UI_ShouldUIThreadClose) {
        printf(">");
        scanf("%s", command);
        UI_ParseCommand(&command);
    }
    pthread_exit(NULL);
}

void UI_ParseCommand(const string * const comm) {
    ASSERT_PRINT("Entering: UI_ParseCommand(%s)\n",*comm);
    if (strcmp(*comm, "createProcess") == 0 || strcmp(*comm, "c") == 0) {
        UI_HandleCreateProcess();
    } else if(strcmp(*comm,"read")==0){
        int vAddr = -1;
        int id = -1;
        int amount = -1;
        scanf("%d %d %d",&vAddr,&id,&amount);
        UI_HandleRead(vAddr,id,amount);
    } else if (strcmp(*comm, "exit") == 0) {
        UI_SignalUIThreadToStop();
    }
}

bool UI_CreateUIThread() {
    ASSERT_PRINT("Entering: UI_CreateUIThread\n");
    pthread_create(&UI_Thread, NULL, UI_Main, NULL);
    return TRUE;
}

void UI_SignalUIThreadToStop() {
    ASSERT_PRINT("Entering:UI_SignalUIThreadToStop\n");
    UI_ShouldUIThreadClose = TRUE;
}

void UI_HandleCreateProcess() {
    ASSERT_PRINT("Entering: UI_HandleCreateProcess\n");
    int id = PROCESS_CREATE();
    if (id != -1)
        printf("%d\n", id);
    else
        printf("Error creating process\n");

}

void UI_HandleDelProcess(PID proccessID) {

}

void UI_HandleRead(int vAddr, PID proccessID, unsigned int amount) {
    ASSERT_PRINT("Entering: UI_HandleRead(%d,%d,%d)\n",vAddr,proccessID,amount);
    mqd_t queue = QUEUE_OpenForProcess(proccessID);
    QUEUE_Send(queue,"read bluh bluh");
    QUEUE_Close(queue);
/*
    string name = calloc(MAX_MESSAGE_SIZE,sizeof(char));
    sprintf(name,"/%d",proccessID);
    mqd_t ret = mq_open(name, O_RDWR,(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH),NULL);
    string message = "read bliuh bluhhh";
    int messageLength = mq_send(ret, message, strlen(message),0);
*/
   
}

void UI_HandleLoopRead(int vAddr, PID proccessID, int offset, unsigned int amount) {

}