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

static bool PRM_shouldClose = FALSE;

pthread_t PRM;



bool PRM_Create();
void PRM_Close();

bool PRM_loadPage(const PID processID, const int pageNum, const int diskStart);


#endif	/* _PRM_H */

