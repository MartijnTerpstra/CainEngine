#include "Precomp.h"
#include "LinuxCoreFactory.h"

using namespace ::CainEngine::Platform;

RefPtr<ICoreFactory> Linux::createInstance()
{
	return RefPtr<Internal::LinuxCoreFactory>::create();
}
