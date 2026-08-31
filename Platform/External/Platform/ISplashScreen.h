#pragma once

namespace CainEngine::Platform {

class ISplashScreen
{
	COMMON_DECLARE_INTERFACE(ISplashScreen);

public:
	// Main functionality

	virtual void show() = 0;

	virtual void hide() = 0;

};

}