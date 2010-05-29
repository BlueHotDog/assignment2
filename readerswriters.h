/* 
 * File:   readerswriters.h
 * Author: danni
 *
 * Created on May 29, 2010, 6:42 PM
 */

#ifndef _READERSWRITERS_H
#define	_READERSWRITERS_H
#include "globaldefs.h"
#include "datatypes.h"
#include <semaphore.h>

static sem_t    m_semReaders;
static sem_t    m_semWriters;
static int      m_nReaders;

void READERSWRITERS_Init();
void READERSWRITERS_Destroy();
void READERSWRITERS_LockDataRead();
void READERSWRITERS_UnlockDataRead();
void READERSWRITERS_LockDataWrite();
void READERSWRITERS_UnlockDataWrite();

#endif	/* _READERSWRITERS_H */

