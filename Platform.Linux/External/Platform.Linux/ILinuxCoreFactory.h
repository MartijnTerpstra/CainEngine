#pragma once

namespace CainEngine {
namespace Platform {
namespace Linux {

class ILinuxCoreFactory : public ICoreFactory, public Common::Castable<ILinuxCoreFactory>
{
	COMMON_DECLARE_INTERFACE(ILinuxCoreFactory);

public:
	// Singleton functionality

	static shared_ptr<ILinuxCoreFactory> CreateInstance();

}; // class ILinuxCoreFactory

inline ILinuxCoreFactory::~ILinuxCoreFactory() = default;

}; // namespace Linux
}; // namespace Platform
}; // namespace CainEngine