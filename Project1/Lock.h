#ifndef LOCK_H
#define LOCK_H

/*------------------------------------------------------------------------------
this struct is the same as _RTL_CRITICAL_SECTION
------------------------------------------------------------------------------*/
#pragma pack( push, 8 )
struct CriticalSection {
    void                *debugInfo;
    long                 lockCount;
    long                 recursionCount;
    void                *owningThread;
    void                *lockSemaphore;
    _w64 unsigned long   spinCount;
};
#pragma pack( pop )

/*------------------------------------------------------------------------------
use to facilitate concurrency
------------------------------------------------------------------------------*/
class Lock{
public:
    // constructor
    Lock();

    // destructor
    ~Lock();
	
	// acquire an exclusive lock
	void Acquire();
	
	// release the lock
	void Release();

private:
	CriticalSection m_cs;	// can be acquired by one thread at a time
};

#endif // LOCK_H