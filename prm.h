/* 
 * File:   prm.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 7:17 PM
 */

#ifndef _PRM_H
#define	_PRM_H
#include "globaldefs.h"

static bool shouldAgingDeamonClose = FALSE;

pthread_t AgingDaemon;
bool CreateAgingDeamon();
void CloseAgingDeamon();
/*
 * where diskStart is the address of the first page of this process on Disk.
 * PRM uses diskStart to find the correct page on the Disk to load,
 * selects where to load it to,
 * and if the selected location already contains a page,
 * then it first pages-out this page and only then copies the page from
 * the Disk to the selected location in MM.
 */
//bool loadPage(PID processID, int pageNum,int diskStart);
void agingAlgorithm();


#endif	/* _PRM_H */

