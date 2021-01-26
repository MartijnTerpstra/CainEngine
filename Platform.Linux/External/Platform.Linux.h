#pragma once

#include <Common.h>
#include <Platform.Core.h>

#include <X11/Xlib.h>

// Forward declarations
namespace CainEngine {
namespace Platform {
namespace Linux {

class ILinuxWindow;
class ILinuxCoreFactory;

}; // namespace Windows
}; // namespace Platform
}; // namespace CainEngine

#include "Platform.Linux/ILinuxWindow.h"
#include "Platform.Linux/ILinuxCoreFactory.h"