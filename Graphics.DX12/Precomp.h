#pragma once

#include "External/Graphics.DX12.h"

#include <Platform.Win32.h>

#include <d3d12.h>
#include <dxgi1_6.h>

#include "D3DX12.h"

#include <mcom_ptr.h>
using mst::com_ptr;

#define CHECK_HRESULT(x) CheckHResult(x, __FUNCTION__ "(): call failed: " #x)

inline void CheckHResult(HRESULT hr, const char* errorMessage)
{
	if (FAILED(hr))
	{
		Common::FatalError("%s HRESULT: 0x%x", errorMessage, (uint32)hr);
	}
}