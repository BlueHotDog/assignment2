/* 
 * File:   prm.h
 * Author: danni & yaniv
 *
 * Created on May 9, 2010, 7:17 PM
 */

#ifndef _PRM_H
#define	_PRM_H
#include "globaldefs.h"
#include "datatypes.h"
static bool PRM_shouldClose = FALSE;

pthread_t PRM;

typedef struct PRM_RequestQueueValueStruct
{
    unsigned int pageNumber;
    unsigned int ProccessID;

} PRM_RequestQueueValue_t,*PRM_RequestQueueValue_tp;

bool PRM_Create();
void PRM_Close();

bool loadPage(const PID processID, const int pageNum, const int diskStart);


#endif	/* _PRM_H */

