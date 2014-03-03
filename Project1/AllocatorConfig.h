#ifndef ALLOCATOR_CONFIG_H
#define ALLOCATOR_CONFIG_H

// if enabled, allocations are initialized to the default value
#define MEMORY_DEFAULT_INITIALIZE 1

// default value for memory initialization
#if MEMORY_DEFAULT_INITIALIZE
    #include <cstring>          // memset
    #define kMemoryDefaultValue 0xcc
#endif

#endif // ALLOCATOR_CONFIG_H