#pragma once

#include <Platform.Win32.h>
#include <Graphics.h>

namespace CainEngine {
namespace Graphics {
namespace DX11 {

std::unique_ptr<API::IRenderer> createInstance();

}; // namespace DX11
}; // namespace Graphics
}; // namespace CainEngine