#pragma once

#include <Common.h>
#include <Platform.h>

#include <X11/Xlib.h>

// Forward declarations
namespace CainEngine {
namespace Platform {
namespace Linux {

class IXorgWindow;

RefPtr<ICoreFactory> createInstance();

}; // namespace Linux
}; // namespace Platform
}; // namespace CainEngine

#include "Platform.Linux/IXorgWindow.h"
