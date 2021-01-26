#include "Precomp.h"

#include "Event.h"
#include "DX12Renderer.h"

using namespace ::CainEngine::Graphics;

unique_ptr<Implementation::IRenderer> DX12::CreateInstance()
{
	return make_unique<DX12::DX12Renderer>();
}