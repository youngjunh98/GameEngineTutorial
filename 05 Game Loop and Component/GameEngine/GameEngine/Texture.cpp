#include "Texture.h"

extern D3D11 g_d3d11;

Texture::Texture () :
	m_width (0), m_height (0), m_mipMapCount(1), m_format (DXGI_FORMAT_UNKNOWN)
{
}

Texture::~Texture ()
{
}

bool Texture::Initialize (const StbImage& image)
{
	// 이미지로 2D 텍스처를 생성한다.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	DXGI_FORMAT format = image.GetFormat ();

	if (g_d3d11.CreateTexture2D (image.m_width, image.m_height, 1, 1, format, image.m_data, image.GetRowPitch (), texture) == false)
	{
		return false;
	}

	// 생성된 2D 텍스처의 Resource view를 생성한다.
	if (g_d3d11.CreateShaderResourceView (texture, format, 0, 1, 1, 0, m_resourceView) == false)
	{
		return false;
	}

	// 텍스처와 함께 사용할 Sampler를 생성한다.
	// (Address Mode: Wrap, Filte Mode: Point)
	if (g_d3d11.CreateSampler (D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT, 1, m_sampler) == false)
	{
		return false;
	}

	return true;
}

void Texture::Destroy ()
{
	m_width = 0;
	m_height = 0;
	m_mipMapCount = 0;
	m_format = DXGI_FORMAT_UNKNOWN;

	m_resourceView = nullptr;
	m_sampler = nullptr;
}

unsigned int Texture::GetWidth () const
{
	return m_width;
}

unsigned int Texture::GetHeight () const
{
	return m_height;
}

unsigned int Texture::GetMipMapCount () const
{
	return m_mipMapCount;
}

DXGI_FORMAT Texture::GetFormat () const
{
	return m_format;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::GetResourceView () const
{
	return m_resourceView;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> Texture::GetSmapler () const
{
	return m_sampler;
}
