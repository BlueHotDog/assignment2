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

#define INDEX_INC(i) (i=((i+1)%NumOfPagesInMM))
#define SIZE_OF_IPT NumOfPagesInMM

IPT_t_p* IPT;
int totalPagesInIPT;

bool IPT_Init();

/*
 * simply return all the given data as a new allocated IPT_t_p dataytpe.
 * return false if allocation failed.
 * the returned value is at newIPTLine, not allocated by the calling method.
 * */
IPT_t_p IPT_CreateIPT_t_p(
            PID processID,
            LPN pageNumber,
            MMFI frame);

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
        MMFI frame);

/*
 * search the table for processID, pageNumber and return the frame index.
 * if the frame is not in the table, i.e. not in the MM, return FALSE.
 * otherwize, return TRUE.
 * */
bool IPT_FindFrame(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber,
        OUT MMFI *frame);

/*
 * search the IPT for processID, pageNumber and remove the entery.
 * if can't find the entry, return false.
 * */
bool IPT_Remove(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber);

/*
 * search the ipt for empy frame by going over all frames in the ipt
 * and searching for an empty spot.
 * return false if there is no empty spot.
 * */
int IPT_FindEmptyFrame();

/*
 * search the IPT for entry with frame value equal to given frame.
 * return true if found such entry
 * return false oherwize.
 * */
int IPT_FindLineByFrame(
        MMFI frame);

/*
 * search the IPT for line folding outProcessID and outPageNumber,
 * free it's memory allocation, create new IPT_t_p line for the inProcessID
 * and inPageNumber and frame.
 * enter the line to the new place by using the HAT Hash function.
 * return false if one of the memory allocation failed. 
 * */
bool IPT_Replace(
        PID outProcessID,
        LPN outPageNumber,
        PID inProcessID,
        LPN inPageNumber,
        MMFI frame);

/*
 * find line by frame index and update it's dirty bit.
 * */
void IPT_UpdateDirtyBit(MMFI frame, bool dirtyBit);

IPT_t_p IPT_FindEmptyLine();

IPT_t_p* IPT_FindIPTLine(
        int HATPointedIndex,
        PID processID,
        LPN pageNumber);
/*
 * find line by frame index and update it's reference bit.
 * */
void IPT_UpdateReferencetyBit(MMFI frame, bool referenceBit);

#endif	/* _IPT_ */

