#include "Precompiled.h"
#include "InputBinding.h"
#include "InputManager.h"   // checking input states

/*------------------------------------------------------------------------------
initialize
------------------------------------------------------------------------------*/
void InputBinding::Init( Key::Type key, InputState::Type state, InputCallbackFn callback ) {
    m_type     = kKey;
    m_key      = key;
    m_state    = state;
    m_callback = callback;
}

/*------------------------------------------------------------------------------
initialize
------------------------------------------------------------------------------*/
void InputBinding::Init( Button::Type button, InputState::Type state, InputCallbackFn callback ) {
    m_type     = kButton;
    m_button   = button;
    m_state    = state;
    m_callback = callback;
}

/*------------------------------------------------------------------------------
calls the handler function if the input state is triggered
------------------------------------------------------------------------------*/
void InputBinding::Call( float dt ) {
    bool callFunction = false;

    if( m_type == kKey ) {
        // check if the state matches the current keyboard state
        switch( m_state ) {
        case InputState::kUp:       callFunction = g_inputManager.GetKeyState( m_key ) == InputState::kUp;   break;
        case InputState::kDown:     callFunction = g_inputManager.GetKeyState( m_key ) == InputState::kDown; break;
        case InputState::kPressed:  callFunction = g_inputManager.GetKeyState( m_key ) == InputState::kDown && g_inputManager.GetPreviousKeyState( m_key ) == InputState::kUp;   break;
        case InputState::kReleased: callFunction = g_inputManager.GetKeyState( m_key ) == InputState::kUp   && g_inputManager.GetPreviousKeyState( m_key ) == InputState::kDown; break;
        }
    }
    else if( m_type == kButton ) {
        // check if the state matches the current mouse state
        switch( m_state ) {
        case InputState::kUp:       callFunction = g_inputManager.GetGameMouseState( m_button ) == InputState::kUp;   break;
        case InputState::kDown:     callFunction = g_inputManager.GetGameMouseState( m_button ) == InputState::kDown; break;
        case InputState::kPressed:  callFunction = g_inputManager.GetGameMouseState( m_button ) == InputState::kDown && g_inputManager.GetPreviousGameMouseState( m_button ) == InputState::kUp;   break;
        case InputState::kReleased: callFunction = g_inputManager.GetGameMouseState( m_button ) == InputState::kUp   && g_inputManager.GetPreviousGameMouseState( m_button ) == InputState::kDown; break;
        }
    }

    if( callFunction ) m_callback( dt );
}

/*------------------------------------------------------------------------------
comparison
------------------------------------------------------------------------------*/
bool InputBinding::Equals( Key::Type key, InputState::Type state, InputCallbackFn callback ) const {
    return ( m_type     == kKey  &&
             m_key      == key   &&
             m_state    == state &&
             m_callback == callback );
}

/*------------------------------------------------------------------------------
comparison
------------------------------------------------------------------------------*/
bool InputBinding::Equals( Button::Type button, InputState::Type state, InputCallbackFn callback ) const {
    return ( m_type     == kButton  &&
             m_button   == button   &&
             m_state    == state    &&
             m_callback == callback );
}