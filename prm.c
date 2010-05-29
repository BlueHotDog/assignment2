#include "prm.h"
#include "ipt.h"
#include "pcb.h"
#include "disk.h"
#include "hat.h"

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
        QueueCommand_t_p command = QUEUES_ReadPRM();
        QUEUES_PrintCommand(command);
        switch(command->command)
        {
            case PRMSegmentationFault: //params[0]=pageNumber, params[1]=ProcessID
            {
                if (totalPagesInIPT = SIZE_OF_IPT)
                {
                    //replace page from the MM

                    int disk_index = PCBArray[command->params[1]].start + command->params[0];
                    Page moved_page = Disk[disk_index];
                    //add the page to the MM and IPT
                    //find empty spot in the MM using the IPT
                    //add a line to the IPT

                }
                else
                {
                    //get the relevent page from the Disk
                    //and add it to the MM and IPT
                    int disk_index = PCBArray[command->params[1]].start+command->params[0];
                    Page moved_page = Disk[disk_index];
                    MMFI frame = -1;
                    if(!IPT_FindEmptyFrame(&frame))//find empty spot in the MM using the IPT
                    {
                        ASSERT(1==2);
                    }
                    MemoryAddress_t mem;
                    mem.pageNumber = command->params[0];
                    mem.processID = command->params[1];
                    int HATPointedIndex = HAT_PRIVATE_Hash(mem);
                    IPT_Add(HATPointedIndex, command->params[1], command->params[1], frame);//add a line to the IPT
                }
                sem_post(&PROCESSES_mutex[1][command->params[1]]);
            }
            break;
        }
        free(command->params);
        free(command);
    }
    ASSERT_PRINT("Exiting:PRM_Main()\n");
}
