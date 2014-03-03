#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "SingletonBase.h"      // base class
#include "StackAllocator.h"     // stack allocation
#include <cstdlib>              // malloc
#include "AllocatorConfig.h"    // default value for memory
#include "Lock.h"               // critical sections

/*------------------------------------------------------------------------------
manages multiple types of memory allocators
------------------------------------------------------------------------------*/
class MemoryManager : public SingletonBase< MemoryManager > {
public:
    friend class StackScopedRestore;
    friend class StackManualRestore;

    // create allocators
    void Initialize( uint32_t stackSize, uint32_t permanentSize );

    // free memory
    void Deinitialize();

    // allocate a block of memory from a stack
    // the stack will be reset at the beginning of each frame
    inline void* StackAllocate( uint32_t size ) { return m_stack.Allocate( size ); }

    // allocate a block of memory from a heap
    // every call to HeapAllocate must be followed a call to HeapFree
    inline void* HeapAllocate( uint32_t size ) { 
        m_heapLock.Acquire();
        void *allocation = malloc( size );
        #if MEMORY_DEFAULT_INITIALIZE
            memset( allocation, kMemoryDefaultValue, size );
        #endif
        m_heapLock.Release();
        return allocation;
    }

    inline void  HeapFree( void *ptr ) { free( ptr ); }

    // allocate a block of memory that will persist until the program exits
    inline void* PermanentAllocate( uint32_t size ) { 
        m_permanentLock.Acquire();
        void *allocation = m_permanentStack.Allocate( size );
        m_permanentLock.Release();
        return allocation;
    }

private:
    StackAllocator  m_stack;               // each worker thread has it's own stack?
    StackAllocator  m_permanentStack;      // used for things that are allocated once at startup

    Lock            m_heapLock;            //
    Lock            m_permanentLock;       //
};

// global instance
extern MemoryManager g_memoryManager;

#endif // MEMORY_MANAGER_H