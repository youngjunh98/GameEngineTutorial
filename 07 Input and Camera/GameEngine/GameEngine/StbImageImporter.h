#pragma once

#include "StbImage.h"

class StbImageImporter
{
public:
	static bool Import (StbImage& image, const unsigned char* fileData, int fileSize, bool bForceAlphaChannel, bool bForceGray);
	static bool Resize (StbImage& resizedImage, const StbImage& sourceImage, int resizedWidth, int resizedHeight);
};
