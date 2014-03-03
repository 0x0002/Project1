#ifndef INPUT_BINDING_H
#define INPUT_BINDING_H

#include "InputTypes.h"     // key enum

typedef void (*InputCallbackFn)( float );

/*------------------------------------------------------------------------------
binds a key or button state to a handler function
------------------------------------------------------------------------------*/
class InputBinding {
public:
    // initialize
    void Init( Key::Type    key,    InputState::Type state, InputCallbackFn callback );
    void Init( Button::Type button, InputState::Type state, InputCallbackFn callback );

    // calls the handler function if the input state is triggered
    void Call( float dt );

    // comparison
    bool Equals( Key::Type    key,    InputState::Type state, InputCallbackFn callback ) const;
    bool Equals( Button::Type button, InputState::Type state, InputCallbackFn callback ) const;

private:
    enum Type {
        kKey,
        kButton
    };

    Type                m_type;         // specifies which field in the union is valid
    union {
        Key::Type       m_key;          //
        Button::Type    m_button;       //
    };
    InputState::Type    m_state;        // input state that triggers a callback
    InputCallbackFn     m_callback;     // called when the input state matches for the key or button
};

#endif // INPUT_HANDLER_H