#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

/*------------------------------------------------------------------------------
allocates memory from a stack
useful for allocations with a short lifespan
------------------------------------------------------------------------------*/
class StackAllocator{
public:
    // allocate a block of memory
    void Initialize( uint32_t size );

    // free the allocated memory
    void Deinitialize();

    // allocate a block of memory from the stack
    void* Allocate( uint32_t size, uint32_t alignment = 16 );

    // set the stack pointer to the start of the memory block
    void Reset();

    // accessors used with stack restore objects
    inline void* GetPointer() const { return m_ptr; }
    void SetPointer( void *ptr );

private:
    void*            m_memory;      // block of allocated memory
    void*            m_ptr;         // pointer to the top of the stack
    uint32_t         m_free;        // amount of memory available
};

#endif // STACK_ALLOCATOR_H