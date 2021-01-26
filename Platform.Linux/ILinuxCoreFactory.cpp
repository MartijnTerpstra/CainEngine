#include "Precomp.h"

#include "LinuxCoreFactory.h"

using namespace ::CainEngine::Platform;
using namespace ::CainEngine::Platform::Linux;

shared_ptr<ILinuxCoreFactory> ILinuxCoreFactory::CreateInstance()
{
	COMMON_CALLSTACK_CALL;

	static bool alreadyCreated = false;

	if (alreadyCreated)
	{
		Common::FatalError("invalid call: can only be called once");
	}

	alreadyCreated = true;
	return make_shared<Internal::LinuxCoreFactory>();
}