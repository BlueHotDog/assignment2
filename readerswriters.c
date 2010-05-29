#include "readerswriters.h"

void READERSWRITERS_Init() {
    ASSERT_PRINT("Entering:READERSWRITERS_Init()\n");
    /*
        m_semReaders = 0;
        m_semWriters = 0;
     */
    m_nReaders = 0;
    // initialize the Readers & Writers variables


    ASSERT(sem_init(&m_semReaders, 0, 1) != -1);
    ASSERT(sem_init(&m_semWriters, 0, 1) != -1);
    m_nReaders = 0;
    ASSERT_PRINT("Exiting:READERSWRITERS_Init()\n");
}

void READERSWRITERS_Destroy() {
    ASSERT_PRINT("Entering:READERSWRITERS_Destroy()\n");
    ASSERT(sem_destroy(&m_semWriters) == 0);
    ASSERT(sem_destroy(&m_semReaders) == 0);
    ASSERT_PRINT("Exiting:READERSWRITERS_Destroy()\n");
}

void READERSWRITERS_LockDataRead() {
    ASSERT_PRINT("Entering:READERSWRITERS_LockDataRead()\n");
    // P( semReaders )
    ASSERT(sem_wait(&m_semReaders) == 0);

    m_nReaders++;

    if (m_nReaders == 1) {
        // P( semWriters )

        ASSERT(sem_wait(&m_semWriters) == 0);
    }
    // V( semReaders )

    ASSERT(sem_post(&m_semReaders) == 0);
    ASSERT_PRINT("Exiting:READERSWRITERS_LockDataRead()\n");
}

void READERSWRITERS_UnlockDataRead() {
    ASSERT_PRINT("Entering:READERSWRITERS_UnlockDataRead()\n");
    // P( semReaders )
    ASSERT(sem_wait(&m_semReaders) == 0);
    m_nReaders--;

    if (m_nReaders == 0) {

        // V( semWriters )
        ASSERT(sem_post(&m_semWriters) == 0);
    }

    // V( semReaders )
    ASSERT(sem_post(&m_semReaders) == 0);
    ASSERT_PRINT("Exiting:READERSWRITERS_UnlockDataRead()\n");
}

void READERSWRITERS_LockDataWrite() {
    ASSERT_PRINT("Entering:READERSWRITERS_LockDataWrite()\n");
    // P( semWriters )
    ASSERT(sem_wait(&m_semWriters) == 0);
    ASSERT_PRINT("Exiting:READERSWRITERS_LockDataWrite()\n");
}

void READERSWRITERS_UnlockDataWrite() {
    ASSERT_PRINT("Entering:READERSWRITERS_UnlockDataWrite()\n");
    // V( semWriters )
    ASSERT(sem_post(&m_semWriters) == 0);
    ASSERT_PRINT("Exiting:READERSWRITERS_UnlockDataWrite()\n");
}
