/* 
 * File:   mm.h
 * Author: yanivdu
 *
 * Created on May 24, 2010, 7:56 PM
 */

#ifndef _MM_H
#define	_MM_H

#include "datatypes.h"
#include "globaldefs.h"

static MM_t MM;

bool MM_Init();

Page MM_ReadPage(int pageNum);
void MM_WritePage(Page data,int pageNum, int bitsToWrite, int dirtyBit);

#endif	/* _MM_H */

