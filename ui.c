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
    } else if (strcmp(*comm, "loopRead") == 0) {
        //loopRead vAddr id off amount
        int vAddr = -1;
        int id = -1;
        int amount = -1;
        int offset = -1;
        int i = 0;
        scanf("%d %d %d %d", &vAddr, &id, &offset, &amount);
        UI_HandleLoopRead(vAddr, id, offset + i, 1);

    } else if (strcmp(*comm, "write") == 0) {
        //write vAddr id s
        int vAddr = -1;
        int id = -1;
        string val = 0;
        scanf("%d %d %s", &vAddr, &id, val);
        UI_HandleWrite(vAddr, id, val);
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
    addr.processID = processID;
    QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));

    comm->command = ProcessReadAddress;
    comm->params = calloc(1, sizeof (int));
    comm->params[0] = addr.pageNumber;
    comm->paramsAmount = 1;

    QUEUES_WriteToProcess(processID, comm);
    ASSERT_PRINT("Exiting: UI_HandleRead(%d,%d,%d)\n", vAddr, processID, amount);
}

void UI_HandleLoopRead(int vAddr, PID processID, int offset, unsigned int amount) {
    int i = 0;
    for (i; i < amount; i++)
        UI_HandleRead(vAddr + i * offset, processID, 1);
}

void UI_HandleReadToFile(int vAddr, PID processID, unsigned int amount, string filename) {
    ASSERT_PRINT("Entering: UI_HandleReadToFile(vAddr:%d,processID:%d,amount:%d,fileName:%s)\n", vAddr, processID, amount, filename);

    FILE* fp;
    if ((fp = freopen("OUT", "w", stdout)) != NULL) {
        UI_HandleRead(vAddr, processID, amount);
        fclose(fp);
    }
    ASSERT_PRINT("Exiting: UI_HandleReadToFile(vAddr:%d,processID:%d,amount:%d,fileName:%s)\n", vAddr, processID, amount, filename);
}

void UI_HandleLoopReadToFile(int vAddr, PID processID, int off, unsigned int amount, string filename) {
    ASSERT_PRINT("Entering: UI_HandleLoopReadToFile(vAddr:%d,processID:%d, off:%d, amount:%d, fileName:%s)\n", vAddr, processID, off, amount, filename);

    FILE* fp;
    if ((fp = freopen("OUT", "w", stdout)) != NULL) {
        UI_HandleLoopRead(vAddr, processID, off, amount);
        fclose(fp);
    }
    ASSERT_PRINT("Exiting: UI_HandleLoopReadToFile(vAddr:%d,processID:%d, off:%d, amount:%d, fileName:%s)\n", vAddr, processID, off, amount, filename);
}

void UI_HandleWrite(int vAddr, PID processID, string s) {
    ASSERT_PRINT("Entering: UI_HandleWrite(vAddr:%d, processID:%d, content:%s)\n", vAddr, processID, s);

    MemoryAddress_t addr;
    addr.pageNumber = vAddr;
    addr.processID = processID;

    QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));

    comm->command = ProcessWriteToAddress;
    comm->params = calloc(1, sizeof (int));
    comm->stringParams =calloc(1,sizeof(string));
    comm->params[0] = addr.pageNumber;
    comm->stringParams[0] = s;
    comm->paramsAmount = 1;
    comm->stringParamsAmount = 1;
    
    QUEUES_WriteToProcess(processID, comm);

    ASSERT_PRINT("Exiting: UI_HandleWrite(vAddr:%d, processID:%d, content:%s)\n", vAddr, processID, s);
}

void UI_HandleLoopWrite(int vAddr, PID processID, char c, int off, unsigned int amount) {

}