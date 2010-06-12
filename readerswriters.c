#include "readerswriters.h"

bool READERSWRITERS_Init() {
    ASSERT_PRINT("Entering:READERSWRITERS_Init()\n");
    /*
        m_semReaders = 0;
        m_semWriters = 0;
     */
    m_nReaders = 0;
    // initialize the Readers & Writers variables


    if (sem_init(&m_semReaders, 0, 1) == -1)
        return FALSE;
    if (sem_init(&m_semWriters, 0, 1) == -1)
        return FALSE;
    m_nReaders = 0;
    ASSERT_PRINT("Exiting:READERSWRITERS_Init()\n");
}

void READERSWRITERS_Destroy() {
    ASSERT_PRINT("Entering:READERSWRITERS_Destroy()\n");
    sem_destroy(&m_semWriters);
    sem_destroy(&m_semReaders);
    ASSERT_PRINT("Exiting:READERSWRITERS_Destroy()\n");
}

void READERSWRITERS_LockDataRead() {
    ASSERT_PRINT("Entering:READERSWRITERS_LockDataRead()\n");
    // P( semReaders )
    sem_wait(&m_semReaders);

    m_nReaders++;

    if (m_nReaders == 1) {
        // P( semWriters )

        sem_wait(&m_semWriters);
    }
    // V( semReaders )

    sem_post(&m_semReaders);
    ASSERT_PRINT("Exiting:READERSWRITERS_LockDataRead()\n");
}

void READERSWRITERS_UnlockDataRead() {
    ASSERT_PRINT("Entering:READERSWRITERS_UnlockDataRead()\n");
    sem_wait(&m_semReaders);
    m_nReaders--;

    if (m_nReaders == 0) {

        sem_post(&m_semWriters);
    }

    sem_post(&m_semReaders);
    ASSERT_PRINT("Exiting:READERSWRITERS_UnlockDataRead()\n");
}

void READERSWRITERS_LockDataWrite() {

    ASSERT_PRINT("Entering:READERSWRITERS_LockDataWrite()\n");
    sem_wait(&m_semWriters);
    ASSERT_PRINT("Exiting:READERSWRITERS_LockDataWrite()\n");
}

void READERSWRITERS_UnlockDataWrite() {
    ASSERT_PRINT("Entering:READERSWRITERS_UnlockDataWrite()\n");
    // V( semWriters )
    sem_post(&m_semWriters);
    ASSERT_PRINT("Exiting:READERSWRITERS_UnlockDataWrite()\n");
}
