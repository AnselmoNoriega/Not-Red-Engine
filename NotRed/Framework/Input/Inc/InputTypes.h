#pragma once

namespace NotRed::Input
{
	enum class KeyCode : uint32_t
	{
		// Keyboard roll 1
		ESCAPE		= VK_ESCAPE,
		F1			= VK_F1,
		F2			= VK_F2,
		F3			= VK_F3,
		F4			= VK_F4,
		F5			= VK_F5,
		F6			= VK_F6,
		F7			= VK_F7,
		F8			= VK_F8,
		F9			= VK_F9,
		F10			= VK_F10,
		F11			= VK_F11,
		F12			= VK_F12,

		// Keyboard roll 2
		GRAVE		= VK_OEM_3,
		ONE			= '1',
		TWO			= '2',
		THREE		= '3',
		FOUR		= '4',
		FIVE		= '5',
		SIX			= '6',
		SEVEN		= '7',
		EIGHT		= '8',
		NINE		= '9',
		ZERO		= '0',
		MINUS		= VK_OEM_MINUS,
		EQUALS		= VK_OEM_PLUS,
		BACKSPACE	= VK_BACK,

		// Keyboard roll 3
		TAB			= VK_TAB,
		Q			= 'Q',
		W			= 'W',
		E			= 'E',
		R			= 'R',
		T			= 'T',
		Y			= 'Y',
		U			= 'U',
		I			= 'I',
		O			= 'O',
		P			= 'P',
		LBRACKET	= VK_OEM_4,
		RBRACKET	= VK_OEM_6,
		BACKSLASH	= VK_OEM_5,

		// Keyboard roll 4
		A			= 'A',
		S			= 'S',
		D			= 'D',
		F			= 'F',
		G			= 'G',
		H			= 'H',
		J			= 'J',
		K			= 'K',
		L			= 'L',
		SEMICOLON	= VK_OEM_1,
		APOSTROPHE	= VK_OEM_7,
		ENTER		= VK_RETURN,

		// Keyboard roll 5
		Z			= 'Z',
		X			= 'X',
		C			= 'C',
		V			= 'V',
		B			= 'B',
		N			= 'N',
		M			= 'M',
		COMMA		= VK_OEM_COMMA,
		PERIOD		= VK_OEM_PERIOD,
		SLASH		= VK_OEM_2,

		// Lock keys
		CAPSLOCK	= VK_CAPITAL,
		NUMLOCK		= VK_NUMLOCK,
		SCROLLLOCK	= VK_SCROLL,

		// Numpad keys
		NUMPAD1		= VK_NUMPAD1,
		NUMPAD2		= VK_NUMPAD2,
		NUMPAD3		= VK_NUMPAD3,
		NUMPAD4		= VK_NUMPAD4,
		NUMPAD5		= VK_NUMPAD5,
		NUMPAD6		= VK_NUMPAD6,
		NUMPAD7		= VK_NUMPAD7,
		NUMPAD8		= VK_NUMPAD8,
		NUMPAD9		= VK_NUMPAD9,
		NUMPAD0		= VK_NUMPAD0,
		NUM_ADD		= VK_ADD,
		NUM_SUB		= VK_SUBTRACT,
		NUM_MUL		= VK_MULTIPLY,
		NUM_DIV		= VK_DIVIDE,
		NUM_ENTER	= VK_RETURN,
		NUM_DECIMAL	= VK_DECIMAL,

		// Navigation keys
		INS			= VK_INSERT,
		DEL			= VK_DELETE,
		HOME		= VK_HOME,
		END			= VK_END,
		PGUP		= VK_PRIOR,
		PGDN		= VK_NEXT,

		// Support keys
		LSHIFT		= VK_SHIFT,
		RSHIFT		= VK_SHIFT,
		LCONTROL	= VK_CONTROL,
		RCONTROL	= VK_CONTROL,
		LALT		= VK_MENU,
		RALT		= VK_MENU,
		LWIN		= VK_LWIN,
		RWIN		= VK_RWIN,
		SPACE		= VK_SPACE,

		// Arrow keys
		UP			= VK_UP,
		DOWN		= VK_DOWN,
		LEFT		= VK_LEFT,
		RIGHT		= VK_RIGHT,
	};

	enum class MouseButton : uint32_t
	{
		LBUTTON		= 0,
		RBUTTON		= 1,
		MBUTTON		= 2,
	};
}