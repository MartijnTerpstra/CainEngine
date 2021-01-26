#pragma once

#include <Common.h>

// Forward declarations
namespace CainEngine {
namespace Platform {

using Common::RefPtr;

class IWindow;
class IInput;
class IProcess;
class ICoreFactory;

namespace ClientInterfaces {

class IWindowEventListener;

}; // namespace ClientInterfaces

}; // namespace Platform
}; // namespace CainEngine

#include "Platform/Enums.h"
#include "Platform/Structs.h"
#include "Platform/ISplashScreen.h"
#include "Platform/ClientInterfaces/IWindowEventListener.h"
#include "Platform/IWindow.h"
#include "Platform/IInput.h"
#include "Platform/IProcess.h"
#include "Platform/IMonitor.h"
#include "Platform/ICoreFactory.h"