#include "prm.h"
#include "disk.h"

bool PRM_Init() {
    ASSERT_PRINT("Entering:PRM_Create()\n");
    if (pthread_create(&PRM, NULL, PRM_Main, NULL) != 0)
        return FALSE;
    ASSERT_PRINT("Exiting:PRM_Create()\n");
    return TRUE;
}

void PRM_Close() {
    ASSERT_PRINT("Entering:PRM_Close()\n");
    PRM_shouldClose = TRUE;
    ASSERT_PRINT("Exiting:PRM_Close()\n");
}

void* PRM_Main() {
    ASSERT_PRINT("Entering:PRM_Main()\n");
    while (!PRM_shouldClose) {

        ASSERT_PRINT("PRM trying to read from queue /PRM\n");
        QueueCommand_t_p command = 0;
        if ((command = QUEUES_ReadPRM())!=0) {
            //QUEUES_PrintCommand(command);
            switch (command->command) {
                case PRMSegmentationFault: //params[0]=pageNumber, params[1]=ProcessID
                {
                    if (totalPagesInIPT >= SIZE_OF_IPT) {
                        //replace page from the MM
                        PID process = command->params[1];
                        LPN pageNumber = command->params[0];
                        int diskIndex = PCBArray[process].start + pageNumber;
                        MMFI oldFrame = PRM_FindOldestPage();
                        int line = -1;
                        line=IPT_FindLineByFrame(oldFrame);
                        PRM_ReplaceMMFrameWithDiskFrame(diskIndex, IPT[line]);
                        MemoryAddress_t mem;
                        mem.processID = process;
                        mem.pageNumber = pageNumber;
                        int HATPointedIndex = HAT_PRIVATE_Hash(mem);
                        if (!IPT_Remove(HATPointedIndex, IPT[line]->processID, IPT[line]->pageNumber)) {
                            ASSERT(1 == 2);
                            exit(-1);
                        }
                        if (!IPT_Add(HATPointedIndex, process, pageNumber, oldFrame)) {
                            ASSERT(1 == 2);
                            exit(-1);
                        }

                    } else {
                        PID process = command->params[1];
                        LPN pageNumber = command->params[0];
                        int disk_index = PCBArray[process].start + pageNumber;
                        MMFI frame = -1;
                        if ((frame = IPT_FindEmptyFrame()) == -1)//find empty spot in the MM using the IPT
                        {
                            ASSERT(1 == 2);
                            exit(-1);
                        }
                        MemoryAddress_t mem;
                        mem.pageNumber = pageNumber;
                        mem.processID = process;
                        int HATPointedIndex = HAT_PRIVATE_Hash(mem);
                        IPT_Add(HATPointedIndex, process, pageNumber, frame); //add a line to the IPT

                        Page page = calloc(PageSize, sizeof (char));
                        DISK_ReadPage(disk_index, &page);
                        MM_WritePage(page, frame, PageSize, 0, 0);
                        free(page);
                    }

                }
                    break;
            }
            DONE_WITH_PRM(command->params[1]);
            QUEUES_FreeCommand(command);
        }
    }
    ASSERT_PRINT("Exiting:PRM_Main()\n");
}

MMFI PRM_FindOldestPage() {
    MMFI oldestIndex = 0;
    unsigned int smallest = Aging_Registers[0];
    int i = 0;
    for (i = 0; i < NumOfPagesInMM; i++) {
        if (Aging_Registers[i] < smallest) {
            oldestIndex = i;
            smallest = Aging_Registers[i];
        }
    }
    printf("oldest index: %d\n",oldestIndex);
    return oldestIndex;
}

bool PRM_ReplaceMMFrameWithDiskFrame(DPI diskPageIndex, IPT_t_p IPTOldFrameLine) {
    ASSERT_PRINT("Entering:PRM_ReplaceMMFrameWithDiskFrame()\n");
    if (IPTOldFrameLine->frame > NumOfPagesInMM || diskPageIndex > NumOfPagesInDisk) {
        ASSERT_PRINT("Exiting:PRM_ReplaceMMFrameWithDiskFrame() - frame or disk index to large\n");
        return FALSE;
    }

    int oldFramePage = IPTOldFrameLine->frame;
    if (IPTOldFrameLine->dirtyBit) //reprace the memory in the disk with the memprt in the MM onlt if it changed
    {
        PID process = IPTOldFrameLine->processID;
        LPN pageNumber = IPTOldFrameLine->pageNumber;
        DISK_WritePage(MM_ReadPage(oldFramePage), process * NumOfProcessPages + pageNumber);
    }

    Page page = calloc(PageSize, sizeof (char));
    DISK_ReadPage(diskPageIndex, &page);
    MM_WritePage(page, oldFramePage, PageSize, 0, 0);
    ASSERT_PRINT("Exiting:IPT_Replace() with return value: TRUE\n");
    return TRUE;
}