#pragma once

#include <memory>
#include <Windows.h>

class Application
{
public:
	Application ();
	virtual ~Application ();

	bool Initialize (LPCTSTR name, unsigned int width, unsigned int height, bool isFullScreen);
	void Shutdown ();

	void Update ();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	std::unique_ptr<TCHAR[]> m_className;
};

using WindowProcedure = LRESULT (*) (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
