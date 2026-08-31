#include "Precomp.h"

#include "Event.h"
#include "DX12Renderer.h"

using namespace ::CainEngine::Graphics;

std::unique_ptr<API::IRenderer> DX12::createInstance()
{
	return std::make_unique<DX12::DX12Renderer>();
}