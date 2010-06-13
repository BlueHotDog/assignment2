/* 
 * File:   main.c
 * Author: danni
 *
 * Created on May 9, 2010, 5:50 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "globaldefs.h"
#include "datatypes.h"
#include "ipt.h"
#include "hat.h"
#include "ui.h"
#include "disk.h"
#include "pcb.h"
#include "freelist.h"
#include "mmu.h"
#include "mm.h"
#include "readerswriters.h"
#include "aging.h"
#include "prm.h"

void readConfigFromFile(string fileName) {
    const int lineLength = 100;
    FILE* hFile;
    hFile = fopen(fileName, "r");
    int i = 0, j = 0;

    char* temp = 0;
    if (hFile == NULL) {
        exit(-1);
    } else {
        char oneLine[lineLength];

        fgets(oneLine, lineLength, hFile);
        sscanf(oneLine, "MaxNumOfProcesses = %u", &MaxNumOfProcesses);

        fgets(oneLine, lineLength, hFile);
        sscanf(oneLine, "PageSize = %u", &PageSize);

        fgets(oneLine, lineLength, hFile);
        sscanf(oneLine, "NumOfPagesInMM = %u", &NumOfPagesInMM);

        fgets(oneLine, lineLength, hFile);
        sscanf(oneLine, "NumOfPagesInDisk = %u", &NumOfPagesInDisk);

        fgets(oneLine, lineLength, hFile);
        sscanf(oneLine, "NumOfProcessPages = %u", &NumOfProcessPages);

        fgets(oneLine, lineLength, hFile);
        sscanf(oneLine, "ShiftClock = %u", &ShiftClock);

        fclose(hFile);
    }
}

void printConfigInfo() {
    fprintf(outFile, "MaxNumOfProcesses = %u\n", MaxNumOfProcesses);
    fprintf(outFile, "PageSize = %u\n", PageSize);
    fprintf(outFile, "NumOfPagesInMM = %u\n", NumOfPagesInMM);
    fprintf(outFile, "NumOfPagesInDisk = %u\n", NumOfPagesInDisk);
    fprintf(outFile, "NumOfProcessPages = %u\n", NumOfProcessPages);
    fprintf(outFile, "ShiftClock = %u\n", ShiftClock);
}

void printUsage() {
    fprintf(outFile, "Incorrect usage, please user:\n sim config_file_name\n");

}

void init() {
    bool ReturnVal = FALSE;

    ASSERT_PRINT("===Starting init===\n");

    ASSERT_PRINT("Init Monitor = FALSE\n");
    monitor = FALSE;

    ASSERT_PRINT("Init FreeList...\n");
    ReturnVal = FREELIST_Init();
    ASSERT(ReturnVal != FALSE);


    ASSERT_PRINT("Init Disk...\n");
    ReturnVal = DISK_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Creating PCB array for %d processes..\n", MaxNumOfProcesses);
    ReturnVal = PCB_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Init MM...\n");
    ReturnVal = MM_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Init MailBox Queue...\n");
    ReturnVal = QUEUES_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Init IPT..\n");
    ReturnVal = IPT_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Init HAT..\n");
    ReturnVal = HAT_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Init PRM..\n");
    ReturnVal = PRM_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Init READERSWRITERS..\n");
    ReturnVal = READERSWRITERS_Init();
    ASSERT(ReturnVal != FALSE);

    ASSERT_PRINT("Init Aging Deamon...\n");
    ReturnVal = AGING_Init();
    ASSERT(ReturnVal != FALSE);


    ASSERT_PRINT("Creating UI Thread...\n");
    ReturnVal = UI_CreateUIThread();
    ASSERT(ReturnVal != FALSE);
}

int main(int argc, char** argv) {
    void* status = NULL;
    inFile = stdin;
    outFile = stdout;
#ifdef DEBUG
    readConfigFromFile("config");
    printConfigInfo();
    UI_HandleBatchFile("batch");
#else
    if (argc != 2) {
        printUsage();
        exit(-1);
    }
    readConfigFromFile(argv[1]);
#endif
    init();
    pthread_join(UI_Thread, status);

    int i = 0;

    for (i; i < MaxNumOfProcesses; i++) {
        if(PCBArray[i].active)
            pthread_join(PCBArray[i].processThread,NULL);
    }

    //closing AGING deamon
    AGING_Close();
    pthread_mutex_unlock(&Aging_mutex);
    pthread_join(Aging, NULL);

    FREELIST_DeAllocate();

    //closing PRM
    PRM_Close();
    sem_post(&PRM_full); // decrement the full semaphore
    sem_post(&PRM_mutex); // enter critical section
    pthread_join(PRM, NULL);

    QUEUES_DeInit();
    MM_DeInit();


    DISK_DeInit();
    PCB_Free();
    for (i = 0; i < NumOfPagesInMM; i++) {
        //if(IPT[i]!=NULL)
            free(IPT[i]);
    }
    free(IPT);
    free(HAT);

    fclose(inFile);
    fclose(outFile);
    return (EXIT_SUCCESS);
}
