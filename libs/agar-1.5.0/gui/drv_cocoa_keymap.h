/*	Public domain	*/

/*
 * Map Cocoa function keys to Agar keysyms.
 */
static const struct ag_cocoa_function_key {
	AG_KeySym keySym;			/* Agar keycode */
	unichar uc;				/* Cocoa unicode */
} agCocoaFunctionKeys[] = {
	{ AG_KEY_UP,		NSUpArrowFunctionKey },
	{ AG_KEY_DOWN,		NSDownArrowFunctionKey },
	{ AG_KEY_LEFT,		NSLeftArrowFunctionKey },
	{ AG_KEY_RIGHT,		NSRightArrowFunctionKey },
	{ AG_KEY_PAGEUP,	NSPageUpFunctionKey },
	{ AG_KEY_PAGEDOWN,	NSPageDownFunctionKey },
	{ AG_KEY_HOME,		NSHomeFunctionKey },
	{ AG_KEY_BEGIN,		NSBeginFunctionKey },
	{ AG_KEY_END,		NSEndFunctionKey },
	{ AG_KEY_INSERT,	NSInsertFunctionKey },
	{ AG_KEY_DELETE,	NSDeleteFunctionKey },
	{ AG_KEY_PRINT_SCREEN,	NSPrintScreenFunctionKey },
	{ AG_KEY_SCROLLOCK,	NSScrollLockFunctionKey },
	{ AG_KEY_PAUSE,		NSPauseFunctionKey },
	{ AG_KEY_SYSREQ,	NSSysReqFunctionKey },
	{ AG_KEY_BREAK,		NSBreakFunctionKey },
	{ AG_KEY_RESET,		NSResetFunctionKey },
	{ AG_KEY_STOP,		NSStopFunctionKey },
	{ AG_KEY_MENU,		NSMenuFunctionKey },
	{ AG_KEY_USER,		NSUserFunctionKey },
	{ AG_KEY_SYSTEM,	NSSystemFunctionKey },
	{ AG_KEY_PRINT,		NSPrintFunctionKey },
	{ AG_KEY_CLEAR_LINE,	NSClearLineFunctionKey },
	{ AG_KEY_CLEAR_DISPLAY,	NSClearDisplayFunctionKey },
	{ AG_KEY_INSERT_LINE,	NSInsertLineFunctionKey },
	{ AG_KEY_DELETE_LINE,	NSDeleteLineFunctionKey },
	{ AG_KEY_INSERT_CHAR,	NSInsertCharFunctionKey },
	{ AG_KEY_DELETE_CHAR,	NSDeleteCharFunctionKey },
	{ AG_KEY_PREV,		NSPrevFunctionKey },
	{ AG_KEY_NEXT,		NSNextFunctionKey },
	{ AG_KEY_SELECT,	NSSelectFunctionKey },
	{ AG_KEY_EXECUTE,	NSExecuteFunctionKey },
	{ AG_KEY_UNDO,		NSUndoFunctionKey },
	{ AG_KEY_REDO,		NSRedoFunctionKey },
	{ AG_KEY_FIND,		NSFindFunctionKey },
	{ AG_KEY_HELP,		NSHelpFunctionKey },
	{ AG_KEY_MODE_SWITCH,	NSModeSwitchFunctionKey },
	{ AG_KEY_F1,		NSF1FunctionKey },
	{ AG_KEY_F2,		NSF2FunctionKey },
	{ AG_KEY_F3,		NSF3FunctionKey },
	{ AG_KEY_F4,		NSF4FunctionKey },
	{ AG_KEY_F5,		NSF5FunctionKey },
	{ AG_KEY_F6,		NSF6FunctionKey },
	{ AG_KEY_F7,		NSF7FunctionKey },
	{ AG_KEY_F8,		NSF8FunctionKey },
	{ AG_KEY_F9,		NSF9FunctionKey },
	{ AG_KEY_F10,		NSF10FunctionKey },
	{ AG_KEY_F11,		NSF11FunctionKey },
	{ AG_KEY_F12,		NSF12FunctionKey },
	{ AG_KEY_F13,		NSF13FunctionKey },
	{ AG_KEY_F14,		NSF14FunctionKey },
	{ AG_KEY_F15,		NSF15FunctionKey },
	{ AG_KEY_F16,		NSF16FunctionKey },
	{ AG_KEY_F17,		NSF17FunctionKey },
	{ AG_KEY_F18,		NSF18FunctionKey },
	{ AG_KEY_F19,		NSF19FunctionKey },
	{ AG_KEY_F20,		NSF20FunctionKey },
	{ AG_KEY_F21,		NSF21FunctionKey },
	{ AG_KEY_F22,		NSF22FunctionKey },
	{ AG_KEY_F23,		NSF23FunctionKey },
	{ AG_KEY_F24,		NSF24FunctionKey },
	{ AG_KEY_F25,		NSF25FunctionKey },
	{ AG_KEY_F26,		NSF26FunctionKey },
	{ AG_KEY_F27,		NSF27FunctionKey },
	{ AG_KEY_F28,		NSF28FunctionKey },
	{ AG_KEY_F29,		NSF29FunctionKey },
	{ AG_KEY_F30,		NSF30FunctionKey },
	{ AG_KEY_F31,		NSF31FunctionKey },
	{ AG_KEY_F32,		NSF32FunctionKey },
	{ AG_KEY_F33,		NSF33FunctionKey },
	{ AG_KEY_F34,		NSF34FunctionKey },
	{ AG_KEY_F35,		NSF35FunctionKey },
	{ AG_KEY_BACKSPACE,	0x007f },
	{ AG_KEY_RETURN,	0x0003 },		/* Extra "Enter" key
							   on my Powerbook G4 */
};
static Uint agCocoaFunctionKeysSize = sizeof(agCocoaFunctionKeys) /
                                      sizeof(agCocoaFunctionKeys[0]);

/*
 * Map Cocoa modifier masks to Agar modifier keysyms.
 */
static const struct ag_cocoa_keymod_entry {
	AG_KeySym keySym;			/* Agar keycode */
	Uint keyMask;				/* Cocoa modifier mask */
} agCocoaKeymod[] = {
	{ AG_KEY_CAPSLOCK,	NSAlphaShiftKeyMask },
	{ AG_KEY_RSHIFT,	NSShiftKeyMask },
	{ AG_KEY_RCTRL,		NSControlKeyMask },
	{ AG_KEY_RALT,		NSAlternateKeyMask },
	{ AG_KEY_RMETA,		NSCommandKeyMask },
	{ AG_KEY_NUMLOCK,	NSNumericPadKeyMask },
	{ AG_KEY_HELP,		NSHelpKeyMask },
	{ AG_KEY_FUNCTION,	NSFunctionKeyMask }
};
static Uint agCocoaKeymodSize = sizeof(agCocoaKeymod) / sizeof(agCocoaKeymod[0]);

#if 0
/* Map Carbon key codes to virtual Agar keys. */
static const struct ag_carbon_keymap_entry {
	AG_KeySym keySym;			/* Agar keycode */
	int keyCode;				/* Carbon keycode */
} agCarbonKeymap[] = {
	/* ANSI-specific */
	{ AG_KEY_A,		0x00 },
	{ AG_KEY_S,		0x01 },
	{ AG_KEY_D,		0x02 },
	{ AG_KEY_F,		0x03 },
	{ AG_KEY_H,		0x04 },
	{ AG_KEY_G,		0x05 },
	{ AG_KEY_Z,		0x06 },
	{ AG_KEY_X,		0x07 },
	{ AG_KEY_C,		0x08 },
	{ AG_KEY_V,		0x09 },
	{ AG_KEY_B,		0x0b },
	{ AG_KEY_Q,		0x0c },
	{ AG_KEY_W,		0x0d },
	{ AG_KEY_E,		0x0e },
	{ AG_KEY_R,		0x0f },
	{ AG_KEY_Y,		0x10 },
	{ AG_KEY_T,		0x11 },
	{ AG_KEY_1,		0x12 },
	{ AG_KEY_2,		0x13 },
	{ AG_KEY_3,		0x14 },
	{ AG_KEY_4,		0x15 },
	{ AG_KEY_6,		0x16 },
	{ AG_KEY_5,		0x17 },
	{ AG_KEY_EQUALS,	0x18 },
	{ AG_KEY_9,		0x19 },
	{ AG_KEY_7,		0x1a },
	{ AG_KEY_MINUS,		0x1b },
	{ AG_KEY_8,		0x1c },
	{ AG_KEY_0,		0x1d },
	{ AG_KEY_RIGHTBRACKET,	0x1e },
	{ AG_KEY_O,		0x1f },
	{ AG_KEY_U,		0x20 },
	{ AG_KEY_LEFTBRACKET,	0x21 },
	{ AG_KEY_I,		0x22 },
	{ AG_KEY_P,		0x23 },
	{ AG_KEY_L,		0x25 },
	{ AG_KEY_J,		0x26 },
	{ AG_KEY_QUOTE,		0x27 },
	{ AG_KEY_K,		0x28 },
	{ AG_KEY_SEMICOLON,	0x29 },
	{ AG_KEY_BACKSLASH,	0x2a },
	{ AG_KEY_COMMA,		0x2b },
	{ AG_KEY_SLASH,		0x2c },
	{ AG_KEY_N,		0x2d },
	{ AG_KEY_M,		0x2e },
	{ AG_KEY_PERIOD,	0x2f },
	{ AG_KEY_GRAVE,		0x32 },
	{ AG_KEY_KP_PERIOD,	0x41 },
	{ AG_KEY_KP_MULTIPLY,	0x43 },
	{ AG_KEY_KP_PLUS,	0x45 },
	{ AG_KEY_KP_CLEAR,	0x47 },
	{ AG_KEY_KP_DIVIDE,	0x4b },
	{ AG_KEY_KP_ENTER,	0x4c },
	{ AG_KEY_KP_MINUS,	0x4e },
	{ AG_KEY_KP_EQUALS,	0x51 },
	{ AG_KEY_KP0,		0x52 },
	{ AG_KEY_KP1,		0x53 },
	{ AG_KEY_KP2,		0x54 },
	{ AG_KEY_KP3,		0x55 },
	{ AG_KEY_KP4,		0x56 },
	{ AG_KEY_KP5,		0x57 },
	{ AG_KEY_KP6,		0x58 },
	{ AG_KEY_KP7,		0x59 },
	{ AG_KEY_KP8,		0x5b },
	{ AG_KEY_KP9,		0x5c },
	/* Keymap-independent */
	{ AG_KEY_RETURN,	0x24 },
	{ AG_KEY_TAB,		0x30 },
	{ AG_KEY_SPACE,		0x31 },
	{ AG_KEY_BACKSPACE,	0x33 },
	{ AG_KEY_ESCAPE,	0x35 },
	{ AG_KEY_COMMAND,	0x37 },
	{ AG_KEY_LSHIFT,	0x38 },
	{ AG_KEY_CAPSLOCK,	0x39 },
	{ AG_KEY_LMETA,		0x3a },
	{ AG_KEY_LCTRL,		0x3b },
	{ AG_KEY_RSHIFT,	0x3c },
	{ AG_KEY_RMETA,		0x3d },
	{ AG_KEY_RCTRL,		0x3e },
	{ AG_KEY_FUNCTION,	0x3f },
	{ AG_KEY_F17,		0x40 },
	{ AG_KEY_VOLUME_UP,	0x48 },
	{ AG_KEY_VOLUME_DOWN,	0x49 },
	{ AG_KEY_VOLUME_MUTE,	0x4a },
	{ AG_KEY_F18,		0x4f },
	{ AG_KEY_F19,		0x50 },
	{ AG_KEY_F20,		0x5a },
	{ AG_KEY_F5,		0x60 },
	{ AG_KEY_F6,		0x61 },
	{ AG_KEY_F7,		0x62 },
	{ AG_KEY_F3,		0x63 },
	{ AG_KEY_F8,		0x64 },
	{ AG_KEY_F9,		0x65 },
	{ AG_KEY_F11,		0x67 },
	{ AG_KEY_F13,		0x69 },
	{ AG_KEY_F16,		0x6a },
	{ AG_KEY_F14,		0x6b },
	{ AG_KEY_F10,		0x6d },
	{ AG_KEY_F12,		0x6f },
	{ AG_KEY_F15,		0x71 },
	{ AG_KEY_HELP,		0x72 },
	{ AG_KEY_HOME,		0x73 },
	{ AG_KEY_PAGEUP,	0x74 },
	{ AG_KEY_DELETE,	0x75 },
	{ AG_KEY_F4,		0x76 },
	{ AG_KEY_END,		0x77 },
	{ AG_KEY_F2,		0x78 },
	{ AG_KEY_PAGEDOWN,	0x79 },
	{ AG_KEY_F1,		0x7a },
	{ AG_KEY_LEFT,		0x7b },
	{ AG_KEY_RIGHT,		0x7c },
	{ AG_KEY_DOWN,		0x7d },
	{ AG_KEY_UP,		0x7e }
};
static Uint agCarbonKeymapSize = sizeof(agCarbonKeymap) / sizeof(agCarbonKeymap[0]);
#endif
