#include "Renderer.h"
#include "D3D11.h"

extern D3D11 g_d3d11;

Renderer::Renderer () :
	m_shader (nullptr), m_mesh (nullptr), m_texture (nullptr)
{
}

Renderer::~Renderer ()
{
	m_shader = nullptr;
	m_mesh = nullptr;
	m_texture = nullptr;
}

void Renderer::OnUpdate (float deltaTime)
{
}

void Renderer::OnRender ()
{
	if (m_shader != nullptr)
	{
		// 지정한 Shader와 InputLayout을 사용한다.
		g_d3d11.SetInputLayout (m_shader->GetInputLayout ());
		g_d3d11.SetVertexShader (m_shader->GetVertexShader ());
		g_d3d11.SetPixelShader (m_shader->GetPixelShader ());
	}

	if (m_texture != nullptr)
	{
		// 지정한 Shader Resource와 Sampler를 바인딩한다.
		g_d3d11.BindPixelShaderResource (m_texture->GetResourceView (), 0);
		g_d3d11.BindPixelShaderSampler (m_texture->GetSmapler (), 0);
	}

	if (m_mesh != nullptr)
	{
		// 지정한 Vertex 버퍼와 Index 버퍼를 그린다.
		g_d3d11.DrawIndexed (m_mesh->GetVertexBuffer (), sizeof (Vertex), m_mesh->GetIndexBuffer (), m_mesh->GetIndexCount ());
	}
}

void Renderer::SetShaderProgram (ShaderProgram* shader)
{
	m_shader = shader;
}

void Renderer::SetMesh (Mesh* mesh)
{
	m_mesh = mesh;
}

void Renderer::SetTexture (Texture* texture)
{
	m_texture = texture;
}
