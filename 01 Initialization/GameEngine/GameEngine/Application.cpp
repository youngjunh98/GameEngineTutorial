#include <tchar.h>

#include "Application.h"
#include "D3D11.h"

D3D11 g_d3d11;

Application::Application () : m_hInstance (nullptr), m_hWnd (nullptr)
{
}

Application::~Application ()
{
}

bool Application::Initialize (LPCTSTR name, unsigned int width, unsigned int height, bool isFullScreen)
{
	// 윈도우 클래스 이름 복사한다.
	size_t classNameLength = _tcslen (name);
	m_className = std::make_unique<TCHAR[]> (classNameLength + 1);
	_tcsnccpy_s (m_className.get (), classNameLength + 1, name, classNameLength);

	// exe 파일 핸들을 가져온다.
	m_hInstance = GetModuleHandle (nullptr);

	// 윈도우 클래스를 설정한다.
	WNDCLASS wc = { };
	wc.hInstance = m_hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_className.get ();
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor (nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon (nullptr, IDI_WINLOGO);

	// 윈도우 클래스를 등록한다.
	if (RegisterClass (&wc) == false)
	{
		return false;
	}

	// 전체화면인지 아닌지에 따라서 스타일을 지정한다.
	DWORD style = 0;

	if (isFullScreen)
	{
		width = GetSystemMetrics (SM_CXSCREEN);
		height = GetSystemMetrics (SM_CYSCREEN);
		style |= WS_POPUP;
	}
	else
	{
		style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	}

	// 화면 크기를 가져온다.
	UINT screenWidth = GetSystemMetrics (SM_CXSCREEN);
	UINT screenHeight = GetSystemMetrics (SM_CYSCREEN);

	// 화면 크기와 윈도우 크기를 바탕으로
	// 윈도우를 화면 중앙에 맞추는 Rect를 계산한다.
	RECT windowRect = { };
	windowRect.left = (screenWidth / 2) - (width / 2);
	windowRect.top = (screenHeight / 2) - (height / 2);
	windowRect.right = (screenWidth / 2) + (width / 2);
	windowRect.bottom = (screenHeight / 2) + (height / 2);

	// 계산한 사각형이 Client Area일 때
	// 지정하 스타일에 맞는 윈도우 Rect를 계산한다.
	AdjustWindowRect (&windowRect, style, false);

	UINT windowX = windowRect.left;
	UINT windowY = windowRect.top;
	UINT windowWidth = windowRect.right - windowRect.left;
	UINT windowHeight = windowRect.bottom - windowRect.top;

	// 윈도우를 생성한다.
	m_hWnd = CreateWindow (
		m_className.get (),
		m_className.get (),
		style,
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		nullptr,
		nullptr,
		m_hInstance,
		nullptr
	);

	if (m_hWnd == nullptr)
	{
		return false;
	}

	// 윈도우를 표시한다.
	ShowWindow (m_hWnd, SW_SHOW);

	// D3D11을 초기화한다.
	if (g_d3d11.Initialize (m_hWnd, width, height, false, true, 60, true, 4) == false)
	{
		return false;
	}

	return true;
}

void Application::Shutdown ()
{
	// D3D11을 종료한다.
	g_d3d11.Shutdown ();

	// 생성한 윈도우를 파괴한다.
	DestroyWindow (m_hWnd);
	m_hWnd = nullptr;

	// 등록한 윈도우 클래스를 해제한다.
	UnregisterClass (m_className.get (), m_hInstance);
	m_hInstance = nullptr;
}

void Application::Update ()
{
	MSG message = { };

	while (true)
	{
		// 메시지가 있으면 처리한다.
		while (PeekMessage (&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				return;
			}

			TranslateMessage (&message);
			DispatchMessage (&message);
		}

		g_d3d11.ClearSwapChain (0.0f, 0.0f, 0.0f, 1.0f);
		g_d3d11.PresentSwapChain (true);
	}
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage (0);
			return 0;
		}
	}

	return DefWindowProc (hWnd, message, wParam, lParam);
}
