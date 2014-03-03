#include "Precompiled.h"
#include "StackAllocator.h"
#include "AllocatorConfig.h"    // default value for allocations
#include <cstdlib>              // malloc

/*------------------------------------------------------------------------------
allocate a block of memory
------------------------------------------------------------------------------*/
void StackAllocator::Initialize( uint32_t size ) {
    m_memory = malloc( size );
    m_ptr = m_memory;
    m_free = size;
}

/*------------------------------------------------------------------------------
free the allocated memory
------------------------------------------------------------------------------*/
void StackAllocator::Deinitialize() {
    free( m_memory );
}

/*------------------------------------------------------------------------------
allocate a block of memory from the stack
------------------------------------------------------------------------------*/
void* StackAllocator::Allocate( uint32_t size, uint32_t alignment ) {
    uint32_t alignOffset = ( alignment - (uintptr_t)m_ptr % alignment ) % alignment;

    Assert( m_free >= alignOffset + size, "stack ran out of memory" );

    void *allocation = Offset( m_ptr, alignOffset );
    m_ptr = Offset( m_ptr, alignOffset + size );
    
#if MEMORY_DEFAULT_INITIALIZE
    memset( allocation, kMemoryDefaultValue, size );
#endif
    m_free -= alignOffset + size;
    return allocation;    
}

/*------------------------------------------------------------------------------
set the stack pointer to the start of the memory block
------------------------------------------------------------------------------*/
void StackAllocator::Reset() {
    m_ptr = m_memory;
}

/*------------------------------------------------------------------------------
modify the stack pointer
------------------------------------------------------------------------------*/
void StackAllocator::SetPointer( void *ptr ) {
    m_free += (uint32_t)OffsetOf( m_ptr, ptr );
    m_ptr = ptr;
}
