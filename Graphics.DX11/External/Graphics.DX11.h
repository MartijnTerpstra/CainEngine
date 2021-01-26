#pragma once

#include <Platform.Win32.h>
#include <Graphics.h>

namespace CainEngine {
namespace Graphics {
namespace DX11 {

unique_ptr<API::IRenderer> CreateInstance();

}; // namespace DX11
}; // namespace Graphics
}; // namespace CainEngine