/*
 * File:   hat.h
 * Authors: danni & yaniv
 *
 * Created on May 9, 2010, 7:01 PM
 *
 * The MMU uses the HAT in order to make the conversion.
 * The HAT is a table which has NumOfPagesInMM entries. Each entry is either
 * NULL or points to an entry in the IPT.
 * Given a PID and a logical page number N, the MMU applies hash
 * function Hash on PID and N in order to get an entry in the HAT.
 * The hash function computes:
 * Hash(PID,N) = (PID*N)%NumOfPagesInMM
 * Then, the entry’s pointer to the IPT entry e is followed,
 * and the process ID and logical page number are compared with
 * those stored in e. If they don’t match,
 * e’s Next pointer is followed to get to another IPT entry,
 * and the process is repeated until a match is
 * found or e’s Next pointer is NULL.
 */

#ifndef _HAT_H
#define	_HAT_H
#include "globaldefs.h"
#include "datatypes.h"
#include "ipt.h"

HAT_t HAT;

bool HAT_Init();
int HAT_PRIVATE_Hash(MemoryAddress_t memoryAddress);

IPT_t_p HAT_GetEntry(MemoryAddress_t memoryAddress);

void HAT_Print();
#endif	/* _HAT_H */

