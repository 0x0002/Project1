#ifndef MEMORY_H
#define MEMORY_H

#include "MemoryManager.h"      // global singleton
#include "StackScopedRestore.h" // used to pop data off the memory manager's stack
#include "StackManualRestore.h" // used to pop data off the memory manager's stack

// allocate a block of memory from a stack
// the stack will be reset at the beginning of each frame
inline void* StackAllocate( uint32_t size ) { return g_memoryManager.StackAllocate( size ); }

// allocate a block of memory from a heap
// every call to HeapAllocate must be followed a call to HeapFree
inline void* HeapAllocate( uint32_t size ) { return g_memoryManager.HeapAllocate( size ); }

// free a block of memory that was allocated using HeapAllocate
inline void  HeapFree( void *ptr ) { g_memoryManager.HeapFree( ptr ); }

// allocate a block of memory that will persist until the program exits
inline void* PermanentAllocate( uint32_t size ) { return g_memoryManager.PermanentAllocate( size ); }

#endif // MEMORY_H