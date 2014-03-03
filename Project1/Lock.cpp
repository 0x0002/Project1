#include "Precompiled.h"
#include "Lock.h"
#include <windows.h>		// windows.h header     

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Lock::Lock() {
	InitializeCriticalSection( (_RTL_CRITICAL_SECTION*)&m_cs );
}

/*------------------------------------------------------------------------------
destructor
------------------------------------------------------------------------------*/
Lock::~Lock() {
	DeleteCriticalSection( (_RTL_CRITICAL_SECTION*)&m_cs );
}
	
/*------------------------------------------------------------------------------
acquire an exclusive lock
------------------------------------------------------------------------------*/
void Lock::Acquire() {
	EnterCriticalSection( (_RTL_CRITICAL_SECTION*)&m_cs );
}

/*------------------------------------------------------------------------------
release the lock
------------------------------------------------------------------------------*/
void Lock::Release() {
	LeaveCriticalSection( (_RTL_CRITICAL_SECTION*)&m_cs );
}