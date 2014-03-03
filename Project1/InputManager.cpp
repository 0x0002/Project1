#include "Precompiled.h"
#include "InputManager.h"
#include "GameWindow.h"     // initial mouse position
//#include "UIManager.h"      // options control

// global singleton
InputManager g_inputManager;

enum { kMaxInputBindings = 100 };

/*------------------------------------------------------------------------------
initialize the singleton
------------------------------------------------------------------------------*/
void InputManager::Initialize() {
    for( uint32_t i = 0; i < Key::kCount; ++i ) {
        m_currentKeyState [ i ] = InputState::kUp;
        m_previousKeyState[ i ] = InputState::kUp;
    }

    for( uint32_t i = 0; i < Button::kCount; ++i ) {
        m_currentMouseState [ i ] = InputState::kUp;
        m_previousMouseState[ i ] = InputState::kUp;
        m_currentGameMouseState [ i ] = InputState::kUp;
        m_previousGameMouseState[ i ] = InputState::kUp;
    }

    m_mousePosition   = g_gameWindow.GetMousePosition();
    m_mouseDelta      = Point( 0, 0 );
    m_mouseWheelDelta = 0;

    m_bindings = (InputBindingNode*)PermanentAllocate( sizeof( InputBindingNode) * kMaxInputBindings );

    ResetBindings();
}

/*------------------------------------------------------------------------------
deinitialize the singleton
------------------------------------------------------------------------------*/
void InputManager::Deinitialize() {
    // do nothing
}

/*------------------------------------------------------------------------------
prepare for the next frame
------------------------------------------------------------------------------*/
void InputManager::FrameEnd() {
    m_mouseDelta      = Point( 0, 0 );
    m_mouseWheelDelta = 0;

    memcpy( m_previousKeyState,   m_currentKeyState,   sizeof( InputState::Type ) * Key::kCount    );
    memcpy( m_previousMouseState, m_currentMouseState, sizeof( InputState::Type ) * Button::kCount );
    memcpy( m_previousGameMouseState, m_currentGameMouseState, sizeof( InputState::Type ) * Button::kCount );
}

/*------------------------------------------------------------------------------
register a key handler
------------------------------------------------------------------------------*/
void InputManager::BindInput( Key::Type key, InputState::Type state, InputCallbackFn callback ) {
    InputBindingNode *node = AllocateBinding();
    node->m_binding.Init( key, state, callback );
}

/*------------------------------------------------------------------------------
register a button handler
------------------------------------------------------------------------------*/
void InputManager::BindInput( Button::Type button, InputState::Type state, InputCallbackFn callback ) {
    InputBindingNode *node = AllocateBinding();
    node->m_binding.Init( button, state, callback );
}
 
/*------------------------------------------------------------------------------
unregister a key handler
------------------------------------------------------------------------------*/
void InputManager::UnbindInput( Key::Type key, InputState::Type state, InputCallbackFn callback ) {
    uint16_t index = m_bindingInUseHead;
    while( index != 0xffff ) {
        InputBindingNode *node = &m_bindings[ index ];
        if( node->m_binding.Equals( key, state, callback ) ) {
           FreeBinding( node );
           return;
        }
        index = node->m_next;
    }
    AssertAlways( "this input handler isn't registered" );
}

/*------------------------------------------------------------------------------
unregister a button handler
------------------------------------------------------------------------------*/
void InputManager::UnbindInput( Button::Type button, InputState::Type state, InputCallbackFn callback ) {
    uint16_t index = m_bindingInUseHead;
    while( index != 0xffff ) {
        InputBindingNode *node = &m_bindings[ index ];
        if( node->m_binding.Equals( button, state, callback ) ) {
           FreeBinding( node );
           return;
        }
        index = node->m_next;
    }
    AssertAlways( "this input handler isn't registered" );
}

/*------------------------------------------------------------------------------
call registered input callbacks
------------------------------------------------------------------------------*/
void InputManager::DispatchInput( float dt ) {
#if 0
    // left button pressed
    if( m_currentMouseState[ Button::kLeft ] == InputState::kDown && m_previousMouseState[ Button::kLeft ] == InputState::kUp ) {
        g_uiManager.OnMouseButton( Button::kLeft, true );
    }

    // right button pressed
    if( m_currentMouseState[ Button::kRight ] == InputState::kDown && m_previousMouseState[ Button::kRight ] == InputState::kUp ) {
        g_uiManager.OnMouseButton( Button::kRight, true );
    }

    // left button released
    if( m_currentMouseState[ Button::kLeft ] == InputState::kUp && m_previousMouseState[ Button::kLeft ] == InputState::kDown ) {
        g_uiManager.OnMouseButton( Button::kLeft, false );
    }

    // right button released
    if( m_currentMouseState[ Button::kRight ] == InputState::kUp && m_previousMouseState[ Button::kRight ] == InputState::kDown ) {
        g_uiManager.OnMouseButton( Button::kRight, false );
    }

    // call all input handlers
    uint16_t index = m_bindingInUseHead;
    while( index != 0xffff ) {
        InputBindingNode *node = &m_bindings[ index ];
        node->m_binding.Call( dt );
        index = node->m_next;
    }
#endif
}

/*------------------------------------------------------------------------------
get the mouse position in pixel coordinates
------------------------------------------------------------------------------*/
Point InputManager::GetMousePosition() const {
    Point mousePosition = m_mousePosition;

    Point resolution = g_gameWindow.GetResolution();
    Point clientSize = g_gameWindow.GetClientSize();
    
    float scaleX = (float)resolution.x / clientSize.x;
    float scaleY = (float)resolution.y / clientSize.y;

    mousePosition.x = (int16_t)( mousePosition.x * scaleX );
    mousePosition.y = (int16_t)( mousePosition.y * scaleY );

    return mousePosition;
}

/*------------------------------------------------------------------------------
process mouse input
------------------------------------------------------------------------------*/
void InputManager::OnMouseMove( int16_t x, int16_t y ) {
    Point oldPosition = GetMousePosition();
    m_mousePosition = Point( x, y );
    m_mouseDelta = GetMousePosition() - oldPosition;

#if 0
    if( g_uiManager.IsMouseOver() ) {
        m_currentGameMouseState[ Button::kLeft  ] = InputState::kUp;
        m_currentGameMouseState[ Button::kRight ] = InputState::kUp;
    }
#endif
}

/*------------------------------------------------------------------------------
process mouse input
------------------------------------------------------------------------------*/
void InputManager::OnMouseButton( Button::Type button, bool pressed ) {
    m_currentMouseState[ button ] = pressed ? InputState::kDown : InputState::kUp;

#if 0
    m_currentGameMouseState[ button ] = pressed ? InputState::kDown : InputState::kUp;
    if( g_uiManager.IsMouseOver() ) m_currentGameMouseState[ button ] = InputState::kUp;
#endif
}

/*------------------------------------------------------------------------------
initialize the binding linked list
------------------------------------------------------------------------------*/
void InputManager::ResetBindings() {
    m_bindingFreeHead = 0;
    m_bindingInUseHead = 0xffff;

    InputBindingNode *bindingNode = m_bindings;
    for( int32_t i = 0; i < kMaxInputBindings; ++i, ++bindingNode ) {
        bindingNode->m_prev = (uint16_t)( i - 1 );          // m_handles[ 0 ].m_prev will be 0xffff
        bindingNode->m_next = (uint16_t)( i + 1 );
    }
    m_bindings[ kMaxInputBindings - 1 ].m_next = 0xffff;
}

/*------------------------------------------------------------------------------
remove a binding node from the free list
------------------------------------------------------------------------------*/
InputBindingNode* InputManager::AllocateBinding() {
    // remove a handler from the free list
    Assert( m_bindingFreeHead != 0xffff, "all input bindings are in use" );

    uint16_t allocatedIndex = m_bindingFreeHead;
    InputBindingNode *node = &m_bindings[ m_bindingFreeHead ];

    // advance the free binding head
    m_bindingFreeHead = node->m_next;

    // add the component to the front of the list in-use list
    node->m_next = m_bindingInUseHead;
    node->m_prev = 0xffff;
    if( m_bindingInUseHead != 0xffff ) {
        InputBindingNode *headNode = &m_bindings[ m_bindingInUseHead ];
        headNode->m_prev = allocatedIndex;
    }
    m_bindingInUseHead = allocatedIndex;

    return node;
}

/*------------------------------------------------------------------------------
remove a binding node from the in-use list and add it to the free list
------------------------------------------------------------------------------*/
void InputManager::FreeBinding( InputBindingNode *node ) {
    uint16_t nodeIndex = (uint16_t)( node - m_bindings );

    // remove the node from the in-use list
    if( node->m_prev != 0xffff ) m_bindings[ node->m_prev ].m_next = node->m_next;
    if( node->m_next != 0xffff ) m_bindings[ node->m_next ].m_prev = node->m_prev;
    if( nodeIndex == m_bindingInUseHead ) m_bindingInUseHead = node->m_next;

    // add the handler to the front of the free list
    InputBindingNode *oldFreeHead = &m_bindings[ m_bindingFreeHead ];
    oldFreeHead->m_prev = nodeIndex;
    node->m_prev = 0xffff;
    node->m_next = m_bindingFreeHead;
    m_bindingFreeHead = nodeIndex;
}