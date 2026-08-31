#include "Precomp.h"
#include "Win32CoreFactory.h"

using namespace ::CainEngine::Platform;

RefPtr<ICoreFactory> Win32::createInstance()
{
	return RefPtr<Internal::Win32CoreFactory>::create();
}