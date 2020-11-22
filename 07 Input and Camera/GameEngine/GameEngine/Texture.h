#pragma once

#include "D3D11.h"
#include "StbImage.h"

class Texture
{
public:
	Texture ();
	virtual ~Texture ();

	bool Initialize (const StbImage& image);
	void Destroy ();

	unsigned int GetWidth () const;
	unsigned int GetHeight () const;
	unsigned int GetMipMapCount () const;
	DXGI_FORMAT GetFormat () const;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView () const;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSmapler () const;

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_mipMapCount;
	DXGI_FORMAT m_format;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resourceView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
};

