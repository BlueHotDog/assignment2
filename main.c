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
    fprintf(outFile, "Incorrect usage, please user:\n sim config_file_name");

}

void init() {
    bool ReturnVal = FALSE;

    ASSERT_PRINT("===Starting init===\n");

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

    ASSERT_PRINT("Creating UI Thread...\n");
    ReturnVal = UI_CreateUIThread();
    ASSERT(ReturnVal != FALSE);
}

int main(int argc, char** argv) {
    void* status;
    inFile = stdin;
    outFile = stdout;
#ifdef DEBUG
    readConfigFromFile("config");
    printConfigInfo();
#else
    if (argc != 2) {
        printUsage();
        exit(-1);
    }
    readConfigFromFile(argv[1]);
#endif
    init();
    pthread_join(UI_Thread, status);

    return (EXIT_SUCCESS);
}
