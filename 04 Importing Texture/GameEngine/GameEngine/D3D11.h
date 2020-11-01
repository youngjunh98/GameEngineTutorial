#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

class D3D11
{
public:
	D3D11 ();
	virtual ~D3D11 ();

	bool Initialize (HWND hWnd, unsigned int swapChainWidth, unsigned int swapChainHeight, bool isFullScreen,
					 bool isVSync, unsigned int refreshRate, bool isMsaa, unsigned int msaaSampleCount);
	void Shutdown ();

	void ClearRenderTarget (float red, float green, float blue, float alpha);
	void ClearDpethStencil (float depth, unsigned char stencil);
	void PresentSwapChain (bool isVSync);

	bool CreateInputLayout (Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout, const Microsoft::WRL::ComPtr<ID3DBlob>& compiledVertexShaderCode);
	void SetInputLayout (const Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout);

	bool CompileShader (LPCTSTR sourcePath, LPCSTR profile, LPCSTR entryPoint, Microsoft::WRL::ComPtr<ID3DBlob>& compiledCode);

	bool CreateVertexShader (Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertexShader, const Microsoft::WRL::ComPtr<ID3DBlob>& compiledCode);
	void SetVertexShader (const Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertexShader);

	bool CreatePixelShader (Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixelShader, const Microsoft::WRL::ComPtr<ID3DBlob>& compiledCode);
	void SetPixelShader (const Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixelShader);

	bool CreateVertexBuffer (const void* buffer, unsigned int bufferByteSize, Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer);
	bool CreateIndexBuffer (const void* buffer, unsigned int bufferByteSize, Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer);

	void DrawIndexed (const Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, unsigned int vertexSize,
					  const Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer, unsigned int indexCount);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	D3D_FEATURE_LEVEL m_featureLevel;
};
