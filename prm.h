/* 
 * File:   prm.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 7:17 PM
 *
 * Page Replacement Manager (PRM)
 * the PRM is a dedicated process simulated by a pthread.
 * Each time a requested page is not in MM,
 * the PRM is called to replace pages so that the requested page will be in the MM.
 * It is the job of the PRM to find a free page slot in MM
 * (if such free slot exists) and load the requested page from the Disk to the MM.
 * If no such free page slot exists, the PRM removes a selected page to the Disk,
 * and loads the requested page from the Disk to the now free slot in the MM.
 */

#ifndef _PRM_H
#define	_PRM_H

#include "globaldefs.h"
#include "datatypes.h"
#include "messagequeues.h"
#include "ipt.h"
#include "pcb.h"
#include "disk.h"
#include "hat.h"
#include "mm.h"
#include "disk.h"

static bool PRM_shouldClose = FALSE;
pthread_t PRM;

bool PRM_Create();
void PRM_Close();
void* PRM_Main();
bool PRM_Init();

/*
 * return the oldest frame index in the MM.
 * */
MMFI PRM_FindOldestPage();

/*
 * recieve diskPageIndex: a page from the disk to enter the MM
 * and IPTOldFrameLine: entry in the IPT that folds all infromatoin about
 * the MM frame that is going to be deleted from the MM.
 * if the dirty bit is up, find the place of the page in the disk and update it.
 * else do nothing.
 * finaly replace the MM entry with the diskPage.
 * return False if fail.
 * */
bool PRM_ReplaceMMFrameWithDiskFrame(DPI diskPageIndex, IPT_t_p IPTOldFrameLine);

bool PRM_loadPage(const PID processID, const int pageNum, const int diskStart);

#endif	/* _PRM_H */

