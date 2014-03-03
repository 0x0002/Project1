#ifndef STACK_SCOPED_RESTORE
#define STACK_SCOPED_RESTORE

#include "MemoryManager.h"

/*------------------------------------------------------------------------------
allocate an object of this type on the stack.
it saves the memory manager's stack pointer in the constructor and resets the
stack to that location when this object goes out of scope
------------------------------------------------------------------------------*/
class StackScopedRestore {
public:
    // constructor
    StackScopedRestore() : m_restorePoint( g_memoryManager.m_stack.GetPointer() ) {}

    // destructor
    ~StackScopedRestore() { g_memoryManager.m_stack.SetPointer( m_restorePoint ); }

private:
    void *m_restorePoint;   // position to reset the stack to
};

#endif // STACK_SCOPED_RESTORE