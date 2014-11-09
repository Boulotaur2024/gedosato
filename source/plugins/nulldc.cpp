#include "plugins/nulldc.h"

#include "utils/d3d9_utils.h"
#include "renderstate_manager_dx9.h"

NullDcPlugin::~NullDcPlugin() {
	SAFEDELETE(bloom);
}

void NullDcPlugin::initialize(unsigned rw, unsigned rh, D3DFORMAT bbformat, D3DFORMAT dssformat) {
	GenericDepthPlugin::initialize(rw, rh, bbformat, dssformat);
	if(Settings::get().getEnableBloom()) bloom = new Bloom(d3ddev, rw, rh, 0.9f, 1.0f, 0.5f);
}

void NullDcPlugin::reportStatus() {
	GenericDepthPlugin::reportStatus();
	if(bloom && doBloom) Console::get().add("Bloom enabled");
	else Console::get().add("Bloom disabled");
}

void NullDcPlugin::preDownsample(IDirect3DSurface9* backBuffer) {
	GenericDepthPlugin::process(backBuffer);
	if(doBloom && bloom) {
		d3ddev->StretchRect(backBuffer, NULL, tmp->getSurf(), NULL, D3DTEXF_NONE);
		bloom->go(tmp->getTex(), tmp->getTex(), backBuffer);
	}
}

HRESULT NullDcPlugin::redirectCreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) {
	if (Width == 0) {
		Width = Settings::get().getRenderWidth();
		Height = Settings::get().getRenderHeight();
	}
	HRESULT res = d3ddev->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
	return res;
}

HRESULT NullDcPlugin::redirectStretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) {
	RECT newRect;
	newRect.left = 0;
	newRect.top = 0;
	newRect.right = Settings::get().getRenderWidth();
	newRect.bottom = Settings::get().getRenderHeight();
	return d3ddev->StretchRect(pSourceSurface, pSourceRect, pDestSurface, &newRect, Filter);
}

void NullDcPlugin::reloadShaders() {
	GenericDepthPlugin::reloadShaders();
}
