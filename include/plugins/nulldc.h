#pragma once

#include "generic.h"
#include "generic_depth.h"
#include "bloom.h"
#include "ssao.h"
#include "dof.h"
#include "depth_texture.h"

class NullDcPlugin : public GenericDepthPlugin {

	Bloom* bloom;
	bool doBloom = true;

public:
	NullDcPlugin(IDirect3DDevice9* device, RSManagerDX9 &manager) : GenericDepthPlugin(device, manager)
	{ }

	virtual ~NullDcPlugin();

	virtual void initialize(unsigned rw, unsigned rh, D3DFORMAT bbformat, D3DFORMAT dssformat) override;
	virtual void reportStatus() override;

	virtual void preDownsample(IDirect3DSurface9* backBuffer) override;

	virtual HRESULT redirectCreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) override;
	virtual HRESULT redirectStretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) override;

	virtual void toggleBloom() override { if(bloom) { doBloom = !doBloom; Console::get().add(doBloom ? "Bloom enabled" : "Bloom disabled"); } else { Console::get().add("Bloom disabled in configuration!"); } }

	virtual void reloadShaders() override;
};
