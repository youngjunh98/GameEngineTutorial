// D3D11 라이브러리 링크
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")

#include "D3D11.h"
#include "ShaderProgram.h"

D3D11::D3D11 () : m_featureLevel()
{
}

D3D11::~D3D11 ()
{
}

bool D3D11::Initialize (HWND hWnd, unsigned int swapChainWidth, unsigned int swapChainHeight, bool isFullScreen, bool isVSync, unsigned int refreshRate, bool isMsaa, unsigned int msaaSampleCount)
{
	// 사용할 DirectX 11 버전
	const D3D_FEATURE_LEVEL targetFeatureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	const UINT targetFeatureLevelsCount = 2;

	// D3D11 Device 설정
	UINT deviceFlags = 0;

#if _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// D3D11 Device를 생성한다.
	if (FAILED (D3D11CreateDevice (nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, targetFeatureLevels, targetFeatureLevelsCount,
		D3D11_SDK_VERSION, m_device.ReleaseAndGetAddressOf (), &m_featureLevel, m_immediateContext.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	// 지원하는 MSAA 품질을 가져온다.
	UINT msaaQuality;
	m_device->CheckMultisampleQualityLevels (DXGI_FORMAT_R8G8B8A8_UNORM, msaaSampleCount, &msaaQuality);

	// DXGI 디바이스를 가져온다.
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	m_device.As (&dxgiDevice);

	// DXGI 어댑터를 가져온다.
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter (dxgiAdapter.ReleaseAndGetAddressOf ());

	// DXGI Factory를 가져온다.
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
	dxgiAdapter->GetParent (__uuidof (IDXGIFactory), &dxgiFactory);

	// Swap chain 설정
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
	swapChainDesc.BufferDesc.Width = swapChainWidth;
	swapChainDesc.BufferDesc.Height = swapChainHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = isVSync ? refreshRate : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = isFullScreen == false;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (isMsaa)
	{
		swapChainDesc.SampleDesc.Count = msaaSampleCount;
		swapChainDesc.SampleDesc.Quality = msaaQuality - 1;
	}

	// Swap chain 생성한다.
	if (FAILED (dxgiFactory->CreateSwapChain (m_device.Get (), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	// Swap chain의 Back buffer 가져온다.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	if (FAILED (m_swapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), (void**)backBuffer.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	// Render target view 만든다.
	if (FAILED (m_device->CreateRenderTargetView (backBuffer.Get (), nullptr, m_renderTargetView.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	// Depth stencil buffer 설정
	D3D11_TEXTURE2D_DESC depthStencilDescription = { };
	depthStencilDescription.Width = swapChainWidth;
	depthStencilDescription.Height = swapChainHeight;
	depthStencilDescription.MipLevels = 1;
	depthStencilDescription.ArraySize = 1;
	depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDescription.SampleDesc.Count = 1;
	depthStencilDescription.SampleDesc.Quality = 0;
	depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDescription.CPUAccessFlags = 0;
	depthStencilDescription.MiscFlags = 0;

	if (isMsaa)
	{
		depthStencilDescription.SampleDesc.Count = msaaSampleCount;
		depthStencilDescription.SampleDesc.Quality = msaaQuality - 1;
	}

	// Depth stencil buffer 만든다.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	if (FAILED (m_device->CreateTexture2D (&depthStencilDescription, nullptr, depthStencilBuffer.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	// Depth stencil view 만든다.
	if (FAILED (m_device->CreateDepthStencilView (depthStencilBuffer.Get (), nullptr, m_depthStencilView.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	// Output merger의 Render target 설정한다.
	m_immediateContext->OMSetRenderTargets (1, m_renderTargetView.GetAddressOf (), m_depthStencilView.Get ());

	// Viewport를 설정한다.
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<FLOAT> (swapChainWidth);
	viewport.Height = static_cast<FLOAT> (swapChainHeight);
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_immediateContext->RSSetViewports (1, &viewport);

	return true;
}

void D3D11::Shutdown ()
{
	// 렌더 타겟 바인딩 해제한다.
	ID3D11RenderTargetView* nullRenderTargetViews[] = { nullptr };
	ID3D11DepthStencilView* nullDepthStencilView = nullptr;
	m_immediateContext->OMSetRenderTargets (1, nullRenderTargetViews, nullDepthStencilView);

	// 생성한 자원 레퍼런스 해제한다.
	m_depthStencilView = nullptr;
	m_renderTargetView = nullptr;
	m_swapChain = nullptr;

	m_immediateContext.Reset ();
	m_device.Reset ();
}

void D3D11::ClearRenderTarget (float red, float green, float blue, float alpha)
{
	// Render Target을 지정한 색으로 덮어쓴다.
	FLOAT color[4] = { red, green, blue, alpha };
	m_immediateContext->ClearRenderTargetView (m_renderTargetView.Get (), color);
}

void D3D11::ClearDpethStencil (float depth, unsigned char stencil)
{
	// Depth와 Stencil을 지정한 값으로 덮어쓴다.
	m_immediateContext->ClearDepthStencilView (m_depthStencilView.Get (), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void D3D11::PresentSwapChain (bool isVSync)
{
	// Swap chain의 Back buffer와 Front Buffer를 교환한다.
	if (isVSync)
	{
		m_swapChain->Present (1, 0);
	}
	else
	{
		m_swapChain->Present (0, 0);
	}
}

bool D3D11::CreateInputLayout (Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout, const Microsoft::WRL::ComPtr<ID3DBlob>& compiledVertexShaderCode)
{
	// Input Layout을 설정한다.
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Input Layout을 생성한다.
	if (FAILED (m_device->CreateInputLayout (inputElementDescs, 3,
		compiledVertexShaderCode->GetBufferPointer (), compiledVertexShaderCode->GetBufferSize (),
		inputLayout.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	return true;
}

void D3D11::SetInputLayout (const Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout)
{
	// 지정한 Input Layout을 사용한다.
	m_immediateContext->IASetInputLayout (inputLayout.Get ());
}

bool D3D11::CompileShader (LPCTSTR sourcePath, LPCSTR profile, LPCSTR entryPoint, Microsoft::WRL::ComPtr<ID3DBlob>& compiledCode)
{
	Microsoft::WRL::ComPtr<ID3DBlob> errorMessages;

	// Shader 컴파일 옵션을 설정한다.
	UINT shaderCompileOptions = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;
	UINT effectCompileOptions = 0;

#if _DEBUG
	shaderCompileOptions |= D3DCOMPILE_DEBUG;
#endif

	// Shader를 컴파일한다.
	if (FAILED (D3DCompileFromFile (sourcePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile,
		shaderCompileOptions, effectCompileOptions, compiledCode.ReleaseAndGetAddressOf (), errorMessages.ReleaseAndGetAddressOf ())))
	{
		// 에러 메시지를 출력한다.
		if (errorMessages)
		{
			OutputDebugStringA (static_cast<char*> (errorMessages->GetBufferPointer ()));
		}
		else
		{
			OutputDebugString (TEXT ("Can't find shader file!\n"));
		}

		return false;
	}

	return true;
}

bool D3D11::CreateVertexShader (Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertexShader, const Microsoft::WRL::ComPtr<ID3DBlob>& compiledCode)
{
	if (!compiledCode)
	{
		return false;
	}

	LPVOID code = compiledCode->GetBufferPointer ();
	SIZE_T codeSize = compiledCode->GetBufferSize ();

	// Vertex Shader를 생성한다.
	if (FAILED (m_device->CreateVertexShader (code, codeSize, nullptr, vertexShader.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	return true;
}

void D3D11::SetVertexShader (const Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertexShader)
{
	// 지정한 Vertex Shader를 사용한다.
	if (vertexShader)
	{
		m_immediateContext->VSSetShader (vertexShader.Get (), nullptr, 0);
	}
	else
	{
		m_immediateContext->VSSetShader (nullptr, nullptr, 0);
	}
}

bool D3D11::CreatePixelShader (Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixelShader, const Microsoft::WRL::ComPtr<ID3DBlob>& compiledCode)
{
	if (!compiledCode)
	{
		return false;
	}

	LPVOID code = compiledCode->GetBufferPointer ();
	SIZE_T codeSize = compiledCode->GetBufferSize ();

	// Pixel Shader를 생성한다.
	if (FAILED (m_device->CreatePixelShader (code, codeSize, nullptr, pixelShader.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	return true;
}

void D3D11::SetPixelShader (const Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixelShader)
{
	// 지정한 Pixel Shader를 사용한다.
	if (pixelShader)
	{
		m_immediateContext->PSSetShader (pixelShader.Get (), nullptr, 0);
	}
	else
	{
		m_immediateContext->PSSetShader (nullptr, nullptr, 0);
	}
}

bool D3D11::CreateVertexBuffer (const void* buffer, unsigned int bufferByteSize, Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer)
{
	// Buffer를 Vertex Buffer로 설정한다.
	D3D11_BUFFER_DESC vertexBufferDescription = { };
	vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescription.ByteWidth = bufferByteSize;
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.CPUAccessFlags = 0;
	vertexBufferDescription.MiscFlags = 0;
	vertexBufferDescription.StructureByteStride = 0;

	// Vertex Buffer의 초기값을 지정한다.
	D3D11_SUBRESOURCE_DATA vertexBufferData = { };
	vertexBufferData.pSysMem = buffer;

	// Vertex Buffer를 생성한다.
	if (FAILED (m_device->CreateBuffer (&vertexBufferDescription, &vertexBufferData, vertexBuffer.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	return true;
}

bool D3D11::CreateIndexBuffer (const void* buffer, unsigned int bufferByteSize, Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer)
{
	// Buffer를 Index Buffer로 설정한다.
	D3D11_BUFFER_DESC indexBufferDescription = { };
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.ByteWidth = bufferByteSize;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;
	indexBufferDescription.StructureByteStride = 0;

	// Index Buffer의 초기값을 지정한다.
	D3D11_SUBRESOURCE_DATA indexBufferData = { };
	indexBufferData.pSysMem = buffer;

	// Index Buffer를 생성한다.
	if (FAILED (m_device->CreateBuffer (&indexBufferDescription, &indexBufferData, indexBuffer.ReleaseAndGetAddressOf ())))
	{
		return false;
	}

	return true;
}

void D3D11::DrawIndexed (const Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, unsigned int vertexSize,
						 const Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer, unsigned int indexCount)
{
	if (!vertexBuffer || !indexBuffer)
	{
		return;
	}

	UINT stride = vertexSize;
	UINT offset = 0;

	// 메시의 Topology(일종의 포맷)를 Triangle List로 설정한다.
	m_immediateContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 그리는데 사용할 Vertex Buffer와 Index Buffer를 지정한다.
	m_immediateContext->IASetVertexBuffers (0, 1, vertexBuffer.GetAddressOf (), &stride, &offset);
	m_immediateContext->IASetIndexBuffer (indexBuffer.Get (), DXGI_FORMAT_R32_UINT, 0);

	// Index를 사용해 그린다.
	m_immediateContext->DrawIndexed (indexCount, 0, 0);
}
