/* 
 * File:   ipt.h
 * Author: yanivdu
 *
 * Created on May 29, 2010, 12:56 PM
 */

#ifndef _IPT_
#define	_IPT_

#include "globaldefs.h"
#include "datatypes.h"

#define INDEX_INC(i) ((i+1)%NumOfPagesInMM)
#define SIZE_OF_IPT NumOfPagesInMM

static IPT_t_p* IPT;
static int totalPagesInIPT;

bool IPT_Init();

/*
 * simply return all the given data as a new allocated IPT_t_p dataytpe.
 * return false if allocation failed.
 * the returned value is at newIPTLine, not allocated by the calling method.
 * */
bool IPT_CreateIPT_t_p(
            PID processID,
            LPN pageNumber,
            MMFI frame, 
            OUT IPT_t_p newIPTLine);

/*
 * update prev and next of newIPTLine and add to IPT table.
 * return true if there is a free sapce in the IPT table
 * otherwise return false, in that case, the calling thread should
 * throw a segmentation fault to the PRM.
 * */
bool IPT_Add(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber,
        MMFI frame)



#endif	/* _IPT_ */

