#pragma once

#include <Platform.h>
#include <Graphics.h>

namespace CainEngine {
namespace Graphics {
namespace Vulkan {

std::unique_ptr<API::IRenderer> createInstance();

}; // namespace Vulkan
}; // namespace Graphics
}; // namespace CainEngine
