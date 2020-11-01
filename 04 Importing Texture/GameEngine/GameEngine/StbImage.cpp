#include <stb_image.h>

#include "StbImage.h"

StbImage::StbImage ():
	m_data (nullptr), m_width (0), m_height (0), m_channels (0),
	m_byte (0), m_bLinear (true)
{
}

StbImage::~StbImage ()
{
	if (m_data != nullptr)
	{
		stbi_image_free (m_data);
	}

	m_data = nullptr;
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	m_byte = 0;
}

bool StbImage::IsSquare () const
{
	return (m_width == m_height) && (m_width > 0) && (m_height > 0);
}

bool StbImage::IsPowerOfTwo () const
{
	return (m_width % 2 == 0) && (m_height % 2 == 0);
}

bool StbImage::IsGray () const
{
	return (m_channels == STBI_grey) || (m_channels == STBI_grey_alpha);
}

bool StbImage::IsAlphaChannel () const
{
	return (m_channels == STBI_grey_alpha) || (m_channels == STBI_rgb_alpha);
}

DXGI_FORMAT StbImage::GetFormat () const
{
	if (m_channels == 1)
	{
		if (m_byte == 1)
		{
			return DXGI_FORMAT_R8_UNORM;
		}
		else if (m_byte == 2)
		{
			return DXGI_FORMAT_R16_UNORM;
		}
		else if (m_byte == 4)
		{
			return DXGI_FORMAT_R32_FLOAT;
		}
	}
	else if (m_channels == 2)
	{
		if (m_byte == 1)
		{
			return DXGI_FORMAT_R8G8_UNORM;
		}
		else if (m_byte == 2)
		{
			return DXGI_FORMAT_R16G16_UNORM;
		}
		else if (m_byte == 4)
		{
			return DXGI_FORMAT_R32G32_FLOAT;
		}
	}
	else if (m_channels == 3)
	{
		if (m_byte == 1)
		{
			return DXGI_FORMAT_UNKNOWN;
		}
		else if (m_byte == 2)
		{
			return DXGI_FORMAT_UNKNOWN;
		}
		else if (m_byte == 4)
		{
			return DXGI_FORMAT_R32G32B32_FLOAT;
		}
	}
	else if (m_channels == 4)
	{
		if (m_byte == 1)
		{
			return m_bLinear ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else if (m_byte == 2)
		{
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		}
		else if (m_byte == 4)
		{
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}

int StbImage::GetRowPitch () const
{
	// 이미지의 행이 몇 바이트인지 계산한다.
	// Ex) 2D 이미지 -> 이미지 가로줄 바이트 수
	return m_byte * m_width * m_channels;
}
