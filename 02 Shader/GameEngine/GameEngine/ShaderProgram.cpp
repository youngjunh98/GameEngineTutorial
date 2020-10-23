#include "ShaderProgram.h"
#include "D3D11.h"

extern D3D11 g_d3d11;

ShaderProgram::ShaderProgram ()
{
}

ShaderProgram::~ShaderProgram ()
{
}

bool ShaderProgram::Initialize (LPCTSTR sourcePath)
{
	Microsoft::WRL::ComPtr<ID3DBlob> compiledVertexShader;

	// Vertex Shader 소스를 컴파일한다.
	if (g_d3d11.CompileShader (sourcePath, "vs_5_0", "VertexMain", compiledVertexShader) == false)
	{
		return false;
	}

	// 컴파일한 소스로 Vertex Shader를 생성한다.
	if (g_d3d11.CreateVertexShader (m_vertexShader, compiledVertexShader) == false)
	{
		return false;
	}

	// 컴파일한 소스로 Input Layout을 생성한다.
	if (g_d3d11.CreateInputLayout (m_inputLayout, compiledVertexShader) == false)
	{
		return false;
	}

	Microsoft::WRL::ComPtr<ID3DBlob> compiledPixelShader;

	// Pixel Shader 소스를 컴파일한다.
	if (g_d3d11.CompileShader (sourcePath, "ps_5_0", "PixelMain", compiledPixelShader) == false)
	{
		return false;
	}

	// 컴파일한 소스로 Pixel Shader를 생성한다.
	if (g_d3d11.CreatePixelShader (m_pixelShader, compiledPixelShader) == false)
	{
		return false;
	}

	// 생성한 Shader와 InputLayout을 사용한다.
	g_d3d11.SetInputLayout (m_inputLayout);
	g_d3d11.SetVertexShader (m_vertexShader);
	g_d3d11.SetPixelShader (m_pixelShader);

	return true;
}

void ShaderProgram::Destroy ()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
}
