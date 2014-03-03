#ifndef INPUT_BINDING_NODE_H
#define INPUT_BINDING_NODE_H

#include "InputBinding.h"

/*------------------------------------------------------------------------------
linked list node
------------------------------------------------------------------------------*/
struct InputBindingNode {
    uint16_t     m_next;        // 0xffff means there isn't a next node
    uint16_t     m_prev;        // 0xffff means there isn't a previous node
    InputBinding m_binding;     // binds an input combination to a callback function
};

#endif // INPUT_BINDING_NODE_H