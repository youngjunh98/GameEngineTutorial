#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "D3D11.h"

Renderer::Renderer () :
	m_shader (nullptr), m_mesh (nullptr), m_texture (nullptr),
	m_roughness (0.0f), m_metallic (0.0f)
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
	Transform& transform = *GetGameObject ().GetComponent<Transform> ();
	Vector3 position = transform.GetPosition ();
	Quaternion rotation = transform.GetRotation ();
	Vector3 scale = transform.GetScale ();

	// Transpose of inverse of A is the inverse of transpose of A.
	// (A-1)T = (AT)-1
	MatrixBuffer matrixBuffer;
	matrixBuffer.LocalToWorld = Matrix4x4::ScaleRotateTranslate (position, rotation, scale).Transposed ();
	matrixBuffer.View = Camera::Main->GetViewMatrix ().Transposed ();
	matrixBuffer.Projection = Camera::Main->GetProjectionMatrix ().Transposed ();

	g_d3d11.UpdateMatrixBuffer (matrixBuffer);

	PBRBuffer pbrBuffer;
	pbrBuffer.Roughness = m_roughness;
	pbrBuffer.Metallic = m_metallic;

	g_d3d11.UpdatePBRBuffer (pbrBuffer);

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

void Renderer::SetRoughness (float roughness)
{
	m_roughness = roughness;
}

void Renderer::SetMetallic (float metallic)
{
	m_metallic = metallic;
}
