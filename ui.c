#include "ui.h"
#include "mmu.h"

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
    ASSERT_PRINT("Entering: UI_ParseCommand(%s)\n", *comm);
    if (strcmp(*comm, "createProcess") == 0 || strcmp(*comm, "c") == 0) {
        UI_HandleCreateProcess();
    } else if (strcmp(*comm, "read") == 0 || strcmp(*comm, "r") == 0) {
        int vAddr = -1;
        int id = -1;
        int amount = -1;
        scanf("%d %d %d", &vAddr, &id, &amount);
        UI_HandleRead(vAddr, id, amount);
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

void UI_HandleDelProcess(PID processID) {

}

void UI_HandleRead(int vAddr, PID processID, unsigned int amount) {
    ASSERT_PRINT("Entering: UI_HandleRead(%d,%d,%d)\n", vAddr, processID, amount);
    MemoryAddress_t addr;
    addr.pageNumber = vAddr;
    for(addr.pageNumber;addr.pageNumber<vAddr+amount;addr.pageNumber++)
    {
        QueueCommand_t_p comm = malloc(sizeof(QueueCommand_t));

        comm->command = ProcessReadAddress;
        comm->params = calloc(1,sizeof(int));
        comm->params[0] = addr.pageNumber;
        comm->paramsAmount = 1;

        QUEUES_WriteToProcess(processID,comm);
    }
    ASSERT_PRINT("Exiting: UI_HandleRead(%d,%d,%d)\n", vAddr, processID, amount);
}

void UI_HandleLoopRead(int vAddr, PID processID, int offset, unsigned int amount) {

}