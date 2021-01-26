#pragma once

namespace CainEngine::Platform {

class ISplashScreen
{
	COMMON_DECLARE_INTERFACE(ISplashScreen);

public:
	// Main functionality

	virtual void Show() = 0;

	virtual void Hide() = 0;

};

}