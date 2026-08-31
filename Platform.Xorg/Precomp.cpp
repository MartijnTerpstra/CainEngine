#include "Precomp.h"
#include "XorgCoreFactory.h"

using namespace ::CainEngine::Platform;

RefPtr<ICoreFactory> Xorg::createInstance()
{
	return RefPtr<Internal::XorgCoreFactory>::create();
}
