#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SingletonBase.h"                  // base class
#include "InputTypes.h"                     // key and button enums
#include "InputBindingNode.h"               // linked list node

/*------------------------------------------------------------------------------
tracks keyboard and mouse state
------------------------------------------------------------------------------*/
class InputManager : public SingletonBase< InputManager > {
public:
    // initialize the singleton
    void Initialize();

    // deinitialize the singleton
    void Deinitialize();

    // prepare for the next frame
    void FrameEnd();

    // register a key or button handler
    void BindInput( Key::Type    key,    InputState::Type state, InputCallbackFn callback );
    void BindInput( Button::Type button, InputState::Type state, InputCallbackFn callback );

    // unregister a key or button handler
    void UnbindInput( Key::Type    key,    InputState::Type state, InputCallbackFn callback );
    void UnbindInput( Button::Type button, InputState::Type state, InputCallbackFn callback );

    // call registered input callbacks
    void DispatchInput( float dt );

    // update input state
    inline void OnKey( Key::Type key, bool pressed ) { m_currentKeyState[ key ] = pressed ? InputState::kDown : InputState::kUp; }
    void OnMouseMove( int16_t x, int16_t y );
    void OnMouseButton( Button::Type button, bool pressed );
    inline void OnMouseWheel( int16_t amount ) { m_mouseWheelDelta = amount; }
    inline void OnChar( wchar_t charcode ) { UnreferencedVariable( charcode ); /* do nothing */ }

    // accessors
    Point GetMousePosition() const;
    inline Point   GetMouseDelta()      const { return m_mouseDelta;      }
    inline int16_t GetMouseWheelDelta() const { return m_mouseWheelDelta; }
    inline InputState::Type GetKeyState( Key::Type key )             const { return m_currentKeyState  [ key    ]; }
    inline InputState::Type GetMouseState( Button::Type button )     const { return m_currentMouseState[ button ]; }
    inline InputState::Type GetGameMouseState( Button::Type button ) const { return m_currentGameMouseState[ button ]; }
    inline InputState::Type GetPreviousKeyState( Key::Type key )             const { return m_previousKeyState  [ key    ]; }
    inline InputState::Type GetPreviousMouseState( Button::Type button )     const { return m_previousMouseState[ button ]; }
    inline InputState::Type GetPreviousGameMouseState( Button::Type button ) const { return m_previousGameMouseState[ button ]; }

private:
    // initialize the binding linked list
    void ResetBindings();

    // remove a binding node from the free list and add it to the in-use list
    InputBindingNode* AllocateBinding();

    // remove a binding node from the in-use list and add it to the free list
    void FreeBinding( InputBindingNode *node );

private:
    InputState::Type m_currentKeyState   [ Key::kCount    ];    // current state of each key
    InputState::Type m_previousKeyState  [ Key::kCount    ];    // previous state of each key
    InputState::Type m_currentMouseState [ Button::kCount ];    // current state of each mouse button
    InputState::Type m_previousMouseState[ Button::kCount ];    // previous state of each mouse button

    InputState::Type m_currentGameMouseState [ Button::kCount ];    // these are modified versions of the true mouse state
    InputState::Type m_previousGameMouseState[ Button::kCount ];    // they're modified if a UI element blocks a mousedown event

    Point   m_mousePosition;                                    // current mouse position
    Point   m_mouseDelta;                                       // amount mouse moved since last frame
    int16_t m_mouseWheelDelta;                                  // amount that mouse wheel scrolled this frame

    InputBindingNode    *m_bindings;         // pre-allocated linked list nodes
    uint16_t             m_bindingFreeHead;  // linked list head pointing to free list
    uint16_t             m_bindingInUseHead; // linked list head pointing to in-use bindings
};

// global singleton
extern InputManager g_inputManager;

#endif // INPUT_MANAGER_H