#include <tchar.h>
#include <memory>
#include <fstream>

#include "Application.h"
#include "D3D11.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "OBJImporter.h"
#include "FBXImporter.h"
#include "Texture.h"
#include"StbImageImporter.h"

D3D11 g_d3d11;
ShaderProgram g_shader;
Mesh g_mesh;
Texture g_texture;

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
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon (nullptr, IDI_WINLOGO);
	wc.hCursor = LoadCursor (nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_className.get ();

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

	// Shader를 초기화한다.
	if (g_shader.Initialize (L"../../../Shaders/LambertShader.hlsl") == false)
	{
		return false;
	}

	// Mesh를 초기화한다.
	std::unique_ptr<char[]> buffer;
	//std::ifstream file ("../../../Assets/monkey.obj", std::ifstream::binary);
	std::ifstream file ("../../../Assets/Container40F.fbx", std::ifstream::binary);
	size_t fileSize;

	if (file)
	{
		file.seekg (0, std::ios::end);
		fileSize = static_cast<size_t> (file.tellg ());
		file.seekg (0, std::ios::beg);

		buffer = std::make_unique<char[]> (fileSize);
		file.read (buffer.get (), fileSize);
	}
	else
	{
		return false;
	}

	//if (OBJImporter::Import (g_mesh, buffer.get (), fileSize) == false)
	//{
	//	return false;
	//}

	if (FBXImporter::Start () == false)
	{
		return false;
	}

	if (FBXImporter::Import (g_mesh, buffer.get (), fileSize) == false)
	{
		return false;
	}

	FBXImporter::Shutdown ();

	// Texture를 초기화한다.
	std::ifstream textureFile ("../../../Assets/Container_Color.tga", std::ifstream::binary);

	if (textureFile)
	{
		textureFile.seekg (0, std::ios::end);
		fileSize = static_cast<size_t> (textureFile.tellg ());
		textureFile.seekg (0, std::ios::beg);

		buffer = std::make_unique<char[]> (fileSize);
		textureFile.read (buffer.get (), fileSize);
	}
	else
	{
		return false;
	}

	StbImage image;
	const unsigned char* imageData = reinterpret_cast<const unsigned char*> (buffer.get ());

	if (StbImageImporter::Import (image, imageData, fileSize, true, false) == false)
	{
		return false;
	}

	if (g_texture.Initialize (image) == false)
	{
		return false;
	}

	return true;
}

void Application::Shutdown ()
{
	// D3D11을 종료한다.
	g_d3d11.Shutdown ();

	// 윈도우를 nullptr로 설정한다.
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

		// 게임 루프
		g_d3d11.ClearRenderTarget (0.0f, 0.0f, 0.0f, 1.0f);
		g_d3d11.ClearDpethStencil (1.0f, 0);

		// 지정한 Shader와 InputLayout을 사용한다.
		g_d3d11.SetInputLayout (g_shader.GetInputLayout ());
		g_d3d11.SetVertexShader (g_shader.GetVertexShader ());
		g_d3d11.SetPixelShader (g_shader.GetPixelShader ());

		// 지정한 Shader Resource와 Sampler를 바인딩한다.
		g_d3d11.BindPixelShaderResource (g_texture.GetResourceView (), 0);
		g_d3d11.BindPixelShaderSampler (g_texture.GetSmapler (), 0);

		// 지정한 Vertex 버퍼와 Index 버퍼를 그린다.
		g_d3d11.DrawIndexed (g_mesh.GetVertexBuffer (), sizeof (Vertex), g_mesh.GetIndexBuffer (), g_mesh.GetIndexCount ());

		// Swap chain의 후면 버퍼를 화면에 표시한다.
		g_d3d11.PresentSwapChain (true);
	}
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			// 생성한 윈도우를 파괴한다.
			DestroyWindow (hWnd);
			return 0;
		}

		case WM_DESTROY:
		{
			// 종료 메시지를 전달한다.
			PostQuitMessage (0);
			return 0;
		}
	}

	return DefWindowProc (hWnd, message, wParam, lParam);
}
