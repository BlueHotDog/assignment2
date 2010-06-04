#include "ui.h"
#include "mmu.h"

void* UI_Main() {

    string command = calloc(MAX_INPUT_LENGTH, sizeof (char));
    int t = 0;
    while (!UI_ShouldUIThreadClose) {
        fprintf(outFile,">");
        if(fscanf(inFile,"%s", command)!=EOF)
            UI_ParseCommand(&command);
        else
        {
            fclose(inFile);
            inFile = stdin;

        }
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
        fscanf(inFile,"%d %d %d", &vAddr, &id, &amount);
        UI_HandleRead(vAddr, id, amount);
    } else if (strcmp(*comm, "loopRead") == 0) {
        //loopRead vAddr id off amount
        int vAddr = -1;
        int id = -1;
        int amount = -1;
        int offset = -1;
        int i = 0;
        fscanf(inFile,"%d %d %d %d", &vAddr, &id, &offset, &amount);
        UI_HandleLoopRead(vAddr, id, offset + i, 1);
    } else if (strcmp(*comm, "write") == 0) {
        //write vAddr id s
        int vAddr = -1;
        int id = -1;
        string val = 0;
        fscanf(inFile,"%d %d %s", &vAddr, &id, val);
        UI_HandleWrite(vAddr, id, val);
    } else if (strcmp(*comm, "exit") == 0) {
        UI_SignalUIThreadToStop();
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
        fscanf(inFile,"%s", file);
        UI_HandleBatchFile(file);
        free(file);
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
        fprintf(outFile,"%d\n", id);
    else
        fprintf(outFile,"Error creating process\n");

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
    comm->paramsAmount = 2;
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

    if ((outFile = fopen(filename, "w")) != NULL) {
        UI_HandleRead(vAddr, processID, amount);
        fclose(outFile);
        outFile = stdout;
    }
    ASSERT_PRINT("Exiting: UI_HandleReadToFile(vAddr:%d,processID:%d,amount:%d,fileName:%s)\n", vAddr, processID, amount, filename);
}

void UI_HandleLoopReadToFile(int vAddr, PID processID, int off, unsigned int amount, string filename) {
    ASSERT_PRINT("Entering: UI_HandleLoopReadToFile(vAddr:%d,processID:%d, off:%d, amount:%d, fileName:%s)\n", vAddr, processID, off, amount, filename);
    if ((outFile = fopen(filename, "w")) != NULL) {
        UI_HandleLoopRead(vAddr, processID, off, amount);
        fclose(outFile);
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
    comm->params = calloc(1, sizeof (int));
    comm->stringParams = calloc(1, sizeof (string));
    comm->params[0] = addr.pageNumber;
    comm->stringParams[0] = s;
    comm->paramsAmount = 1;
    comm->stringParamsAmount = 1;

    QUEUES_WriteToProcess(processID, comm);

    ASSERT_PRINT("Exiting: UI_HandleWrite(vAddr:%d, processID:%d, content:%s)\n", vAddr, processID, s);
}

void UI_HandleLoopWrite(int vAddr, PID processID, char c, int off, unsigned int amount) {
    ASSERT_PRINT("Entering: UI_HandleLoopWrite(vAddr:%d, processID:%d,char:%c,off:%d,amount:%d)\n", vAddr, processID, c, off, amount);
    int i = 0;
    for (i; i < amount; i++)
        UI_HandleWrite(vAddr + i * off, processID, &c);
    ASSERT_PRINT("Exiting: UI_HandleLoopWrite(vAddr:%d, processID:%d,char:%c,off:%d,amount:%d)\n", vAddr, processID, c, off, amount);
}

void UI_HandlePrintHat() {
    ASSERT_PRINT("Entering: UI_HandlePrintHat()\n");
    HAT_Print();
    ASSERT_PRINT("Exiting: UI_HandlePrintHat()\n");
}

void UI_HandleMonitor() {
    ASSERT_PRINT("Entering: UI_HandleMonitor()\n");
    //monitor = TRUE;
    ASSERT_PRINT("Exiting: UI_HandleMonitor()\n");
}
//switches the system to No Monitor mode.

void UI_HandleNoMonitor() {
    ASSERT_PRINT("Entering: UI_HandleNoMonitor()\n");
    //monitor = FALSE;
    ASSERT_PRINT("Exiting: UI_HandleNoMonitor()\n");
}

void UI_HandleBatchFile(string filename) {
    ASSERT_PRINT("Entering: UI_HandleBatchFile(file:%s)\n", filename);
    //FILE* f; // create a new file pointer
    
    if ((inFile = fopen(filename,"r")) == NULL) { // open a file
        fprintf(outFile,"could not open file"); // print an error
        exit(1);
    }
    ASSERT_PRINT("Exiting: UI_HandleBatchFile(file:%s)\n", filename);
}

void UI_HandlePrintMMUTable() {
    ASSERT_PRINT("Entering: UI_HandleBatchFile()\n");
    int i=0;
    for(i=0; i<SIZE_OF_IPT; i++)
    {
        if(IPT[i])
            fprintf(outFile,"%d) (pid=%d, pageNum=%d, dirty bit=%d, aging reference bit=?, next=%d, prev=%d)\n",
                    i,IPT[i]->processID,IPT[i]->pageNumber, IPT[i]->dirtyBit,IPT[i]->next, IPT[i]->prev);
        else
            fprintf(outFile,"%d) (free)\n",i);

    }
    ASSERT_PRINT("Exiting: UI_HandleBatchFile()\n");
}