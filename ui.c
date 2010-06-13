#include "ui.h"
#include "mmu.h"

void* UI_Main() {

    string command = calloc(MAX_INPUT_LENGTH, sizeof (char));
    int t = 0;
    while (!UI_ShouldUIThreadClose) {
        fprintf(outFile, ">");
        if (fscanf(inFile, "%s", command) != EOF)
            UI_ParseCommand(&command);
        else {
            fclose(inFile);
            inFile = stdin;

        }
    }
    pthread_exit(NULL);
}

void UI_ParseCommand(const string * const comm) {
    ASSERT_PRINT("Entering: UI_ParseCommand(%s)\n", *comm);
    printf("Command:%s\n", *comm);
    if (strcmp(*comm, "createProcess") == 0 || strcmp(*comm, "c") == 0) {
        UI_HandleCreateProcess();
    } else if (strcmp(*comm, "read") == 0 || strcmp(*comm, "r") == 0) {
        int vAddr = -1;
        int id = -1;
        int amount = -1;
        fscanf(inFile, "%d %d %d", &vAddr, &id, &amount);
        UI_HandleRead(vAddr, id, amount);
    } else if (strcmp(*comm, "loopRead") == 0) {
        //loopRead vAddr id off amount
        int vAddr = -1;
        int id = -1;
        int amount = -1;
        int offset = -1;
        fscanf(inFile, "%d %d %d %d", &vAddr, &id, &offset, &amount);
        UI_HandleLoopRead(vAddr, id, offset, amount);
    } else if (strcmp(*comm, "readToFile") == 0) { //readToFile vAddr id amount filename
        int vAddr = 0;
        int id = 0;
        int amount = 0;
        string fileName = calloc(60, sizeof (char));
        fscanf(inFile, "%d %d %d %60s", &vAddr, &id, &amount, fileName);
        UI_HandleReadToFile(vAddr, id, amount, fileName);
        free(fileName);
    } else if (strcmp(*comm, "loopReadToFile") == 0) { //loopReadToFile vAddr id off amount filename
        int vAddr = -1;
        int id = -1;
        int amount = -1;
        int offset = -1;
        string filename = calloc(60, sizeof (char));
        fscanf(inFile, "%d %d %d %d %60s", &vAddr, &id, &offset, &amount, filename);
        UI_HandleLoopReadToFile(vAddr, id, offset, amount, filename);
        free(filename);
    } else if (strcmp(*comm, "write") == 0 || strcmp(*comm, "w") == 0) {
        //write vAddr id s
        int vAddr = -1;
        int id = -1;
        string str = calloc(60, sizeof (char));

        fscanf(inFile, "%d %d %60s", &vAddr, &id, str);
        UI_HandleWrite(vAddr, id, str);
        free(str);
    } else if (strcmp(*comm, "loopWrite") == 0) { //loopWrite vAddr id c off amount
        int vAddr = -1;
        int id = -1;
        char c = 0;
        int offset = -1;
        int amount = -1;
        fscanf(inFile, "%d %d %c %d %d", &vAddr, &id, &c, &offset, &amount);
        UI_HandleLoopWrite(vAddr, id, c, offset, amount);
    } else if (strcmp(*comm, "exit") == 0) {
        UI_SignalUIThreadToStop();
    } else if (strcmp(*comm, "delProcess") == 0) { //void UI_HandleDelProcess(PID processID);
        int id = -1;
        fscanf(inFile, "%d", &id);
        UI_HandleDelProcess(id);
    } else if (strcmp(*comm, "printHat") == 0) {
        UI_HandlePrintHat();
    } else if (strcmp(*comm, "printMMU") == 0) {
        UI_HandlePrintMMUTable();
    } else if (strcmp(*comm, "monitor") == 0) {
        UI_HandleMonitor();
    } else if (strcmp(*comm, "noMonitor") == 0) {
        UI_HandleNoMonitor();
    } else if (strcmp(*comm, "batchFile") == 0) {
        string file = calloc(60, sizeof (char));
        fscanf(inFile, "%60s", file);
        UI_HandleBatchFile(file);
        free(file);
    } else if (strcmp(*comm, "printMM") == 0) {
        UI_HandlePrintMM();
    } else if (strcmp(*comm, "hitRate") == 0) {
        UI_HandleHitRate();
    } else if (strcmp(*comm, "printRegisters") == 0) {
        UI_HandlePrintRegisters();
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
    ASSERT_PRINT("Exiting:UI_SignalUIThreadToStop\n");
}

void UI_HandleCreateProcess() {
    ASSERT_PRINT("Entering: UI_HandleCreateProcess\n");
    int id = PROCESS_CREATE();
    if (id != -1)
        fprintf(outFile, "%d\n", id);
    else
        fprintf(outFile, "Error creating process\n");

}

void UI_HandleDelProcess(PID processID) {
    ASSERT_PRINT("Entering: UI_HandleDelProcess(%d)\n", processID);
    if(PCBArray[processID].active == FALSE)
    {
        fprintf(stderr, "WARNING! tring to kill a dead process: %d\n",processID);
        return;
    }
    QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));
    comm->command = ProcessClose;
    if (outFile != stdout) {
        comm->voidParams = calloc(1, sizeof (void*));
        comm->voidParams[0] = outFile;
        comm->voidParamsAmount = 1;
    } else {
        comm->voidParamsAmount = 0;
    }
    comm->stringParamsAmount = 0;
    comm->paramsAmount = 0;
    if (QUEUES_WriteToProcess(processID, comm) == TRUE)
        pthread_join(PCB_GetByProcessID(processID)->processThread, NULL);
    printf("process closed..\n");
    comm = malloc(sizeof (QueueCommand_t));
    comm->command = PRMDeleteProcessIPT;
    comm->params = calloc(2, sizeof (int));
    comm->params[0] = -1;
    comm->params[1] = processID;
    comm->paramsAmount = 2;
    comm->stringParamsAmount = 0;
    comm->voidParamsAmount = 0;
    QUEUES_WriteToPRM(comm);
    ASSERT_PRINT("Entering: UI_HandleDelProcess(%d)\n", processID);
}

void UI_HandleRead(int vAddr, PID processID, unsigned int amount) {
    ASSERT_PRINT("Entering: UI_HandleRead(%d,%d,%d)\n", vAddr, processID, amount);

    MemoryAddress_t addr;
    addr.pageNumber = vAddr;
    addr.processID = processID;
    QueueCommand_t_p comm = malloc(sizeof (QueueCommand_t));


    comm->command = ProcessReadAddress;
    comm->params = calloc(2, sizeof (int));
    comm->params[0] = addr.pageNumber;
    comm->params[1] = amount;

    if (outFile != stdout) {
        comm->voidParams = calloc(1, sizeof (void*));
        comm->voidParams[0] = outFile;
        comm->voidParamsAmount = 1;
    } else {
        comm->voidParamsAmount = 0;
    }
    comm->stringParamsAmount = 0;
    comm->paramsAmount = 2;

    //printf("sending command...\n");
    QUEUES_WriteToProcess(processID, comm);
    ASSERT_PRINT("Exiting: UI_HandleRead(%d,%d,%d)\n", vAddr, processID, amount);
}

void UI_HandleLoopRead(int vAddr, PID processID, int offset, unsigned int amount) {
    int i = 0;
    for (i; i < amount; i++) {
        UI_HandleRead(vAddr + i * offset, processID, 1);
    }
}

void UI_HandleReadToFile(int vAddr, PID processID, unsigned int amount, string filename) {
    ASSERT_PRINT("Entering: UI_HandleReadToFile(vAddr:%d,processID:%d,amount:%d,fileName:%s)\n", vAddr, processID, amount, filename);
    if ((outFile = fopen(filename, "w+")) != NULL) {
        UI_HandleRead(vAddr, processID, amount);
        outFile = stdout;
    }
    ASSERT_PRINT("Exiting: UI_HandleReadToFile(vAddr:%d,processID:%d,amount:%d,fileName:%s)\n", vAddr, processID, amount, filename);
}

void UI_HandleLoopReadToFile(int vAddr, PID processID, int off, unsigned int amount, string filename) {
    ASSERT_PRINT("Entering: UI_HandleLoopReadToFile(vAddr:%d,processID:%d, off:%d, amount:%d, fileName:%s)\n", vAddr, processID, off, amount, filename);
    if ((outFile = fopen(filename, "w")) != NULL) {
        UI_HandleLoopRead(vAddr, processID, off, amount);
        close(outFile);
        outFile = stdout;
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
    comm->params = calloc(2, sizeof (int));

    comm->params[0] = addr.pageNumber;
    comm->params[1] = strlen(s);

    comm->stringParams = calloc(1, sizeof (string));
    comm->stringParams[0] = calloc(strlen(s), sizeof (char));
    int i = 0;
    for (i = 0; i < strlen(s); i++)
        comm->stringParams[0][i] = s[i];
    comm->paramsAmount = 2;
    comm->stringParamsAmount = 1;
    comm->voidParamsAmount = 0;
    if (QUEUES_WriteToProcess(processID, comm) == TRUE);
    printf("OK");
    ASSERT_PRINT("Exiting: UI_HandleWrite(vAddr:%d, processID:%d, content:%s)\n", vAddr, processID, s);
}

void UI_HandleLoopWrite(int vAddr, PID processID, char c, int off, unsigned int amount) {
    ASSERT_PRINT("Entering: UI_HandleLoopWrite(vAddr:%d, processID:%d,char:%c,off:%d,amount:%d)\n", vAddr, processID, c, off, amount);
    int i = 0;
    string temp = calloc(2, sizeof (char));
    temp[0] = c;
    temp[1] = 0;
    for (i; i < amount; i++)
        UI_HandleWrite(vAddr + i * off, processID, temp);
    free(temp);
    ASSERT_PRINT("Exiting: UI_HandleLoopWrite(vAddr:%d, processID:%d,char:%c,off:%d,amount:%d)\n", vAddr, processID, c, off, amount);
}

void UI_HandlePrintHat() {
    ASSERT_PRINT("Entering: UI_HandlePrintHat()\n");
    HAT_Print();
    ASSERT_PRINT("Exiting: UI_HandlePrintHat()\n");
}

void UI_HandleMonitor() {
    ASSERT_PRINT("Entering: UI_HandleMonitor()\n");
    monitor = TRUE;
    ASSERT_PRINT("Exiting: UI_HandleMonitor()\n");
}
//switches the system to No Monitor mode.

void UI_HandleNoMonitor() {
    ASSERT_PRINT("Entering: UI_HandleNoMonitor()\n");
    monitor = FALSE;
    ASSERT_PRINT("Exiting: UI_HandleNoMonitor()\n");
}

void UI_HandleBatchFile(string filename) {
    ASSERT_PRINT("Entering: UI_HandleBatchFile(file:%s)\n", filename);
    //FILE* f; // create a new file pointer

    if ((inFile = fopen(filename, "r")) == NULL) { // open a file
        fprintf(outFile, "could not open file"); // print an error
        exit(1);
    }
    ASSERT_PRINT("Exiting: UI_HandleBatchFile(file:%s)\n", filename);
}

void UI_HandlePrintMMUTable() {
    ASSERT_PRINT("Entering: UI_HandleBatchFile()\n");
    int i = 0;
    pthread_mutex_lock(&IPT_mutex);
    for (i = 0; i < SIZE_OF_IPT; i++) {
        if (IPT[i])
            fprintf(outFile, "%d) (pid=%d, pageNum=%d, dirty bit=%d, aging reference bit=?)\n", i, IPT[i]->processID, IPT[i]->pageNumber, IPT[i]->dirtyBit);
        else
            fprintf(outFile, "%d) (free)\n", i);

    }
    pthread_mutex_unlock(&IPT_mutex);
    ASSERT_PRINT("Exiting: UI_HandleBatchFile()\n");
}

void UI_HandlePrintMM() {
    ASSERT_PRINT("Entering: UI_HanldePrintMM()\n");
    int i = 0;
    int j = 0;
    //MM[-1]=1;
    Page res; //= calloc(PageSize, sizeof (char));
    for (i = 0; i < NumOfPagesInMM; i++) {

        res = MM_ReadPage(i);
        printf("%d) ", i);
        if (res != NULL) {
            for (j = 0; j < PageSize; j++) {
                fprintf(outFile, "%c|", (res)[j]);
            }
        } else
            printf("NULL");

        fprintf(outFile, "\n");
    }
    ASSERT_PRINT("Exiting: UI_HanldePrintMM()\n");
}

void UI_HandleHitRate() {
    ASSERT_PRINT("Entering: UI_HandleHitRate()\n");
    printf("%f\n", (MM_Access_Counter == 0 || MM_Hit_Counter == 0) ? 0 : ((double) MM_Hit_Counter / (double) MM_Access_Counter));
    ASSERT_PRINT("Exiting: UI_HandleHitRate()\n");
}

void UI_HandlePrintRegisters() {
    ASSERT_PRINT("Entering: UI_HandlePrintRegisters()\n");
    //pthread_mutex_lock(&Aging_mutex); //so that the aging algorithm wont kick in
    int i = 0;
    for (i; i < NumOfPagesInMM; i++)
        printf("%x", Aging_Registers[i]);
    printf("\n");
    //pthread_mutex_unlock(&Aging_mutex);
    ASSERT_PRINT("Exiting: UI_HandlePrintRegisters()\n");
}