#include "Precomp.h"
#include "Win32CoreFactory.h"

using namespace ::CainEngine::Platform;

RefPtr<ICoreFactory> Win32::CreateInstance()
{
	return RefPtr<Internal::Win32CoreFactory>::Create();
}