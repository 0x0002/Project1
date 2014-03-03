#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H

/*------------------------------------------------------------------------------
keyboard keys. these match up with Windows virtual key codes
------------------------------------------------------------------------------*/
namespace Key {
	enum Type {
		// misc. keys
		kEscape         = 0x1B,
		kSpace          = 0x20,
		kEnd            = 0x23,
		kHome           = 0x24,
		kLeft           = 0x25,
		kUp             = 0x26,
		kRight          = 0x27,
		kDown           = 0x28,
		kInsert         = 0x2D,
		kDelete         = 0x2E,

		// regular number keys (not num pad)
		k0              = 0x30,
		k1              = 0x31,
		k2              = 0x32,
		k3              = 0x33,
		k4              = 0x34,
		k5              = 0x35,
		k6              = 0x36,
		k7              = 0x37,
		k8              = 0x38,
		k9              = 0x39,

		// letter keys
		kA				= 0x41,
		kB				= 0x42,
		kC				= 0x43,
		kD				= 0x44,
		kE				= 0x45,
		kF				= 0x46,
		kG				= 0x47,
		kH				= 0x48,
		kI				= 0x49,
		kJ				= 0x4A,
		kK				= 0x4B,
		kL				= 0x4C,
		kM				= 0x4D,
		kN				= 0x4E,
		kO				= 0x4F,
		kP				= 0x50,
		kQ				= 0x51,
		kR				= 0x52,
		kS				= 0x53,
		kT				= 0x54,
		kU				= 0x55,
		kV				= 0x56,
		kW				= 0x57,
		kX				= 0x58,
		kY				= 0x59,
		kZ				= 0x5A,

		// num pad keys
		kNumPad0        = 0x60,
		kNumPad1        = 0x61,
		kNumPad2        = 0x62,
		kNumPad3        = 0x63,
		kNumPad4        = 0x64,
		kNumPad5        = 0x65,
		kNumPad6        = 0x66,
		kNumPad7        = 0x67,
		kNumPad8        = 0x68,
		kNumPad9        = 0x69,
		kMultiply       = 0x6A,
		kAdd            = 0x6B,
		kSeparator      = 0x6C,
		kSubtract       = 0x6D,
		kDecimal        = 0x6E,
		kDivide         = 0x6F,

		// function keys
		kF1             = 0x70,
		kF2             = 0x71,
		kF3             = 0x72,
		kF4             = 0x73,
		kF5             = 0x74,
		kF6             = 0x75,
		kF7             = 0x76,
		kF8             = 0x77,
		kF9             = 0x78,
		kF10            = 0x79,
		kF11            = 0x7A,
		kF12            = 0x7B,
		kF13            = 0x7C,
		kF14            = 0x7D,
		kF15            = 0x7E,
		kF16            = 0x7F,
		kF17            = 0x80,
		kF18            = 0x81,
		kF19            = 0x82,
		kF20            = 0x83,
		kF21            = 0x84,
		kF22            = 0x85,
		kF23            = 0x86,
		kF24            = 0x87,

		// modifier keys
		kLeftShift      = 0xA0,
		kRightShift     = 0xA1,
		kLeftControl    = 0xA2,
		kRightControl   = 0xA3,
		kLeftAlt        = 0xA4,
		kRightAlt       = 0xA5,

		kCount          = 0xFF
	};
}

/*------------------------------------------------------------------------------
mouse buttons
------------------------------------------------------------------------------*/
namespace Button {
	enum Type {
		kLeft           = 0,
		kRight          = 1,
		kMiddle         = 2,

        kCount
	};
}

/*------------------------------------------------------------------------------
state of a key or button
------------------------------------------------------------------------------*/
namespace InputState {
    enum Type {
        kUp             = 0,
        kDown           = 1,

        kPressed,
        kReleased
    };
}

/*------------------------------------------------------------------------------
bit flags
------------------------------------------------------------------------------*/
namespace KeyModifier {
	enum Type{
		kNone           = 0x00,
		kLeftShift      = 0x01,
		kRightShift     = 0x02,
		kLeftControl    = 0x04,
		kRightControl	= 0x08,
		kLeftAlt        = 0x10,
		kRightAlt       = 0x20,
	};
}

#endif // INPUT_TYPES_H