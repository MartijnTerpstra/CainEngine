#pragma once

#define _PLATFORM_WINDOWS_DECLARE_DLL_TYPE MST_DECLARE_DLL_TYPE_GENERATE
#define _PLATFORM_WINDOWS_DECLARE_DLL_FUNCTION MST_DECLARE_DLL_FUNCTION_GENERATE

#include "External\Platform.Win32.h"

namespace CainEngine {
namespace Graphics {
namespace Internal {

class Win32Process;
class Win32Monitor;
class Win32Window;
class Win32Environment;
class Win32CoreFactory;

}; // namespace Internal
}; // namespace Graphics
}; // namespace CainEngine