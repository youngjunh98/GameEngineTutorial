#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>

class ShaderProgram
{
public:
	ShaderProgram ();
	virtual ~ShaderProgram ();

	bool Initialize (LPCTSTR sourcePath);
	void Destroy ();

	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout () const;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader () const;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader () const;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};

