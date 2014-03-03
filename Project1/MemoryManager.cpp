#include "Precompiled.h"
#include "MemoryManager.h"

// global instance
MemoryManager g_memoryManager;

/*------------------------------------------------------------------------------
create allocators
------------------------------------------------------------------------------*/
void MemoryManager::Initialize( uint32_t stackSize, uint32_t permanentSize ) {
    m_stack.Initialize( stackSize );
    m_permanentStack.Initialize( permanentSize );
}

/*------------------------------------------------------------------------------
free memory
------------------------------------------------------------------------------*/
void MemoryManager::Deinitialize() {
    m_stack.Deinitialize();
    m_permanentStack.Deinitialize();
}