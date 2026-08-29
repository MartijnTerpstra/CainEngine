#include "Precomp.h"
#include "XorgCoreFactory.h"

using namespace ::CainEngine::Platform;

RefPtr<ICoreFactory> Xorg::CreateInstance()
{
	return RefPtr<Internal::XorgCoreFactory>::Create();
}
