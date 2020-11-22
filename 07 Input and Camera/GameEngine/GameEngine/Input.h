#pragma once

#include <Windows.h>

enum class KeyCode
{
	None,


	/* Normal Keys */

	Escape,
	Enter,
	Space,
	Backspace,
	Tab,
	CapsLock,
	LeftShift,
	RightShift,
	LeftControl,
	RightControl,
	LeftAlt,
	RightAlt,

	PrintScreen,
	ScrollLock,
	Pause,

	Insert,
	Delete,
	Home,
	End,
	PageUp,
	PageDown,

	Alpha0,
	Alpha1,
	Alpha2,
	Alpha3,
	Alpha4,
	Alpha5,
	Alpha6,
	Alpha7,
	Alpha8,
	Alpha9,

	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,

	BackQuote,
	Minus,
	Plus,
	BackSlash,
	LeftBracket,
	RightBracket,
	Semicolon,
	Quote,
	Comma,
	Dot,
	Slash,


	/* Function Keys */

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,


	/* Arrow Keys */

	LeftArrow,
	RightArrow,
	UpArrow,
	DownArrow,


	/* Numpad Keys */

	Numpad0,
	Numpad1,
	Numpad2,
	Numpad3,
	Numpad4,
	Numpad5,
	Numpad6,
	Numpad7,
	Numpad8,
	Numpad9,

	NumLock,
	NumpadDivide,
	NumpadMultiply,
	NumpadMinus,
	NumpadPlus,
	NumpaEnter,
	NumpadDot,
	NumpadEquals,
};

enum class MouseButton
{
	Left,
	Middle,
	Right
};

class Input
{
public:
	Input ();
	virtual ~Input ();

	bool Initialize (HWND hWnd);
	void Shutdown ();

	void Update ();
	void ProcessKeyboardMessages (UINT message, WPARAM wParam, LPARAM lParam);
	void ProcessMouseMessages (UINT message, WPARAM wParam, LPARAM lParam);

	bool GetKey (KeyCode keyCode) const;
	bool GetKeyUp (KeyCode keyCode) const;
	bool GetKeyDown (KeyCode keyCode) const;

	bool GetMouseButton (MouseButton mouseButton) const;
	bool GetMouseButtonUp (MouseButton mouseButton) const;
	bool GetMouseButtonDown (MouseButton mouseButton) const;

	float GetMousePositionX () const;
	float GetMousePositionY () const;

	float GetMouseDeltaX () const;
	float GetMouseDeltaY () const;

	float GetMouseWheelScrollDelta () const;

private:
	KeyCode MapVirtualKeyToKeyCode (WPARAM virtualKey, bool bExtendedKey);

	void KeyDown (KeyCode keyCode);
	void KeyUp (KeyCode keyCode);

	void MouseButtonDown (MouseButton mouseButton);
	void MouseButtonUp (MouseButton mouseButton);

	void MousePosition (float x, float y);
	void MouseDelta (float deltaX, float deltaY);

	void MouseWheelScrollDelta (float scrollDeta);

private:
	enum class State
	{
		Up,
		Down
	};

	State m_keyStates[256];
	State m_prevKeyStates[256];

	State m_mouseButtonStates[3];
	State m_prevMouseButtonStates[3];

	float m_mousePositionX;
	float m_mousePositionY;

	float m_mouseDeltaX;
	float m_mouseDeltaY;

	float m_mouseWheelScrollDelta;
};
