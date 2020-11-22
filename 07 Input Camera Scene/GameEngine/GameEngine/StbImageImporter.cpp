#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_resize.h>

#include "StbImageImporter.h"

bool StbImageImporter::Import (StbImage& image, const unsigned char* fileData, int fileSize, bool bForceAlphaChannel, bool bForceGray)
{
	int channels = STBI_default;

	// 이미지 정보를 불러온다.
	if (stbi_info_from_memory (fileData, fileSize, nullptr, nullptr, &channels) == 0)
	{
		return false;
	}

	// 이미지 채널 포맷을 지정한다.
	bool bNoSourceAlpha = channels == STBI_grey || channels == STBI_rgb;
	bool bIsRGB = channels == STBI_rgb || channels == STBI_rgb_alpha;

	if (bForceAlphaChannel && bNoSourceAlpha)
	{
		channels += 1;
	}

	if (bForceGray && bIsRGB)
	{
		channels -= 2;
	}

	image.m_channels = channels;

	// 이미지 포맷을 불러온다.
	bool bIsHDR = stbi_is_hdr_from_memory (fileData, fileSize) == 1;
	bool bIs16Bit = stbi_is_16_bit_from_memory (fileData, fileSize) == 1;

	// 이미지 포맷에 따라 이미지를 불러온다.
	if (bIsHDR)
	{
		image.m_data = stbi_loadf_from_memory (fileData, fileSize, &image.m_width, &image.m_height, nullptr, channels);
		image.m_byte = 4;
	}
	else if (bIs16Bit)
	{
		image.m_data = stbi_load_16_from_memory (fileData, fileSize, &image.m_width, &image.m_height, nullptr, channels);
		image.m_byte = 2;
	}
	else
	{
		image.m_data = stbi_load_from_memory (fileData, fileSize, &image.m_width, &image.m_height, nullptr, channels);
		image.m_byte = 1;
	}

	if (image.m_data == nullptr)
	{
		return false;
	}

	return true;
}

bool StbImageImporter::Resize (StbImage& resizedImage, const StbImage& sourceImage, int resizedWidth, int resizedHeight)
{
	resizedImage.m_data = STBI_MALLOC (sourceImage.m_byte * sourceImage.m_width * sourceImage.m_height * sourceImage.m_channels);
	resizedImage.m_width = resizedWidth;
	resizedImage.m_height = resizedHeight;
	resizedImage.m_channels = sourceImage.m_channels;
	resizedImage.m_byte = sourceImage.m_byte;

	int alphaIndex = sourceImage.IsAlphaChannel () ? sourceImage.m_channels - 1 : STBIR_ALPHA_CHANNEL_NONE;
	int flags = 0;

	stbir_edge edge = STBIR_EDGE_WRAP;
	stbir_filter filter = STBIR_FILTER_BOX;
	stbir_colorspace colorSpace = sourceImage.m_bLinear ? STBIR_COLORSPACE_LINEAR : STBIR_COLORSPACE_SRGB;

	if (sourceImage.m_byte == 1)
	{
		auto* sourceData = static_cast<stbir_uint8*> (sourceImage.m_data);
		auto* resizedData = static_cast<stbir_uint8*> (resizedImage.m_data);

		if (stbir_resize_uint8_generic (sourceData, sourceImage.m_width, sourceImage.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
			sourceImage.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 0)
		{
			return false;
		}
	}
	else if (sourceImage.m_byte == 2)
	{
		auto* sourceData = static_cast<stbir_uint16*> (sourceImage.m_data);
		auto* resizedData = static_cast<stbir_uint16*> (resizedImage.m_data);

		if (stbir_resize_uint16_generic (sourceData, sourceImage.m_width, sourceImage.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
			sourceImage.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 0)
		{
			return false;
		}
	}
	else if (sourceImage.m_byte == 4)
	{
		auto* sourceData = static_cast<float*> (sourceImage.m_data);
		auto* resizedData = static_cast<float*> (resizedImage.m_data);

		if (stbir_resize_float_generic (sourceData, sourceImage.m_width, sourceImage.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
			sourceImage.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 0)
		{
			return false;
		}
	}

	return true;
}
