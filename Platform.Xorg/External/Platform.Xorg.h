#pragma once

#include <Common.h>
#include <Platform.h>

#include <X11/Xlib.h>

// Forward declarations
namespace CainEngine {
namespace Platform {
namespace Xorg {

class IXorgWindow;

RefPtr<ICoreFactory> createInstance();

}; // namespace Xorg
}; // namespace Platform
}; // namespace CainEngine

#include "Platform.Xorg/IXorgWindow.h"
