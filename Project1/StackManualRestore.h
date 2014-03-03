#ifndef STACK_MANUAL_RESTORE
#define STACK_MANUAL_RESTORE

#include "MemoryManager.h"

/*------------------------------------------------------------------------------
provides access to the memory manager's stack so you can save it's current
position, allocate some memory, then reset the stack to its original position when
you're finished with the memory
------------------------------------------------------------------------------*/
class StackManualRestore {
public:
    // constructor
    StackManualRestore() : m_restorePoint( NULL ) {}
    
    // save the stack pointer
    void SaveLocation() { m_restorePoint = g_memoryManager.m_stack.GetPointer(); }

    // reset the stack pointer to the saved position
    void ResetStack() {
        Assert( m_restorePoint != NULL, "call SaveLocation() before resetting the stack");
        g_memoryManager.m_stack.SetPointer( m_restorePoint );
    }

private:
    void *m_restorePoint;   // position to reset the stack to
};

#endif // STACK_MANUAL_RESTORE