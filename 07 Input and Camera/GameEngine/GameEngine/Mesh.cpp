#include "Mesh.h"
#include "D3D11.h"

Mesh::Mesh () : m_vertexCount (0), m_indexCount (0)
{
}

Mesh::~Mesh ()
{
}

bool Mesh::Initialize (const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	m_vertexCount = vertices.size ();
	m_indexCount = indices.size ();

	// Vertex Buffer를 만든다.
	if (g_d3d11.CreateVertexBuffer (vertices.data (), sizeof (Vertex) * m_vertexCount, m_vertexBuffer) == false)
	{
		return false;
	}

	// Index Buffer를 만든다.
	if (g_d3d11.CreateIndexBuffer (indices.data (), sizeof (unsigned int) * m_indexCount, m_indexBuffer) == false)
	{
		return false;
	}

	return true;
}

void Mesh::Destroy ()
{
	m_vertexCount = 0;
	m_vertexBuffer = nullptr;

	m_indexCount = 0;
	m_indexBuffer = nullptr;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer () const
{
	return m_vertexBuffer;
}

unsigned int Mesh::GetVertexCount () const
{
	return m_vertexCount;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer () const
{
	return m_indexBuffer;
}

unsigned int Mesh::GetIndexCount () const
{
	return m_indexCount;
}
