#pragma once

#include "D3D11.h"

class StbImage
{
public:
	StbImage ();
	~StbImage ();

	bool IsSquare () const;
	bool IsPowerOfTwo () const;

	bool IsGray () const;
	bool IsAlphaChannel () const;

	DXGI_FORMAT GetFormat () const;
	int GetRowPitch () const;

	// Pointer to Image Data
	void* m_data;

	// Width of Image
	int m_width;

	// Height of Image
	int m_height;

	// Number of Channel
	// 1 -> Gray
	// 2 -> Gray + Alpha
	// 3 -> RGB
	// 4 -> RGB + Alpha
	int m_channels;

	// Byte Size of Pixel Component
	// 1 -> 8-bit
	// 2 -> 16-bit
	// 4 -> 32-bit
	int m_byte;

	// Is Image Color Space Linear?
	bool m_bLinear;
};

