#include <cstring>
#include <hidusage.h>

#include "Input.h"

Input::Input () : m_keyStates { State::Up }, m_prevKeyStates { State::Up },
	m_mouseButtonStates { State::Up }, m_prevMouseButtonStates { State::Up },
	m_mousePositionX (0.0f), m_mousePositionY (0.0f),
	m_mouseDeltaX (0.0f), m_mouseDeltaY (0.0f),
	m_mouseWheelScrollDelta (0.0f)
{
}

Input::~Input ()
{
}

bool Input::Initialize (HWND hWnd)
{
	RAWINPUTDEVICE rawInputDevice;
	rawInputDevice.usUsagePage = HID_USAGE_PAGE_GENERIC;
	rawInputDevice.usUsage = HID_USAGE_GENERIC_MOUSE;
	rawInputDevice.dwFlags = 0;
	rawInputDevice.hwndTarget = hWnd;

	if (RegisterRawInputDevices (&rawInputDevice, 1, sizeof (RAWINPUTDEVICE)) == FALSE)
	{
		return false;
	}

	return true;
}

void Input::Shutdown ()
{
	// Do nothing
}

void Input::Update ()
{
	static_assert (sizeof (m_keyStates) == sizeof (m_prevKeyStates), "Size of key states and previous states are different!");
	static_assert (sizeof (m_mouseButtonStates) == sizeof (m_prevMouseButtonStates), "Size of mouse button states and previous states are different!");

	std::memcpy (m_prevKeyStates, m_keyStates, sizeof (m_prevKeyStates));
	std::memcpy (m_prevMouseButtonStates, m_mouseButtonStates, sizeof (m_prevMouseButtonStates));

	m_mouseDeltaX = 0.0f;
	m_mouseDeltaY = 0.0f;

	m_mouseWheelScrollDelta = 0.0f;
}

void Input::ProcessKeyboardMessages (UINT message, WPARAM wParam, LPARAM lParam)
{
	bool bKeyDown = false;

	switch (message)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			bKeyDown = true;
			break;
		}

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			// Do nothing
			break;
		}
	}

	WPARAM virtualKey = wParam;

	if (virtualKey == VK_SHIFT)
	{
		// 16-23 bits (start from 0) are scan code.
		// 0000 0000  1111 1111  0000 0000  0000 0000 (32 bits)
		UINT scanCode = (static_cast<UINT> (lParam) & 0x00ff0000) >> 16;
		virtualKey = MapVirtualKey (scanCode, MAPVK_VSC_TO_VK_EX);
	}

	// 24th bit (start from 0) indicates whether the key is an extended key.
	// 0000 0001  0000 0000  0000 0000  0000 0000 (32 bits)
	bool bExtendedKey = (static_cast<UINT> (lParam) & 0x01000000) != 0;
	KeyCode keyCode = MapVirtualKeyToKeyCode (virtualKey, bExtendedKey);

	if (bKeyDown)
	{
		KeyDown (keyCode);
	}
	else
	{
		KeyUp (keyCode);
	}
}

void Input::ProcessMouseMessages (UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INPUT:
		{
			RAWINPUT rawInput;
			UINT rawInputSize = sizeof (RAWINPUT);

			if (GetRawInputData (reinterpret_cast<HRAWINPUT> (lParam), RID_INPUT, &rawInput, &rawInputSize, sizeof (RAWINPUTHEADER)) == UINT (-1))
			{
				// When failed to get raw input data.
				// Currently, do nothing.
			}

			if (rawInput.header.dwType == RIM_TYPEMOUSE)
			{
				float mouseDataX = static_cast<float> (rawInput.data.mouse.lLastX);
				float mouseDataY = static_cast<float> (rawInput.data.mouse.lLastY);

				if (rawInput.data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
				{
					MouseDelta (mouseDataX, mouseDataY);
				}

				/* Need more research
				if ((rawInput.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE)
				{
					bool isVirtualDesktop = (rawInput.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;
					isVirtualDesktop = (rawInput.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;

					int width = GetSystemMetrics (isVirtualDesktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
					int height = GetSystemMetrics (isVirtualDesktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);

					POINT normalizedAbsoluteCoordinates;
					normalizedAbsoluteCoordinates.x = (mouseDataX / 65535.0f) * width;
					normalizedAbsoluteCoordinates.y = (mouseDataY / 65535.0f) * height;

					if (ScreenToClient (m_hWnd, &normalizedAbsoluteCoordinates))
					{
						float absoluteX = static_cast<float> (normalizedAbsoluteCoordinates.x);
						float absoluteY = static_cast<float> (normalizedAbsoluteCoordinates.y);

						MousePosition (absoluteX, absoluteY);
					}
				}
				else if ((rawInput.data.mouse.usFlags & MOUSE_MOVE_RELATIVE) == MOUSE_MOVE_RELATIVE)
				{
					MouseDelta (mouseDataX, mouseDataY);
				}
				*/
			}

			break;
		}

		case WM_MOUSEMOVE:
		case WM_MOUSEHOVER:
		{
			POINTS position = MAKEPOINTS (lParam);
			MousePosition (static_cast<float> (position.x), static_cast<float> (position.y));
			break;
		}

		case WM_LBUTTONDOWN:
		{
			MouseButtonDown (MouseButton::Left);
			break;
		}

		case WM_LBUTTONUP:
		{
			MouseButtonUp (MouseButton::Left);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			MouseButtonDown (MouseButton::Right);
			break;
		}

		case WM_RBUTTONUP:
		{
			MouseButtonUp (MouseButton::Right);
			break;
		}

		case WM_MBUTTONDOWN:
		{
			MouseButtonDown (MouseButton::Middle);
			break;
		}

		case WM_MBUTTONUP:
		{
			MouseButtonUp (MouseButton::Middle);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			float mouseScrollDelta = static_cast<float> (GET_WHEEL_DELTA_WPARAM (wParam));
			MouseWheelScrollDelta (mouseScrollDelta);
			break;
		}

		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			break;
	}
}

bool Input::GetKey (KeyCode keyCode) const
{
	return keyCode == KeyCode::None ? false : m_keyStates[static_cast<int> (keyCode)] == State::Down;
}

bool Input::GetKeyUp (KeyCode keyCode) const
{
	if (keyCode == KeyCode::None)
	{
		return false;
	}

	int index = static_cast<int> (keyCode);
	return m_prevKeyStates[index] == State::Down && m_keyStates[index] == State::Up;
}

bool Input::GetKeyDown (KeyCode keyCode) const
{
	if (keyCode == KeyCode::None)
	{
		return false;
	}

	int index = static_cast<int> (keyCode);
	return m_prevKeyStates[index] == State::Up && m_keyStates[index] == State::Down;
}

bool Input::GetMouseButton (MouseButton mouseButton) const
{
	State state = State::Up;

	if (mouseButton == MouseButton::Left)
	{
		state = m_mouseButtonStates[0];
	}
	else if (mouseButton == MouseButton::Middle)
	{
		state = m_mouseButtonStates[1];
	}
	else if (mouseButton == MouseButton::Right)
	{
		state = m_mouseButtonStates[2];
	}

	return state == State::Down;
}

bool Input::GetMouseButtonUp (MouseButton mouseButton) const
{
	State prevState = State::Up;
	State state = State::Up;

	if (mouseButton == MouseButton::Left)
	{
		prevState = m_prevMouseButtonStates[0];
		state = m_mouseButtonStates[0];
	}
	else if (mouseButton == MouseButton::Middle)
	{
		prevState = m_prevMouseButtonStates[1];
		state = m_mouseButtonStates[1];
	}
	else if (mouseButton == MouseButton::Right)
	{
		prevState = m_prevMouseButtonStates[2];
		state = m_mouseButtonStates[2];
	}

	return prevState == State::Down && state == State::Up;
}

bool Input::GetMouseButtonDown (MouseButton mouseButton) const
{
	State prevState = State::Up;
	State state = State::Up;

	if (mouseButton == MouseButton::Left)
	{
		prevState = m_prevMouseButtonStates[0];
		state = m_mouseButtonStates[0];
	}
	else if (mouseButton == MouseButton::Middle)
	{
		prevState = m_prevMouseButtonStates[1];
		state = m_mouseButtonStates[1];
	}
	else if (mouseButton == MouseButton::Right)
	{
		prevState = m_prevMouseButtonStates[2];
		state = m_mouseButtonStates[2];
	}

	return prevState == State::Up && state == State::Down;
}

float Input::GetMousePositionX () const
{
	return m_mousePositionX;
}

float Input::GetMousePositionY () const
{
	return m_mousePositionY;
}

float Input::GetMouseDeltaX () const
{
	return m_mouseDeltaX;
}

float Input::GetMouseDeltaY () const
{
	return m_mouseDeltaY;
}

float Input::GetMouseWheelScrollDelta () const
{
	return m_mouseWheelScrollDelta;
}

KeyCode Input::MapVirtualKeyToKeyCode (WPARAM virtualKey, bool bExtendedKey)
{
	switch (virtualKey)
	{
		case VK_ESCAPE:     return KeyCode::Escape;
		case VK_RETURN:     return bExtendedKey ? KeyCode::NumpaEnter : KeyCode::Enter;
		case VK_SPACE:      return KeyCode::Space;
		case VK_BACK:       return KeyCode::Backspace;
		case VK_TAB:        return KeyCode::Tab;
		case VK_CAPITAL:    return KeyCode::CapsLock;
		case VK_LSHIFT:     return KeyCode::LeftShift;
		case VK_RSHIFT:     return KeyCode::RightShift;
		case VK_CONTROL:    return bExtendedKey ? KeyCode::RightControl : KeyCode::LeftControl;
		case VK_MENU:       return bExtendedKey ? KeyCode::RightAlt : KeyCode::LeftAlt;

		case VK_SNAPSHOT:   return KeyCode::PrintScreen;
		case VK_SCROLL:     return KeyCode::ScrollLock;
		case VK_PAUSE:      return KeyCode::Pause;

		case VK_INSERT:     return KeyCode::Insert;
		case VK_DELETE:     return KeyCode::Delete;
		case VK_HOME:       return KeyCode::Home;
		case VK_END:        return KeyCode::End;
		case VK_PRIOR:      return KeyCode::PageUp;
		case VK_NEXT:       return KeyCode::PageDown;

		case 0x30:          return KeyCode::Alpha0;
		case 0x31:          return KeyCode::Alpha1;
		case 0x32:          return KeyCode::Alpha2;
		case 0x33:          return KeyCode::Alpha3;
		case 0x34:          return KeyCode::Alpha4;
		case 0x35:          return KeyCode::Alpha5;
		case 0x36:          return KeyCode::Alpha6;
		case 0x37:          return KeyCode::Alpha7;
		case 0x38:          return KeyCode::Alpha8;
		case 0x39:          return KeyCode::Alpha9;

		case 0x41:          return KeyCode::A;
		case 0x42:          return KeyCode::B;
		case 0x43:          return KeyCode::C;
		case 0x44:          return KeyCode::D;
		case 0x45:          return KeyCode::E;
		case 0x46:          return KeyCode::F;
		case 0x47:          return KeyCode::G;
		case 0x48:          return KeyCode::H;
		case 0x49:          return KeyCode::I;
		case 0x4A:          return KeyCode::J;
		case 0x4B:          return KeyCode::K;
		case 0x4C:          return KeyCode::L;
		case 0x4D:          return KeyCode::M;
		case 0x4E:          return KeyCode::N;
		case 0x4F:          return KeyCode::O;
		case 0x50:          return KeyCode::P;
		case 0x51:          return KeyCode::Q;
		case 0x52:          return KeyCode::R;
		case 0x53:          return KeyCode::S;
		case 0x54:          return KeyCode::T;
		case 0x55:          return KeyCode::U;
		case 0x56:          return KeyCode::V;
		case 0x57:          return KeyCode::W;
		case 0x58:          return KeyCode::X;
		case 0x59:          return KeyCode::Y;
		case 0x5A:          return KeyCode::Z;

		case VK_OEM_3:      return KeyCode::BackQuote;
		case VK_OEM_MINUS:  return KeyCode::Minus;
		case VK_OEM_PLUS:   return KeyCode::Plus;
		case VK_OEM_5:      return KeyCode::BackSlash;
		case VK_OEM_4:      return KeyCode::LeftBracket;
		case VK_OEM_6:      return KeyCode::RightBracket;
		case VK_OEM_1:      return KeyCode::Semicolon;
		case VK_OEM_7:      return KeyCode::Quote;
		case VK_OEM_COMMA:  return KeyCode::Comma;
		case VK_OEM_PERIOD: return KeyCode::Dot;
		case VK_OEM_2:      return KeyCode::Slash;

		case VK_F1:         return KeyCode::F1;
		case VK_F2:         return KeyCode::F2;
		case VK_F3:         return KeyCode::F3;
		case VK_F4:         return KeyCode::F4;
		case VK_F5:         return KeyCode::F5;
		case VK_F6:         return KeyCode::F6;
		case VK_F7:         return KeyCode::F7;
		case VK_F8:         return KeyCode::F8;
		case VK_F9:         return KeyCode::F9;
		case VK_F10:        return KeyCode::F10;
		case VK_F11:        return KeyCode::F11;
		case VK_F12:        return KeyCode::F12;

		case VK_LEFT:       return KeyCode::LeftArrow;
		case VK_RIGHT:      return KeyCode::RightArrow;
		case VK_UP:         return KeyCode::UpArrow;
		case VK_DOWN:       return KeyCode::DownArrow;

		case VK_NUMPAD0:    return KeyCode::Numpad0;
		case VK_NUMPAD1:    return KeyCode::Numpad1;
		case VK_NUMPAD2:    return KeyCode::Numpad2;
		case VK_NUMPAD3:    return KeyCode::Numpad3;
		case VK_NUMPAD4:    return KeyCode::Numpad4;
		case VK_NUMPAD5:    return KeyCode::Numpad5;
		case VK_NUMPAD6:    return KeyCode::Numpad6;
		case VK_NUMPAD7:    return KeyCode::Numpad7;
		case VK_NUMPAD8:    return KeyCode::Numpad8;
		case VK_NUMPAD9:    return KeyCode::Numpad9;

		case VK_NUMLOCK:    return KeyCode::NumLock;
		case VK_DIVIDE:     return KeyCode::NumpadDivide;
		case VK_MULTIPLY:   return KeyCode::NumpadMultiply;
		case VK_SUBTRACT:   return KeyCode::NumpadMinus;
		case VK_ADD:        return KeyCode::NumpadPlus;
		case VK_DECIMAL:    return KeyCode::NumpadDot;
	}

	return KeyCode::None;
}

void Input::KeyDown (KeyCode keyCode)
{
	if (keyCode == KeyCode::None)
	{
		return;
	}

	int index = static_cast<int> (keyCode);
	m_keyStates[index] = State::Down;
}

void Input::KeyUp (KeyCode keyCode)
{
	if (keyCode == KeyCode::None)
	{
		return;
	}

	int index = static_cast<int> (keyCode);
	m_keyStates[index] = State::Up;
}

void Input::MouseButtonDown (MouseButton mouseButton)
{
	if (mouseButton == MouseButton::Left)
	{
		m_mouseButtonStates[0] = State::Down;
	}
	else if (mouseButton == MouseButton::Middle)
	{
		m_mouseButtonStates[1] = State::Down;
	}
	else if (mouseButton == MouseButton::Right)
	{
		m_mouseButtonStates[2] = State::Down;
	}
}

void Input::MouseButtonUp (MouseButton mouseButton)
{
	if (mouseButton == MouseButton::Left)
	{
		m_mouseButtonStates[0] = State::Up;
	}
	else if (mouseButton == MouseButton::Middle)
	{
		m_mouseButtonStates[1] = State::Up;
	}
	else if (mouseButton == MouseButton::Right)
	{
		m_mouseButtonStates[2] = State::Up;
	}
}

void Input::MousePosition (float x, float y)
{
	m_mousePositionX = x;
	m_mousePositionY = y;
}

void Input::MouseDelta (float deltaX, float deltaY)
{
	m_mouseDeltaX = deltaX;
	m_mouseDeltaY = deltaY;
}

void Input::MouseWheelScrollDelta (float scrollDeta)
{
	m_mouseWheelScrollDelta = scrollDeta;
}
