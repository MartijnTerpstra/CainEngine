#pragma once

#include <Common.h>
#include <Platform.h>

#include <Windows.h>

// Forward declarations
namespace CainEngine {
namespace Platform {
namespace Win32 {

class IWin32Window;

RefPtr<ICoreFactory> CreateInstance();

}; // namespace Windows
}; // namespace Platform
}; // namespace CainEngine

#include "Platform.Win32\IWin32Window.h"