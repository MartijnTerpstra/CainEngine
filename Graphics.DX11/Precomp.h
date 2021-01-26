#pragma once

#include "External/Graphics.DX11.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include <mcom_ptr.h>
using mst::com_ptr;

#define CHECK_HRESULT(x) CheckHResult(x, __FUNCTION__ "(): call failed: " #x)

inline void CheckHResult(HRESULT hr, const char* errorMessage)
{
	if (FAILED(hr))
	{
		Common::FatalError("%s HRESULT: 0x%x", errorMessage, (uint32_t)hr);
	}
}

namespace CainEngine::Graphics::DX11 {

class DX11Renderer;

}